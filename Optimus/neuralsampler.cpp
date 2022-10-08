#include "neuralsampler.h"
# include "tolmin.h"
NeuralSampler::NeuralSampler(Problem *p,int w)
    :ProblemSampler("neural_sampler",p)
{
    int dimension = p->getDimension();
    weight.resize((dimension+2)*w);
    dataset = NULL;
}

int     NeuralSampler::getNWeights() const
{
    return weight.size();
}

Data    NeuralSampler::getWeights()
{
    return weight;
}
static double sig(double arg)
{
    return 1.0/(1.0+exp(-arg));

}
void    NeuralSampler::setWeights(Data &w)
{
    weight = w;
}

double  NeuralSampler::eval(Data &x)
{
    int dimension = x.size();
    int nodes=weight.size()/ (dimension + 2);
    double per=0.0;
   for(int i=1;i<=nodes;i++)
        {
            double arg=0.0;
            for(int j=1;j<=dimension;j++)
            {
                int pos=(dimension+2)*i-(dimension+1)+j;
                arg+=x[j-1]*weight[pos-1];
            }
            arg+=weight[(dimension+2)*i-1];
            per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);

        }
        return per;
}

Data    NeuralSampler::evalDeriv()
{
    Data g;
    g.resize(weight.size());
    for(int i=0;i<g.size();i++)
        g[i]=0.0;
    for(int i=0;i<dataset->getpatterns();i++)
    {
        Data trainx = dataset->getx(i);
        Data gtemp = evalDeriv(trainx);

        double per=eval(trainx)-dataset->gety(i);
        for(int j=0;j<g.size();j++)	g[j]+=gtemp[j]*per;
    }
    for(int j=0;j<g.size();j++) g[j]*=2.0;
    return g;

}
Data    NeuralSampler::evalDeriv(Data &xpoint)
{
    Data g;
    g.resize(weight.size());

    int dimension = xpoint.size();
    for(int i=0;i<g.size();i++) g[i]=0.0;
    Data tempg;
    Data tempg2;
    tempg.resize(g.size());
    tempg2.resize(g.size());

    int nodes=weight.size()/ (dimension + 2);
    for(int i=1;i<=nodes;i++)
    {
        double arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j;
            arg+=xpoint[j-1]*weight[pos-1];
        }
        arg+=weight[(dimension+2)*i-1];
        double s=sig(arg);
        double s2=s*(1-s);
        g[(dimension+2)*i-(dimension+1)-1]=s;
        g[(dimension+2)*i-1]=weight[(dimension+2)*i-(dimension+1)-1]*s2;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j;
            g[pos-1]=weight[(dimension+2)*i-(dimension+1)-1]*xpoint[j-1]*s2;
        }
    }
    return g;
}

void    NeuralSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
    if(dataset!=NULL)
        delete dataset;
    dataset = new Dataset(myProblem,N);
    xsample.resize(N);
    ysample.resize(N);
    for(int i=0;i<N;i++)
    {
        xsample[i]=dataset->getx(i);
        ysample[i]=dataset->gety(i);
    }
}


class NeuralProblem : public IntervalProblem
{
private:
    NeuralSampler *thisSampler;
public:
    NeuralProblem(NeuralSampler *p);
    double      funmin(Data &x);
    void        granal(Data &x,Data &g);
    ~NeuralProblem();
};

NeuralProblem::NeuralProblem(NeuralSampler *p)
    :IntervalProblem(p->getNWeights())
{
    thisSampler = p;
    vector<Interval> margin;
    margin.resize(getDimension());
    unsigned int i;
    for(i=0;i<getDimension();i++)
    {
        margin[i]=Interval(-100.0,100.0);
    }
    setMargins(margin);

}

double      NeuralProblem::funmin(Data &x)
{
    thisSampler->setWeights(x);
    return thisSampler->getTrainError();
}

static double dmax(double a,double b)
{
    return a>b?a:b;
}

void        NeuralProblem::granal(Data &x,Data &g)
{
    thisSampler->setWeights(x);
    g = thisSampler->evalDeriv();
    return;
    for(int i=0;i<(int)x.size();i++)
        {
            double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
            x[i]+=eps;
            double v1=funmin(x);
            x[i]-=2.0 *eps;
            double v2=funmin(x);
            g[i]=(v1-v2)/(2.0 * eps);
            x[i]+=eps;
        }

}

NeuralProblem::~NeuralProblem()
{
    //nothing for now
}

double  NeuralSampler::getTrainError()
{
    double sum = 0.0;
    for(int i=0;i<dataset->getpatterns();i++)
    {
       Data x = dataset->getx(i);
       double v1= eval(x);
       double v2= dataset->gety(i);
       sum+=(v1-v2)*(v1-v2);
    }
    return sum;
}

void    NeuralSampler::trainModel()
{
    NeuralProblem *problem = new NeuralProblem(this);
    Problem *np=new Problem(problem);
    Tolmin tol(np);
    for(int i=0;i<weight.size();i++)
    {
        weight[i]=0.1*(2.0*drand48()-1.0);
    }
    double value = tol.Solve(weight,false);
    delete np;
    delete  problem;
}

void    NeuralSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    const int NMany = 10 * N;
    xsample.resize(NMany);
    ysample.resize(NMany);
    for(int i=0;i<NMany;i++)
    {
        xsample[i]=myProblem->getRandomPoint();
        ysample[i]=eval(xsample[i]);
    }
    //sort
    for(int i=0;i<NMany;i++)
    {
        for(int j=0;j<NMany-1;j++)
        {
            if(ysample[j+1]<ysample[j])
            {
                Data tx = xsample[j];
                xsample[j]=xsample[j+1];
                xsample[j+1]=tx;
                double dx = ysample[j];
                ysample[j]=ysample[j+1];
                ysample[j+1]=dx;
            }
        }
    }

    xsample.resize(N);
    ysample.resize(N);
}

NeuralSampler::~NeuralSampler()
{
    if(dataset!=NULL)
        delete dataset;
}

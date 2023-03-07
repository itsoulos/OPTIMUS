#include "neuralnetwork.h"
# define ADEPT_GRANAL

NeuralNetwork::NeuralNetwork(Dataset *tr,int N)
{
    normalTrain = 1;
    nodes = N;
    trainSet = tr;
    dimension = trainSet->getdimension();
    weight.resize((dimension+2)*nodes);
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        double trainy = trainSet->gety(i);
    bool found=false;
    for(int j=0;j<dclass.size();j++)
    {
        if(fabs(dclass[j]-trainy)<1e-7)
        {
            found=true;
            break;
        }

    }
    if(!found)
        dclass.push_back(trainy);
    }
}

void    NeuralNetwork::disableNormalTrain()
{
    normalTrain = 0;
}

void    NeuralNetwork::enableNormalTrain()
{
    normalTrain = 1;
}

adept::adouble	NeuralNetwork::adsig(adept::adouble x)
{
    return 1.0/(1.0+exp(-x));
}

double	NeuralNetwork::dsig(double x)
{
    return 1.0/(1.0+exp(-x));
}

double	NeuralNetwork::dgetValue(Data &node,Data &x,int &fcount)
{
    double arg=0.0;
    double per=0.0;
    for(int i=1;i<=nodes;i++)
    {
        arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j-1;
            arg+=node[pos]*x[j-1];
        }
        arg+=node[(dimension+2)*i-1];
        fcount+=(fabs(arg)>=10.0);
        per+=node[(dimension+2)*i-(dimension+1)-1]*dsig(arg);
    }
    return per;
}

adept::adouble	NeuralNetwork::adgetValue(adept::aVector &node,Data &x,int &fcount)
{
    adept::adouble arg=0.0;
    adept::adouble per=0.0;
    for(int i=1;i<=nodes;i++)
    {
        arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j-1;
            arg+=node[pos]*x[j-1];
        }
        arg+=node[(dimension+2)*i-1];
        fcount+=(fabs(arg)>=10.0);
        per+=node[(dimension+2)*i-(dimension+1)-1]*adsig(arg);
    }
    return per;
}

void	NeuralNetwork::getDeriv(Data &node,Data &x,Data &G)
{
    double arg;
        double f,f2;
        for(int i=1;i<=nodes;i++)
        {
                arg = 0.0;
                for(int j=1;j<=dimension;j++)
                {
                        arg+=node[(dimension+2)*i-(dimension+1)+j-1]*x[j-1];
                }
                arg+=node[(dimension+2)*i-1];
                f=dsig(arg);
                f2=f*(1.0-f);
                G[(dimension+2)*i-1]=node[(dimension+2)*i-(dimension+1)-1]*f2;
                G[(dimension+2)*i-(dimension+1)-1]=f;
                for(int k=1;k<=dimension;k++)
                {
                        G[(dimension+2)*i-(dimension+1)+k-1]=
                                x[k-1]*f2*node[(dimension+2)*i-(dimension+1)-1];
                }
    }
}

void NeuralNetwork::getOriginalGranal(Data &node,Data &g)
{
    for(int i=0;i<g.size();i++) g[i]=0.0;
    Data gtemp;
    gtemp.resize(g.size());
    int fcount=0;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xx = trainSet->getx(i);
        getDeriv(node,xx,gtemp);
        double per=dgetValue(node,xx,fcount)-trainSet->gety(i);
        for(int j=0;j<g.size();j++)	g[j]+=gtemp[j]*per;
    }
    for(int j=0;j<(dimension+2)*nodes;j++) g[j]*=2.0;
}


void	NeuralNetwork::getGradient(Data &node,Data &g)
{
    {
        adept::Stack stack;
        g.resize(node.size());
        for(int i=0;i<g.size();i++) g[i]=0.0;
        Data gtemp;
        gtemp.resize(g.size());
        int fcount=0;
        QJsonObject    done(Data &x);
    #ifdef ADEPT_GRANAL
        stack.continue_recording();

        adept::aVector w(node.size());
        for(unsigned i = 0; i < node.size();i++) w<<node[i];// w[i] = node[i];
        stack.new_recording();
        adept::adouble error;


        error = afunmin(w);
        error.set_gradient(1.0);
        stack.compute_adjoint();
        for(unsigned i = 0; i < node.size();i++) {
            g[i] = w[i].get_gradient();
            //std::cout << i << " " << g[i] << std::endl;
        }
        stack.pause_recording();
    #else

        for(int i=0;i<trainx.size();i++)
        {
            getDeriv(node,trainx[i],gtemp);
            double per=dgetValue(node,trainx[i],fcount)-trainy[i];
            for(int j=0;j<g.size();j++)	g[j]+=gtemp[j]*per;
        }
        for(int j=0;j<(dimension+2)*nodes;j++) g[j]*=2.0;
    #endif
    }
}

adept::adouble NeuralNetwork::afunmin(adept::aVector &Weights)
{
    adept::adouble sum=0.0;
    adept::adouble per=0.0;
    int fcount=0;
    Data A;
    A.resize(Weights.size());
    for(unsigned i = 0; i < A.size();i++) {
        A[i] = double(Weights[i].value());
    }
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xx = trainSet->getx(i);
        per=adgetValue(Weights,xx,fcount)-trainSet->gety(i);
        sum+=per * per;
    }
    if(normalTrain==1) return sum;

    return sum*(1.0+10.0*pow(fcount*1.0/(nodes * trainSet->getpatterns()),2.0));

}
double	NeuralNetwork::funmin(vector<double> &x)
{
    double sum=0.0;
    double per=0.0;
    int fcount=0;
    weight = x;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xx = trainSet->getx(i);
        per=dgetValue(x,xx,fcount)-trainSet->gety(i);
        sum+=per * per;
    }
    if(normalTrain==1) return sum;
    return sum*(1.0+10.0*pow(fcount*1.0/(nodes * trainSet->getpatterns()),2.0));
}

void    NeuralNetwork::granal(vector<double> &x,vector<double> &g)
{
    if(normalTrain==1) getOriginalGranal(x,g);
    else
    {
            getGradient(x,g);

    }
}

double NeuralNetwork::getTrainError()
{
    double sum = 0.0;
    int fcount = 0;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xx = trainSet->getx(i);
        double v = dgetValue(weight,xx,fcount);
        double yy = trainSet->gety(i);
        sum+=(v-yy)*(v-yy);
    }
    return sum;
}

double  NeuralNetwork::getTestError(Dataset *tt)
{
    double sum = 0.0;
    int fcount = 0;
    for(int i=0;i<tt->getpatterns();i++)
    {
        Data xx = tt->getx(i);
        double v = dgetValue(weight,xx,fcount);
        double yy = tt->gety(i);
        sum+=(v-yy)*(v-yy);
    }
    return sum;
}

double  NeuralNetwork::getClassTestError(Dataset *tt)
{

    double sum = 0.0;
    int fcount = 0;
    for(int i=0;i<tt->getpatterns();i++)
    {
        Data xx = tt->getx(i);
        double v = nearestClass( dgetValue(weight,xx,fcount));
        double yy = tt->gety(i);
        sum+=(fabs(v-yy)>=1e-5);
    }
    return sum*100.0/tt->getpatterns();
}

double NeuralNetwork::nearestClass(double y)
{
    int ifound=-1;
    double dmin=1e+100;
    for(int i=0;i<dclass.size();i++)
    {
        if(fabs(dclass[i]-y)<dmin)
        {
            dmin=fabs(dclass[i]-y);
            ifound=i;
        }
    }
    return dclass[ifound];
}

NeuralNetwork::~NeuralNetwork()
{

}

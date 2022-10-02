#include "rbfsampler.h"

RbfSampler::RbfSampler(Problem *p,int w)
    :ProblemSampler("rbfsampler",p)
{
    nweights = w;
    weight.resize(nweights);
    variance.resize(nweights);
    center.resize(nweights);
    for(int i=0;i<nweights;i++)
    {
        center[i].resize(myProblem->getDimension());
    }
}

double  RbfSampler::getDistance(Data &x,Data &y)
{
    double sum = 0.0;
    for(unsigned int i=0;i<x.size();i++)
        sum+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum);
}

double  RbfSampler::gaussian(Data &x,Data &m,double v)
{
    double dist =getDistance(x,m);
    return exp(-dist * dist/v);
}

int     RbfSampler::getNWeights() const
{
    return nweights;
}

Data    RbfSampler::getWeights()
{
    return weight;
}

void    RbfSampler::setWeights(Data &w)
{
    weight = w;
}

double  RbfSampler::eval(Data &x)
{
    double sum = 0.0;
    for(int i=0;i<(int)weight.size();i++)
    {
        double px = gaussian(x,center[i],variance[i]);
        sum+=weight[i] * px;
    }
    return sum;
}

void    RbfSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
    xpoint.resize(N);
    ypoint.resize(N);
    for(int i=0;i<N;i++)
    {
        xpoint[i]=myProblem->getRandomPoint();
        ypoint[i]=myProblem->funmin(xpoint[i]);
    }
    member.resize(N);
    for(int i=0;i<N;i++)
        member[i]=-1;
    xsample = xpoint;
    ysample = ypoint;
}

void    RbfSampler::initCenters()
{
    int i;
    srand(0);
    for(i=0;i<member.size();i++)
    {
        member[i]=rand() % nweights;
    }
    updateCenters();
}
void    RbfSampler::kmeans()
{
    int i,j;
    initCenters();
    vector<Data> copyCenters;
    while(true)
    {
        copyCenters=center;
        for(i=0;i<member.size();i++)
        {
            Data x=xpoint[i];
            int t=nearestTeam(x);
            member[i]=t;
        }
        updateCenters();
        double totalDistance=0.0;
        for(i=0;i<center.size();i++)
            totalDistance+=getDistance(center[i],copyCenters[i]);
        if(totalDistance<1e-5) break;
    }
    for(i=0;i<center.size();i++)
    {
        variance[i]=0.0;
    }
    double total_var=0.0;
    for(i=0;i<center.size();i++)
    {
        double sum=0.0;
        for(j=0;j<xpoint.size();j++)
        {
            if(member[j]==i)
            {
                Data x=xpoint[j];
                sum+=getDistance(center[i],x);
            }
        }
        //an exei mono ena melos mia omada
        if(teamMembers[i]==1)
             variance[i]=sqrt(1.0/(teamMembers[i]) * sum);
        else
        variance[i]=sqrt(1.0/(teamMembers[i]-1.0) * sum);
        total_var+=variance[i];
    }
    //gia na apofygoume poly xamiles times
    for(i=0;i<center.size();i++)
        variance[i]=total_var;
}

int     RbfSampler::nearestTeam(Data &x)
{
    int i;
    double minDist=1e+100;
    int imin=-1;
    for(i=0;i<nweights;i++)
    {
        double dist=getDistance(x,center[i]);
        if(dist<minDist)
        {
            minDist=dist;
            imin=i;
        }
    }
    return imin;
}

void    RbfSampler::updateCenters()
{
 int i,j;
 for(i=0;i<nweights;i++)
 {
     for(j=0;j<center[i].size();j++)
         center[i][j]=0.0;
 }
 teamMembers.resize(nweights);
 for(int i=0;i<nweights;i++)
     teamMembers[i]=0;

 for(i=0;i<member.size();i++)
 {
     teamMembers[member[i]]++;
 }
 for(i=0;i<member.size();i++)
 {
     Data x=xpoint[i];
     int whatTeam=member[i];
     for(j=0;j<x.size();j++)
     {
         center[whatTeam][j]+=x[j]/teamMembers[whatTeam];
     }
 }
}


Matrix  RbfSampler::matrix_transpose(Matrix &x)
{
    Matrix xx;
    xx.resize(x[0].size());
    int i,j;
    for(i=0;i<xx.size();i++)
    {
        xx[i].resize(x.size());
        for(j=0;j<x.size();j++)
        {
            xx[i][j]=x[j][i];
        }
    }
    return xx;
}

Matrix  RbfSampler::matrix_mult(Matrix &x,Matrix &y)
{
    int m=x.size();
    int p=x[0].size();
    int n=y[0].size();
    if(p!=y.size())
    {
        printf("Impossible to multiple \n");

    }
    else
    {
        Matrix z;
        z.resize(m);
        int i,j,k;
        for(i=0;i<m;i++) z[i].resize(n);
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                z[i][j]=0.0;
                for(k=0;k<p;k++)
                {
                    z[i][j]=z[i][j]+x[i][k]*y[k][j];

                }

            }
        }
        return z;
    }
}

Matrix  RbfSampler::matrix_inverse(Matrix x,bool &ok)
{
    Matrix c=x;
    ok=true;
    int npivot;
    double det;
    int pass, row, col, maxrow, i, j, error_flag;
    double temp, pivot, mult;
    int n=x.size();
     for(i=0; i<n; ++i) {
        for(j=0; j<n; ++j) {
            if(i==j) {
            c[i][j] = 1.0;
            } else {
            c[i][j] = 0.0;
              }
        }
       }

       det=1.0;
       npivot=0;


       for(pass=0; pass<n; ++pass) {
        maxrow=pass;
        for(row=pass; row<n; ++row)
            if(fabs(x[row][pass]) > fabs(x[maxrow][pass]))
            maxrow=row;

        if(maxrow != pass)
            ++npivot;

        for(col=0; col<n; ++col) {
            temp=x[pass][col];
            x[pass][col] = x[maxrow][col];
            x[maxrow][col] = temp;
            temp = c[pass][col];
            c[pass][col] = c[maxrow][col];
            c[maxrow][col] = temp;
        }


        pivot = x[pass][pass];
        det *= pivot;

        /*
        if(fabs(det) < 1.0e-40) {
           ok=false;
            return c;
        }*/


        for(col=0; col<n; ++col) {
            if(fabs(pivot)<1e-7)
            {
                ok=false;
                return c;
            }
            x[pass][col] = x[pass][col]/pivot;
            c[pass][col] = c[pass][col]/pivot;

        }

        for(row=0; row<n; ++row) {
            if(row != pass) {
                mult = x[row][pass];
                for(col=0; col<n; ++col) {
                    x[row][col] = x[row][col] - x[pass][col] * mult;
                    c[row][col] = c[row][col] - c[pass][col] * mult;
                }
            }
        }

       }

       if(npivot % 2 != 0)
        det = det * (-1.0);
      return c;
}

Matrix  RbfSampler::matrix_pseudoinverse(Matrix &a,bool &ok)
{

    Matrix b=matrix_transpose(a);
    Matrix e=matrix_mult(b,a);
    Matrix d=matrix_inverse(e,ok);
    Matrix c=matrix_mult(d,b);
    return c;
}



void    RbfSampler::trainModel()
{
    //kmeans first
    kmeans();
    Matrix A;
    Matrix RealOutput;
    int i,j;
    A.resize(xpoint.size());
    RealOutput.resize(xpoint.size());

    for(i=0;i<A.size();i++)
    {
        RealOutput[i].resize(1);
        A[i].resize(center.size());
    }

    for(i=0;i<A.size();i++)
    {
        RealOutput[i][0]=ypoint[i];
        Data x=xpoint[i];
        for(j=0;j<center.size();j++)
        {
          A[i][j]=gaussian(x,center[j],variance[j]);
        }
    }
    bool ok=true;
    Matrix pA=matrix_pseudoinverse(A,ok);
    Matrix pW=matrix_mult(pA,RealOutput);
    for(i=0;i<nweights;i++) weight[i]=pW[i][0];
    //then train
}

void    RbfSampler::sampleFromModel(int &N,vector<Data> &xsample,Data &ysample)
{
    //sample many points and take the N lowest values
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

  /*  Matrix finalXsample;
    Data finalYSample;
    for(int i=0;i<N;i++)
    {
        double minDist =1e+100;
        int iminDist =-1;
        for(int j=0;j<i;j++)
        {
            if(i==j) continue;
            Data gi,gj;
            gi=evalDeriv(xsample[i]);
            gj=evalDeriv(xsample[j]);
            double sum = 0.0;
            for(int k=0;k<gi.size();k++)
            {
                sum+=(xsample[i][k]-xsample[j][k])*(gi[k]-gj[k]);
            }
             if(fabs(ysample[i]-ysample[j])<1e-7) {
                 iminDist = j;
                 continue;
             }
            double d= getDistance(xsample[i],xsample[j])/myProblem->getVolume();
            if(sum<0)
            {
                if(d<minDist || iminDist==-1)
                {
                    minDist = d;
                    iminDist =j;
                }
            }

        }
        if(iminDist>=0)
        {

            if(fabs(ysample[i]-ysample[iminDist])<1e-5) continue;
            if(minDist>0.10)
            {
                finalXsample.push_back(xsample[i]);
                finalYSample.push_back(ysample[i]);
            }
        }
        else
        {
            finalXsample.push_back(xsample[i]);
            finalYSample.push_back(ysample[i]);
        }

    }
    N = finalXsample.size();
    xsample = finalXsample;
    ysample = finalYSample;*/
}

double  RbfSampler::gaussianDerivative(Data &x,Data &m,double v,int pos)
{
    double hx = gaussian(x,m,v);
    return hx * (-2.0/v)*(x[pos]-m[pos]);
}

Data  RbfSampler::evalDeriv(Data &x)
{
    Data g;
    g.resize(x.size());
    for(int pos=0;pos<x.size();pos++)
    {

        double sum = 0.0;
        for(int i=0;i<(int)weight.size();i++)
        {
            double px = gaussianDerivative(x,center[i],variance[i],pos);
            sum = sum + weight[i]*px;
        }
        g[pos]=sum;
    }
    return g;
}
RbfSampler::~RbfSampler()
{
    //nothing for now
}

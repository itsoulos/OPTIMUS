#include "maxwellsampler.h"

MaxwellSampler::MaxwellSampler(Problem *p)
    :ProblemSampler("maxwell",p)
{

}
double  MaxwellSampler::eval(Data &xpoint)
{
    return 0.0;
}
void    MaxwellSampler::addSampleFromProblem(Data &x,double y)
{
    //nothing
}
void    MaxwellSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
    //unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    //std::default_random_engine generator(seed);
    std::random_device seed;
    std::mt19937 generator(seed());
    const double k_T = 0.1;
    gamma_distribution<double> maxwell(3./2., k_T);
    int n =myProblem->getDimension();
    Data x,a,b;
    a = myProblem->getLeftMargin();
    b = myProblem->getRightMargin();
    a.resize(n);
    b.resize(n);
    xpoint.resize(N);
    ypoint.resize(N);
    x.resize(n);
    for(int i=0;i<N;i++)
    {
        xpoint[i].resize(n);
        for(int j=0;j<n;j++)
        {
            double r=maxwell(generator);
            x[j]=a[j]+r*(b[j]-a[j]);
        }
        xpoint[i] =x;
        ypoint[i]=myProblem->funmin(xpoint[i]);
    }
   xsample = xpoint;
   ysample = ypoint;
}
void    MaxwellSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    xsample = xpoint;
    ysample = ypoint;
}

MaxwellSampler::~MaxwellSampler()
{

}

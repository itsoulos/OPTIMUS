#include "uniformsampler.h"

UniformSampler::UniformSampler(Problem *p)
    :ProblemSampler("uniform",p)
{

}
void    UniformSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
   int i;
   xpoint.resize(N);
   ypoint.resize(N);
   for(i=0;i<N;i++)
   {
       xpoint[i].resize(myProblem->getDimension());
       xpoint[i]=myProblem->getRandomPoint();
       ypoint[i]=myProblem->funmin(xpoint[i]);
   }
   xsample = xpoint;
   ysample = ypoint;
}
void    UniformSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    xsample = xpoint;
    ysample = ypoint;
}

UniformSampler::~UniformSampler()
{

}

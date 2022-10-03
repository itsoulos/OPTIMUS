#include "triangularsampler.h"

TriangularSampler::TriangularSampler(Problem *p)
    :ProblemSampler("triangular",p)
{

}
void    TriangularSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
   int i;
   xpoint.resize(N);
   ypoint.resize(N);
   int n =myProblem->getDimension();
   Data a, b, c, F;
   a.resize(n);
   b.resize(n);
   c.resize(n);
   F.resize(n);
   a = myProblem->getLeftMargin();
   b = myProblem->getRightMargin();
   //printf("n = %i\n",n);
   for (int j = 0; j < n; j++)
   {
       c[j] = a[j] + b[j] / 2.;
       F.at(j) = (c[j]-a[j]) / (b[j]-a[j]);
   }
   Data point;
   point.resize(n);
   for (int i = 0; i < N; i++)
   {
       xpoint.at(i).resize(n);
       xpoint.at(i) = myProblem->getRandomPoint();


       for (int j = 0; j < n; j++)
       {
           double U = rand() / (double)RAND_MAX;
           if (U <= F[j])
               point.at(j) = a[j] + sqrt(U * (b[j] - a[j]) * (c[j] - a[j]));
           else
               point.at(j) = b[j] - sqrt((1. - U) * (b[j] - a[j]) * (b[j] - c[j]));
       }
       xpoint.at(i) = point;
       ypoint.at(i) = myProblem->funmin(xpoint.at(i));
   }
   xsample = xpoint;
   ysample = ypoint;

}
void    TriangularSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    xsample = xpoint;
    ysample = ypoint;
}

TriangularSampler::~TriangularSampler()
{

}


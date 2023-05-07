#include "problemsampler.h"

ProblemSampler::ProblemSampler(QString n,Problem *p)
{
    name = n;
    myProblem = p;
}

void    ProblemSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
    //nothing to be done. It is a virtual class.
}

QString ProblemSampler::getName() const
{
    return name;
}

void    ProblemSampler::trainModel(QString m)
{
    //nothing to be done. It is a virtual class.
}

void    ProblemSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    //nothing to be done. It is a virtual class.
}

ProblemSampler::~ProblemSampler()
{
    //nothing to be done. It is a virtual class.
}

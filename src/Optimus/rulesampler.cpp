#include "rulesampler.h"
RuleSampler::RuleSampler(Problem *p)
    :ProblemSampler("rule",p)
{

}
void    RuleSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{

}

void    RuleSampler::trainModel()
{

}

void    RuleSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{

}

double  RuleSampler::eval(Data &xpoint)
{
    return 0.0;
}

void    RuleSampler::addSampleFromProblem(Data &x,double y)
{

}

RuleSampler::~RuleSampler()
{

}

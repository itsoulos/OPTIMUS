#ifndef RULESAMPLER_H
#define RULESAMPLER_H

#include "problemsampler.h"
#include "dataset.h"

class RuleSampler : public ProblemSampler
{
private:
    Dataset *train;
public:
    RuleSampler(Problem *p);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    trainModel();
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    QString getName() const;
    double eval(Data &xpoint)=0;
    void    addSampleFromProblem(Data &x,double y)=0;
    ~RuleSampler();
};

#endif // RULESAMPLER_H

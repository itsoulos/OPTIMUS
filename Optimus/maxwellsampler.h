#ifndef MAXWELLSAMPLER_H
#define MAXWELLSAMPLER_H

#include "problemsampler.h"
/**
 * @brief The MaxwellSampler class samples point from
 *        the objective problem using maxwell distribution.
 */
class MaxwellSampler : public ProblemSampler
{
private:
    Matrix xpoint;
    Data   ypoint;
public:
    MaxwellSampler(Problem *p);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    double  eval(Data &xpoint);
    void    addSampleFromProblem(Data &x,double y);
    ~MaxwellSampler();
};

#endif // MAXWELLSAMPLER_H

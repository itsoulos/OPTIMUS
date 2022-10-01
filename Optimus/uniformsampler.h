#ifndef UNIFORMSAMPLER_H
#define UNIFORMSAMPLER_H

#include "problemsampler.h"
/**
 * @brief The UniformSampler class samples point from
 *        the objective problem using uniform distribution.
 */
class UniformSampler : public ProblemSampler
{
private:
    Matrix xpoint;
    Data   ypoint;
public:
    UniformSampler(Problem *p);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    ~UniformSampler();
};

#endif // UNIFORMSAMPLER_H

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
    Data ypoint;

public:
    /**
     * @brief MaxwellSampler, initializes the sample allocator
     * @param p is the problem to be optimized.
     */
    MaxwellSampler(Problem *p);
    /**
     * @brief sampleFromProblem, sample allocator
     * @param Ν, number of samples.
     * @param xsample, sample table.
     * @param ysample f(x) table.
     */
    void sampleFromProblem(int N, Matrix &xsample, Data &ysample);
    /**
     * @brief sampleFromModel
     * @param Ν, number of samples.
     * @param xsample, sample table.
     * @param ysample f(x) table.
     */
    void sampleFromModel(int &N, Matrix &xsample, Data &ysample);
    double eval(Data &x);
    void addSampleFromProblem(Data &x,double y);
    /**
     * The destructor of the class.
     */
    ~MaxwellSampler();
};

#endif // MAXWELLSAMPLER_H

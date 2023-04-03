#ifndef TRIANGULARSAMPLER_H
#define TRIANGULARSAMPLER_H

#include "problemsampler.h"
/**
 * @brief The TriangularSampler class samples point from
 *        the objective problem using triangular distribution.
 */
class TriangularSampler : public ProblemSampler
{
private:
    Matrix xpoint;
    Data   ypoint;
public:
    TriangularSampler(Problem *p);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    double  eval(Data &xpoint);
    void    addSampleFromProblem(Data &x,double y);
    ~TriangularSampler();
};

#endif // TRIANGULARSAMPLER_H

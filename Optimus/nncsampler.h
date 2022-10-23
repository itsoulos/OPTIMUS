#ifndef NNCSAMPLER_H
#define NNCSAMPLER_H

#include "problemsampler.h"
# include "nncneuralprogram.h"
# include "dataset.h"
/**
 * @brief The NNCSampler class  implements sampling from the objective function using a constructed neural network.
 */
class NNCSampler : public ProblemSampler
{
private:
    NNCNeuralProgram *program;
    Dataset *dataset;
public:
    NNCSampler(Problem *p);
    double  eval(Data &xpoint);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    trainModel();
    void    addSampleFromProblem(Data &x,double y);
    void    setParameters(Data &x);
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    virtual ~NNCSampler();
};

#endif // NNCSAMPLER_H

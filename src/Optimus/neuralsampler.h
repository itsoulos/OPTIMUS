#ifndef NEURALSAMPLER_H
#define NEURALSAMPLER_H

#include "problemsampler.h"
# include "dataset.h"
/**
 * @brief The NeuralSampler class implements sampling from the objective function
 * using an artificial neural network.
 */
class NeuralSampler : public ProblemSampler
{
private:
    Data weight;
    Dataset *dataset;
public:
    NeuralSampler(Problem *p,int w);
    int     getNWeights() const;
    Data    getWeights();
    void    setWeights(Data &w);
    double  eval(Data &x);
    ///derivative at point x
    Data    evalDeriv(Data &x);
    //Total derivative for all dataset
    Data    evalDeriv();
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    trainModel();
    double  getTrainError();
    void    addSampleFromProblem(Data &x,double y);

    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    ~NeuralSampler();
};

#endif // NEURALSAMPLER_H

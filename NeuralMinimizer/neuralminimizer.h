#ifndef NEURALMINIMIZER_H
#define NEURALMINIMIZER_H

#include "NeuralMinimizer_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
# include <Optimus/rbfsampler.h>
# include <Optimus/neuralsampler.h>
class NeuralMinimizerInterface
{
public:
    virtual ~NeuralMinimizerInterface()
    {

    }
};
class NEURALMINIMIZER_EXPORT NeuralMinimizer :public
          Optimizer, NeuralMinimizerInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int olditer =0;
    NeuralSampler *sampler;
    int iter ;
    Matrix xsample;
    Data   ysample;
    Data bestx;
    double besty;
      double x1,x2,stopat,variance,oldBesty;
   Data minima;
public:
    NeuralMinimizer(Problem *p);
    virtual         void Solve();
    virtual         ~NeuralMinimizer();
};
extern "C" NEURALMINIMIZER_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // NEURALMINIMIZER_H

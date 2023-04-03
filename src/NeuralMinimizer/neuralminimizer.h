#ifndef NEURALMINIMIZER_H
#define NEURALMINIMIZER_H

#include "NeuralMinimizer_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
# include <Optimus/rbfsampler.h>
# include <Optimus/neuralsampler.h>
# include <Optimus/nncsampler.h>
class NeuralMinimizerInterface
{
public:
    virtual ~NeuralMinimizerInterface()
    {

    }
};
/**
 * @brief The NeuralMinimizer class implements a novell method that incorporates
 * machine learning models and a multistart like method to locate the global optimum.
 * Parameters:
 * 1)neural_model: The used machine learning model. Available values are: neural,rbf,nnc.
 * 2)neural_weights: The weights used in Rbf and Neural machine learning models.
 * 3)neural_samples: The number of samples used by the neural minimizer.
 * 4)neural_iterations: The maximum number of iterations used for the nnc model.
 * 5)neural_start_samples: The number of starts samples for the neural minimizer.
 * 6)neural_termination: The used termination rule. Available values are: similarity, doublebox.
 * 7)neural_similarityMaxValue: The maximum number of iterations for the similarity stopping rule.
 */
class NEURALMINIMIZER_EXPORT NeuralMinimizer :public
          Optimizer, NeuralMinimizerInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int olditer =0;
    NeuralSampler *neuralSampler;
    RbfSampler    *rbfSampler;
    NNCSampler    *nncSampler;
    ProblemSampler *sampler;
    int iter ;
    Matrix xsample;
    Data   ysample;
    Data bestx;
    double besty;

    //for the doublebox stopping rule
    double doublebox_xx1,doublebox_xx2,doublebox_stopat,
            doublebox_variance,doublebox_oldBesty;

   //for the similarity stopping rule
   double similarity_best_value;
   int similarity_current_count,similarity_max_count;

   Data minima;
public:
    NeuralMinimizer(Problem *p);
    virtual         void Solve();
    virtual         ~NeuralMinimizer();
};
extern "C" NEURALMINIMIZER_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // NEURALMINIMIZER_H

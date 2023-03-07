#ifndef FITPROBLEM_H
#define FITPROBLEM_H
# include <Optimus/intervalproblem.h>
# include <Optimus/neuralsampler.h>
# include <Optimus/rbfsampler.h>
# include <Optimus/nncsampler.h>
# include "dataproblem.h"
# include "neuralnetwork.h"

# define BOUNDWEIGHT_PHASE 0x1
# define TRAIN_PHASE     0x2
/**
 * @brief The FitProblem class is used for data fitting and
 *         data classifiction purposes.
 *         Parameters:
 *         trainName: the full path of train file
 *         testName:  the full path of test file
 *         weights:   the used weights for mlp
 *
 */
class FitProblem : public IntervalProblem
{
private:
    NeuralNetwork *network;
    QJsonObject     parameters;
    DataProblem     *dataProblem;
    Problem         *dataProblemWrapper;
public:
    FitProblem(QJsonObject &px);
    double      funmin(Data &x);
    void        setPhase(int phase,Data &x);
    void        granal(Data &x,Data &g);
    double      getTrainError();
    double      getTestError();
    double      getClassTestError();
    QJsonObject done(Data &x);
   // virtual void    getSample(Data &x);
    ~FitProblem();
};

#endif // FITPROBLEM_H

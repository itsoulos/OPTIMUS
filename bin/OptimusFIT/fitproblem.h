#ifndef FITPROBLEM_H
#define FITPROBLEM_H
# include <Optimus/intervalproblem.h>
# include <Optimus/neuralsampler.h>
# include <Optimus/rbfsampler.h>
# include <Optimus/nncsampler.h>
# include "dataproblem.h"
/**
 * @brief The FitProblem class is used for data fitting and
 *         data classifiction purposes.
 *         Parameters:
 *         trainName: the full path of train file
 *         testName:  the full path of test file
 *         model:     the name of the model (mlp,rbf,nnc)
 *         weights:   the used weights for rbf and mlp
 *         nncsize:   the size of the nnc chromosomes
 */
class FitProblem : public IntervalProblem
{
private:
    ProblemSampler *sampler;
    QJsonObject     parameters;
    DataProblem     *dataProblem;
    Problem         *dataProblemWrapper;
public:
    FitProblem(QJsonObject &px);
    double      funmin(Data &x);
    void        granal(Data &x,Data &g);
    double      getTrainError();
    double      getTestError();
    double      getClassTestError();
    QJsonObject done(Data &x);
    ~FitProblem();
};

#endif // FITPROBLEM_H

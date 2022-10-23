#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include "problem.h"
# include "tolmin.h"
# include "gradientdescent.h"
# include "adam.h"
# include "lbfgs.h"
#include "nelderMead.h"
# include "uniformsampler.h"
# include "rbfsampler.h"
# include "neuralsampler.h"
# include "maxwellsampler.h"
# include "triangularsampler.h"
# include "nncsampler.h"
# include "simplexmethod.h"
/*! \mainpage Optimus programming library
 *
 * This project helps user identify minima of a multidimensional function.
 * Installation instructions for Linux systems:
 * - \b Download and install the QT programming library from https://qt.io
 * - \b Download the software from https://github.com/itsoulos/OPTIMUS.git
 * - \b Set the OPTIMUSPATH environment variable pointing at the installation directory of OPTIMUS e.g.
 *     OPTIMUSPATH=/home/user/OPTIMUS/
 * - \b Set the LD_LIBRAPY_PATH to include the OPTIMUS/lib subdirectory e.g. LD_LIBRAPY_PATH=$LD_LIBRAPY_PATH:$OPTIMUSPATH/lib/:
 * - \b Issue the command:  cd $OPTIMUSPATH
 * - \b Execute the compilation script: ./compile.sh
 *
 *
 */

/**
 * @brief The Optimizer class, a base class for all the Optimizers created.
 */
class Optimizer
{
protected:
    Problem     *myProblem;
    QJsonObject params;
    ProblemSampler *defaultSampler;
    int     threads;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void    prepareSampler();
public:
    /**
     * @brief Optimizer the constructor of the class.
     * @param p,  the objective function.
     */
    Optimizer(Problem *p);
    /**
     * @brief sampleFromProblem, samples from the objective function a series of samples.
     * @param N, the number of samples.
     * @param xsample, the vector holding the samples.
     * @param ysample, the function values of the samples.
     */
    void        sampleFromProblem(int &N,Matrix &xsample,Data &ysample);
    /**
     * @brief setProblem changes the objective function
     * @param p, the new objective function to be used.
     */
    void            setProblem(Problem *p);
    /**
     * @brief setSettings, setup the settings of the optimizer in  json format.
     * @param settings
     */
    virtual void    setSettings(QJsonObject settings);
    /**
     * @brief setThreads changes the number of OpenMP threads used.
     * @param t, the number of threads
     */
    void            setThreads(int t);
    /**
     * @brief localSearch perform a localsearch starting from x.
     * @param x
     * @return
     */
    double          localSearch(Data &x);
    /**
     * @brief getProblem
     * @return  a pointer to the objective function.
     */
    Problem         *getProblem();
    /**
     * @brief Solve perform the steps of the optimizer.
     */
    virtual         void Solve();
    /**
     * @brief addParameter adds a new optimization parameter.
     * @param name The name of the parameter.
     * @param value The default value of the parameter.
     * @param description A description of the usage of the parameter.
     */
    void            addParameter(QString name,QString value,QString description);
    /**
     * @brief getParameter returns the value of a parameter
     * @param name the name of the parameter
     * @return the value
     */
    QString         getParameter(QString name);
    /**
     * @brief getParameterDescription returns the usage description of a parameter.
     * @param name the name of the parameter
     * @return the description.
     */
    QString         getParameterDescription(QString name);
    /**
     * @brief getParameters returns all the parameters of the optimizer
     * @return
     */
    QJsonObject     getParameters();
    ~Optimizer();
};

#endif // OPTIMIZER_H

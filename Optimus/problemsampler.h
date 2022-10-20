#ifndef PROBLEMSAMPLER_H
#define PROBLEMSAMPLER_H
# include "problem.h"

typedef vector<Data> Matrix;

/**
 * @brief The ProblemSampler class is a virtual class
 * to take samples from the real problem and produce (maybe)
 * additional samples.
 */
class ProblemSampler
{
protected:
    Problem *myProblem;
    QString name;
public:
    /**
     * @brief ProblemSampler, the constructor of the base class.
     * @param n, the name of the sample eg. uniform.
     * @param p, a pointer to the objective optimization problem.
     */
    ProblemSampler(QString n,Problem *p);
    /**
     * @brief sampleFromProblem, takes samples from the problem
     *                           using some distribution.
     * @param N                , the number of samples required.
     * @param xsample
     * @param ysample
     */
    virtual void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);

    /**
     * @brief trainModel, this method will train the sample models if
     *                    it is needed.
     */
    virtual void    trainModel();

    /**
     * @brief sampleFromModel, this method will sample from the sample model
     *                         instead of the real problem.
     * @param N              , the number of required samples.
     * @param xsample
     * @param ysample
     */
    virtual void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);

    /**
     * @brief getName
     * @return the name of the sampler.
     */
    QString getName() const;

    /**
     * @brief eval gets the value of model at point
     * @param xpoint
     * @return
     */
    virtual double eval(Data &xpoint)=0;


    /**
     * @brief addSampleFromProblem adds a new sample from the problem.
     * @param x  the xpoint of the new sample
     * @param y  the ypoint of the new sample
     */
    virtual void    addSampleFromProblem(Data &x,double y)=0;

    /**
     * @brief ~ProblemSampler, the virtual destructor of the class.
     */
    virtual ~ProblemSampler();
};

#endif // PROBLEMSAMPLER_H

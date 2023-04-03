#ifndef SIMPLEXMETHOD_H
#define SIMPLEXMETHOD_H
# include "problem.h"

typedef vector<Data> Matrix;

/**
 * @brief The SimplexMethod class implements the Simplex Local optimization method.
 */
class SimplexMethod
{
private:
    Matrix xpoint;
    Data ypoint;
    Problem *myProblem;
    Data xc, xr;
    double fc, fr;
    double alpha, beta, gamma;
    Data getCentroid();
    Data getReflected(Data &centroid);
    void sortPoints();
    double getMeanf();
    double getVariancef();
    void insertionStep();
    void stretchingStep();
    void contractionStep();

public:
    SimplexMethod(Problem *p);
    void setAlpha(double a);
    double getAlpha() const;
    void setBeta(double b);
    double getBeta() const;
    void setGamma(double g);
    double getGamma() const;
    void runAlgorithm();
    Data getBest();
    ~SimplexMethod();
};

#endif // SIMPLEXMETHOD_H

#ifndef PSOLOCAL_H
#define PSOLOCAL_H

# include <problem.h>
class PsoLocal
{
private:
    Problem *myProblem;
    vector<Data> position;
    vector<Data> bestPosition;
    vector<Data> velocity;
    Data bestg;
    Data fitnessArray;
    Data bestFitnessArray;
    double r1,r2;
    double percentageTheta;
    double  bestFitness;
    Data    lmargin;
    Data    rmargin;
public:
    PsoLocal(Problem *p,int particles);
    PsoLocal(Problem *p,Data &initialPoint,int particles,double theta);
    void    setR1(double r);
    void    setR2(double r);
    void    Solve(int generations);
    Data    gestBestParticle() const;
    ~PsoLocal();
};

#endif // PSOLOCAL_H

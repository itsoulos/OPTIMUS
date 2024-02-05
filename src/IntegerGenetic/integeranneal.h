#ifndef INTEGERANNEAL_H
#define INTEGERANNEAL_H
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
class IntegerAnneal
{
private:

    Problem *myProblem;
    vector<int> xpoint;
    double ypoint;
    vector<int> bestx;
    double besty;
    int neps=100;
    double T0;
    int k;
public:
    IntegerAnneal(Problem *pr);
    void    setNeps(int n);
    void    setT0(double t);
    void setPoint(vector<int> &g,double &y);
    void getPoint(vector<int> &g,double &y);
    void Solve();
    void    updateTemp();
    ~IntegerAnneal();
};
#endif // INTEGERANNEAL_H

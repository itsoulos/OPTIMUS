#include "gradientdescent.h"

GradientDescent::GradientDescent(Problem *p)
{
    myProblem=p;
    iters=10000;
    rate=0.01;
}

double  GradientDescent::Solve(Data &x)
{
    Data gradient;
    gradient.resize(x.size());
    Data lM=myProblem->getLeftMargin();
    Data rM=myProblem->getRightMargin();
    double f;
    for(unsigned i = 0; i < iters; i++){
            f=myProblem->funmin(x);
            myProblem->granal(x,gradient);
            for(unsigned j = 0; j < gradient.size(); j++){
                x[j] -= rate * gradient[j];
                if(x[j] > rM[j]) x[j] = rM[j];
                if(x[j] < lM[j]) x[j] = lM[j];
            }
            if(i % (iters/10) == 0)std::cout << "BGD ITER: " << i << " ERROR: " << f << std::endl;
            //getchar();
        }
   return f;
}


void   GradientDescent::setRate(double r)
{
    if(r>0 && r<=1) rate=r;
}

double GradientDescent::getRate() const
{
    return rate;
}

void  GradientDescent::setIters(int i)
{
    if(i>0)
        iters=i;
}

int   GradientDescent::getIters() const
{
    return iters;
}

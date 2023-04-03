#include "bfgs.h"

Bfgs::Bfgs(Problem *p)
    :Optimizer(p)
{

}

Bfgs::~Bfgs()
{

}

bool Bfgs::terminated()
{
    return true;
}


void Bfgs::step()
{
}

void Bfgs::init()
{
    initialx.resize(myProblem->getDimension());
    initialx=myProblem->getRandomPoint();
}

void Bfgs::done()
{
    double y=localSearch(initialx);
}

extern "C" BFGS_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Bfgs(p);
}

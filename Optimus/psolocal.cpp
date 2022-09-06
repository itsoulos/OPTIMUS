#include "psolocal.h"

PsoLocal::PsoLocal(Problem *p,Data &initialPoint,int particles,double theta)
{

    myProblem = p;
    bestg.resize(p->getDimension());

    percentageTheta = theta;
    fitnessArray.resize(particles);
    bestFitnessArray.resize(particles);
    r1 = 1.0;
    r2 = 1.0;

    position.resize(particles);
    velocity.resize(particles);
    bestPosition.resize(particles);
    bestFitness= 1e+100;

    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();

    for(int i=0;i<particles;i++)
    {
        position[i].resize(myProblem->getDimension());
        bestPosition[i].resize(myProblem->getDimension());
        velocity[i].resize(myProblem->getDimension());
        if(i==0)
        {
            position[i]=initialPoint;
        }
        else
        {
            for(int j=0;j<myProblem->getDimension();j++)
            {
                double value;
                bool okflag=false;
                do{
                value=initialPoint[j]-myProblem->randomDouble()*theta *(rmargin[j]-lmargin[j]);
                if(value<lmargin[j])
                {
                    value=initialPoint[j]+myProblem->randomDouble()*theta *(rmargin[j]-lmargin[j]);
                    if(value<rmargin[j]) okflag=true;
                }
                else okflag=true;
                }while(!okflag);
                position[i][j]=value;
            }
        }
        fitnessArray[i]=myProblem->funmin(position[i]);
        bestPosition[i]=position[i];
        bestFitnessArray[i]=fitnessArray[i];
        if(fitnessArray[i]<bestFitness)
        {
            bestFitness = fitnessArray[i];
            bestg = position[i];
        }
        for(int j=0;j<myProblem->getDimension();j++)
        {
                double left=-(rmargin[j]-lmargin[j])/20.0;
                double right=(rmargin[j]-lmargin[j])/20.0;
              velocity[i][j]=left+myProblem->randomDouble()*(right-left);
         }
    }
}

void    PsoLocal::setR1(double r)
{
    r1 = r;
}

void    PsoLocal::setR2(double r)
{
    r2 = r;
}

void    PsoLocal::Solve(int generations)
{
    for(int g=1;g<=generations;g++)
    {
        int particles = position.size();
        for(int i=0;i<particles;i++)
        {
            for(int j=0;j<myProblem->getDimension();j++)
            {
                double rt1 = myProblem->randomDouble();
                double rt2 = myProblem->randomDouble();
                velocity[i][j]=velocity[i][j]+rt1 * r1 * (bestPosition[i][j]-position[i][j])+
                         rt2 * r2 * (bestg[j]-position[i][j]);
                double tryValue = position[i][j]+velocity[i][j];
                if(tryValue>=lmargin[j] && tryValue<=rmargin[j])
                    position[i][j]=tryValue;
            }
            fitnessArray[i]=myProblem->funmin(position[i]);
            if(fitnessArray[i]<bestFitnessArray[i])
            {
                bestFitnessArray[i]=fitnessArray[i];
                bestPosition[i]=position[i];
            }
            if(fitnessArray[i]<bestFitness)
            {
                bestFitness = fitnessArray[i];
                bestg = position[i];
            }
        }
      //  printf("PSO LOCAL ITER=%4d VALUE=%20.10lg\n",g,bestFitness);
    }
}

Data    PsoLocal::gestBestParticle() const
{
    return bestg;
}

PsoLocal::~PsoLocal()
{

}

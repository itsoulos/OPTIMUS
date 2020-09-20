#include "minfinder.h"

Minfinder::Minfinder(Problem *p)
    :Optimizer(p)
{
    addParameter("minfinder_samples","25","Number of minfinder samples");
    addParameter("minfinder_iterations","200","Number of maximum iterations");
    addParameter("minfinder_epsilon","0.000001","Epsilon for comparisons");
    addParameter("minfinder_sampling","uniform","Sampling method: uniform,repulsion");
}



bool  Minfinder::checkForGradientCriterion(Data &x)
{
    double eps=params["minfinder_epsilon"].toString().toDouble();
    double dmin=1e+100;
    int imin=0;
    vector<Data> localCopy;
#pragma omp critical
    localCopy=minimax;
    if(localCopy.size()>0)
    {

    dmin=getDistance(localCopy[0],x);
    for(int j=0;j<localCopy.size();j++)
    {
        double d=getDistance(localCopy[j],x);
        if(d<dmin)
        {
            imin=j;
            dmin=d;
        }
    }
    }
    else return false;
    if(dmin<eps || (dmin<RC/localSearchCount && myProblem->getGradientCriterion(x,localCopy[imin])))
        return true;
    return false;
}

bool Minfinder::terminated()
{
    double eps=params["minfinder_epsilon"].toString().toDouble();

    int max_generations=params["minfinder_iterations"].toString().toInt();
    double fmin=fabs(1.0+fabs(mbesty));
    if(iteration<=1){x1=0.0;x2=0.0;}
    x1=x1+fmin;
    x2=x2+fmin * fmin;


    variance = x2/(iteration+1) -(x1/(iteration+1))*(x1/(iteration+1));
    variance=fabs(variance);

    if(mbesty<oldBesty)
    {
        oldBesty=mbesty;
        stopat=variance/2.0;
    }

    if(stopat<eps && iteration>=20) return true;
    printf("Iteration %d  Best value: %lf variance: %lf stopat: %lf\n",
           iteration,mbesty,variance,stopat);

    return iteration>=max_generations || (variance<=stopat && iteration>=20);
}

double Minfinder::mdelta(double a,double r0,double r)
{
        double b=a/r0;
        if(r>=0 && r<=r0) return a-b*r;
        return 0.0;
}

Data  Minfinder::getSample()
{
    QString sampling=params["minfinder_sampling"].toString();

    Data tx;
    tx.resize(myProblem->getDimension());

    if(sampling=="uniform")
        return myProblem->getRandomPoint();
    else
    if(sampling=="repulsion")
    {
        const double alpha=0.1;
        const double rho0=0.25;
        tx=myProblem->getRandomPoint();
        for(int j=0;j<tx.size();j++)
        {
           double y=tx[j];
           for(int k=0;k<samplex.size();k++)
           {
              Data z=tx;
              z=samplex[k];
              for(int l=0;l<z.size();l++)
              {
                  double r=getDistance(tx,z);
                  double u=y+mdelta(alpha,rho0,r)*(y-z[l])/r;
                  tx[j]=u;
               }
            }

         }
    }
    else
    if(sampling=="awayminima")
    {
        bool redo=false;
        int fail_count=0;
        do
        {
            tx=myProblem->getRandomPoint();
            redo=checkForGradientCriterion(tx);
            if(redo) fail_count++;
            if(fail_count==10) break;
            if(!redo) break;
            redo=false;
        }while(true);

    }
    return tx;
}
void Minfinder::step()
{
    ++iteration;
    double eps=params["minfinder_epsilon"].toString().toDouble();
    int samples=params["minfinder_samples"].toString().toInt();
    samplex.clear();
    for(int i=0;i<samples;i++)
    {
        Data tx=getSample();
        samplex.push_back(tx);
    }
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<samples;i++)
    {
        Data tx=samplex[i];
        if(!checkForGradientCriterion(tx))
        {
            Data txold=tx;
            Tolmin mTolmin(myProblem);
            double y=mTolmin.Solve(tx);
            RC+=getDistance(txold,tx);
            localSearchCount++;
#pragma omp critical
{
            bool found=false;
            for(int j=0;j<minimax.size();j++)
            {
                if(getDistance(minimax[j],tx)<eps) {found=true;break;}
            }
            if(!found)
            {
                minimax.push_back(tx);
                minimay.push_back(y);
                if(y<mbesty)
                {
                    mbesty=y;
                    bestIndex=minimax.size()-1;
                }
            }
}
        }
    }
}

void Minfinder::init()
{
    x1=0.0;
    x2=0.0;
    iteration=0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;
    mbesty=1e+100;
    minimax.clear();
    minimay.clear();
    startx.clear();
    starty.clear();
    RC=0.0;
    localSearchCount=0;
}

void Minfinder::done()
{
    Tolmin mTolmin(myProblem);
    minimay[bestIndex]=mTolmin.Solve(minimax[bestIndex]);
}

void Minfinder::Solve()
{
    Optimizer::Solve();
}

Minfinder::~Minfinder()
{
}


extern "C" MINFINDER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Minfinder(p);
}

#include "optimizer.h"
# include <QStringList>
# include <grs.h>
# include <psolocal.h>
Optimizer::Optimizer(Problem *p)
{
    myProblem=p;
    addParameter("localsearch_method","bfgs","The desired local search method(bfgs,gradient,adam,lbfgs,grs,hill)");
    addParameter("bfgs_iterations","2001","The maximum number of iterations for BFGS");
    addParameter("bfgs_debug","no","If it is yes, then full debug info wil be displayed from BFGS");
    addParameter("gradient_rate","0.01","The learning rate for gradient descent");
    addParameter("gradient_iterations","10000","The maximum number of iterations for gradient descent");
    addParameter("adam_b1","0.9","The parameter b1 of Adam algorithm");
    addParameter("adam_b2","0.999","The parameter b2 of Adam algorithm");
    addParameter("adam_rate","0.01","The learning rate for Adam algorithm");
    addParameter("adam_iterations","10000","The maximum number of iterations for Adam algorithm");
    addParameter("psoLocal_particles","100","Number of particles for pso local");
    addParameter("psoLocal_generations","50","Maximum number of iterations for pso local");
    addParameter("psoLocal_r1","1.0","R1 param for pso local");
    addParameter("psoLocal_r2","1.0","R2 parameter for pso local");
    addParameter("psoLocal_theta","0.1","Theta percentage for pso local");
}


double  Optimizer::localSearch(Data &x)
{
    QString method=params["localsearch_method"].toString();

    if(method=="psoLocal")
    {
        int particles = params["psoLocal_particles"].toString().toInt();
        double theta = params["psoLocal_theta"].toString().toDouble();
        double r1 = params["psoLocal_r1"].toString().toDouble();
        double r2 = params["psoLocal_r2"].toString().toDouble();
        int generations = params["psoLocal_generations"].toString().toInt();
        PsoLocal pso(myProblem,x,particles,theta);
        pso.setR1(r1);
        pso.setR2(r2);
        pso.Solve(generations);
        x=pso.gestBestParticle();
        double y = myProblem->funmin(x);
        return y;

    }
    else
    if(method=="bfgs")
    {
        Tolmin mt(myProblem);
	bool debug = false;
	if(params["bfgs_debug"].toString()=="yes") debug =true;
        return mt.Solve(x,debug,params["bfgs_iterations"].toString().toInt());
    }
    else
    if(method=="gradient")
    {
        GradientDescent des(myProblem);
        des.setIters(params["gradient_iterations"].toString().toInt());
        des.setRate(params["gradient_rate"].toString().toDouble());
        return des.Solve(x);
    }
    else
    if(method=="adam")
    {
        Adam at(myProblem);
        at.setB1(params["adam_b1"].toString().toDouble());
        at.setB2(params["adam_b2"].toString().toDouble());
        at.setLearningRate(params["adam_rate"].toString().toDouble());
        at.setIterations(params["adam_iterations"].toString().toInt());
        return at.Solve(x);
    }
    else
    if(method=="lbfgs")
    {
        Lbfgs lt(myProblem);
        return lt.Solve(x);
    }
    else
    if(method=="gslbfgs")
    {
        //GSlBfgs xx(myProblem);
        //return xx.Solve(x);
    }
    else
    if(method=="grs")
    {
        Grs *solver=new Grs(myProblem);
        solver->setGenomeCount(20);
        solver->setGenomeLength(10 * myProblem->getDimension());
       double y=myProblem->funmin(x);
       solver->Solve(x,y);
       y=myProblem->funmin(x);
       delete solver;
       return y;
    }
    else
	if(method=="random")
	{
        double y = myProblem->funmin(x);
		for(int i=0;i<500;i++)
		{
			Data z = myProblem->getRandomPoint();
			double zy = myProblem->funmin(z);
			if(zy<y)
			{
			printf("random[%d]=%lf \n",i,zy);
				y = zy;
				x = z;
			}
		}
		return y;
	}	
    else
    if(method=="nelderMead")
    {
        double y = myProblem->funmin(x);
        nelderMead n(myProblem,x,y);
        n.init();
        while(!n.terminated())
        {
            n.step();
        }
        n.done();
        x = n.getBestX();
        y = n.getBestY();

    }
    else
    if(method=="hill")
    {
       Data currentPoint;
       currentPoint.resize(x.size());    // the zero-magnitude vector is common
       Data stepSize;
       stepSize.resize(x.size());
       {
        for(int i=0;i<x.size();i++) {
		stepSize[i]=1.0;
		currentPoint[i]=x[i];
	}

       }
       double acceleration=1.2;// // a value such as 1.2 is common
       Data candidate;
       candidate.resize(4);
    candidate[0] = -acceleration;
    candidate[1] = -1 / acceleration;
    candidate[2] = 1 / acceleration;
    candidate[3] = acceleration;
    double bestScore = myProblem->funmin(currentPoint);
    double oldmin= bestScore;
    const int itermax=100;
    int iter=1;
    while(true)
    {
        bool termflag=false;
        double beforeScore = bestScore;
        for(int i=0;i<currentPoint.size();i++)
        {

            double beforePoint = currentPoint[i];
            double bestStep = 0;
            for(int j=0;j<4;j++)
            {// try each of 4 candidate locations
                double step = stepSize[i] * candidate[j];
                currentPoint[i] = beforePoint + step;
                double score = myProblem->funmin(currentPoint);
                if( score < bestScore){

                    bestScore = score;
                    bestStep = step;
                }
            }
            if(fabs(bestStep)<1e-5){
                currentPoint[i] = beforePoint;
		
                stepSize[i] = stepSize[i] / acceleration;
            }
            else
            {
                currentPoint[i] = beforePoint + bestStep;
                stepSize[i] = bestStep; // acceleration
            }
        //    printf("Best score = %lf oldScore  = %lf iter =%d \n",bestScore,beforeScore,iter);
	}
        if (fabs(bestScore-beforeScore) < 1e-5 || iter>itermax)
        {
		termflag=true;
		break;
	}
            iter++;
        if(termflag) break;
    }
    //printf("Hill %lf -> %lf \n",oldmin,bestScore);
   for(int i=0;i<x.size();i++) x[i]=currentPoint[i];
    return myProblem->funmin(x);
    }
}

void Optimizer::setProblem(Problem *p)
{
    myProblem=p;
}

void Optimizer::setSettings(QJsonObject settings)
{
    QStringList keys=settings.keys();
    for(int i=0;i<keys.size();i++)
    {
        if(params.contains(keys[i]))
            params[keys[i]]=settings[keys[i]].toString();
    }
}

void Optimizer::setThreads(int t)
{
    threads=t;
}

Problem         *Optimizer::getProblem()
{
    return myProblem;
}

void Optimizer::Solve()
{
    init();
    do
    {
        step();
    }while(!terminated());
    done();
}

void          Optimizer::addParameter(QString name,QString value,QString description)
{
    params[name]=value;
    params[name+"_desc"]=description;
}

QString        Optimizer::getParameterDescription(QString name)
{
    return params[name+"_desc"].toString();
}

QString        Optimizer::getParameter(QString name)
{
    return params[name].toString();
}

QJsonObject    Optimizer::getParameters()
{
    return params;
}

bool   Optimizer::terminated()
{
    return false;
}

void  Optimizer::step()
{

}
void  Optimizer::init()
{

}

void Optimizer::done()
{

}

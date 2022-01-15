# include "grs.h"		
# include <math.h>
Grs::Grs(Problem *p)
{
	problem = p;
//#pragma omp parallel
	program = new RlsProgram(problem);
	const int def_popcount=10;
	const int def_popsize= 200;
	const double def_srate=0.1;
	const double def_mrate=0.05;
	const int    def_maxgenerations=10;
//#pragma omp parallel
	pop=new Population(def_popcount,def_popsize,program);
	pop->setSelectionRate(def_srate);
	pop->setMutationRate(def_mrate);
	pop->setElitism(0);
	maxGenerations=def_maxgenerations;
}

int	Grs::getGenomeCount() const
{
	return pop->getCount();
}

int	Grs::getGenomeLength() const
{
	return pop->getSize();
}

int	Grs::getMaxGenerations() const
{
	return maxGenerations;
}

void	Grs::setGenomeCount(int c)
{
	int l=pop->getSize();
	double s=pop->getSelectionRate();
	double m=pop->getMutationRate();
	delete pop;
//#pragma omp parallel
	pop=new Population(c,l,program);
	pop->setSelectionRate(s);
	pop->setMutationRate(m);
	pop->setElitism(0);
}

void	Grs::setGenomeLength(int l)
{
	int c=pop->getCount();
	double s=pop->getSelectionRate();
	double m=pop->getMutationRate();
	delete pop;
//#pragma omp parallel
	pop=new Population(c,l,program);
	pop->setSelectionRate(s);
	pop->setMutationRate(m);
	pop->setElitism(0);
}

void	Grs::setMaxGenerations(int g)
{
	maxGenerations = g;
}

double	Grs::getSelectionRate() const
{
	return pop->getSelectionRate();
}

double	Grs::getMutationRate() const
{
	return pop->getMutationRate();
}

void	Grs::setSelectionRate(double s)
{
	pop->setSelectionRate(s);
}

void	Grs::setMutationRate(double m)
{
	pop->setMutationRate(m);
}

int	Grs::getFunctionEvaluations()
{
    //return fevals;
    return problem->getFunctionCalls();
}

int	Grs::getGradientEvaluations()
{
    //return gevals;
    return 0;
}


void	Grs::Solve(Data &x,double &y)
{


	pop->reset();
	double *xx=new double[x.size()];
    double *xx1=new double[x.size()];
//#pragma omp parallel for
	for(int i=0;i<x.size();i++) xx[i]=x[i];
	double oldy=y;

	for(int iters=1;iters<=maxGenerations;iters++)
    {

//#pragma omp parallel
		program->setX0(xx,y);
		pop->nextGeneration();
		
		double f=pop->getBestFitness();
		y=program->getBestValue();
		program->getBestX0(xx1);	

		double diff=0.0;
		for(int i=0;i<x.size();i++)
		{

			diff+=pow(xx1[i]-xx[i],2.0);
			x[i]=xx1[i];
			xx[i]=x[i];	
		}
		if(diff<1e-5)
		{
			break;
		}

	}
	delete[] xx;
	delete[] xx1;

}

double	Grs::getMinimum() const
{
	return program->getBestValue();
}

Grs::~Grs()
{
	delete program;
	delete pop;
}

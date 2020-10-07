# include <gensolver.h>
# include <stdlib.h>
# include <string.h>
# include <neural.h>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <tolmin.h>
# include <omp.h>

# define MAXTHREADS 32

GenSolver::GenSolver(int gcount,Problem *p,double mx,int ff)
{
	small_tolmin_flag=ff;
    //srand(1);
	maxx=mx;
	problem = p;
	genome_size=p->getDimension();
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.05;
	genome_count   = gcount;
	generation     = 0;

	double f;
	genome=new double*[genome_count];
	children=new double*[genome_count];
	for(int i=0;i<genome_count;i++)
	{
		genome[i]=new double[genome_size];
		children[i]=new double[genome_size];
		for(int j=0;j<genome_size;j++)
			genome[i][j]=2.0*drand48()-1.0;
				
	}
	fitness_array=new double[genome_count];
}

void	GenSolver::reset()
{
}

static double computeFactor(double v)
{
	if(v<=0.1) return pow(2.0,1.0);
	if(v<=0.2) return pow(2.0,2.0);
	if(v<=0.3) return pow(2.0,3.0);
	if(v<=0.4) return pow(2.0,4.0);
	if(v<=0.5) return pow(2.0,5.0);
	if(v<=0.6) return pow(2.0,6.0);
	if(v<=0.7) return pow(2.0,7.0);
	if(v<=0.8) return pow(2.0,8.0);
	if(v<=0.9) return pow(2.0,9.0);
	if(v<=1.0) return pow(2.0,10.0);
}

#define VIOLATE
double 	GenSolver::fitness(vector<double> &g)
{
	/*	
//	problem->funmin(g);
//	return -((Model*)problem)->classTrainError();
#ifdef VIOLATE
	double v=problem->funmin(g);
	double violate=((Neural *)problem)->countViolate(10.0);
	double factor=10.0;//computeFactor(violate);
	v=-v*(1.0+factor * violate*violate);
	return v;	
#else
*/
	return -problem->funmin(g);
//#endif
}

void	GenSolver::select()
{
	double itemp[genome_size];
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_count-1;j++)
		{
			if(fitness_array[j+1]>fitness_array[j])
			{
				double dtemp;
				dtemp=fitness_array[j];
				fitness_array[j]=fitness_array[j+1];
				fitness_array[j+1]=dtemp;
				
				memcpy(itemp,genome[j],genome_size*sizeof(double));
				memcpy(genome[j],genome[j+1],genome_size*sizeof(double));
				memcpy(genome[j+1],itemp,genome_size*sizeof(double));
			}
		}
	}
}

void	GenSolver::crossover()
{
        int parent[2];
        int nchildren=(int)((1.0 - selection_rate) * genome_count);
	if(!(nchildren%2==0)) nchildren++;
        const int tournament_size =(genome_count<=100)?4:10;
        int count_children=0;
        while(1)
        {
                //epilogi ton goneon
                for(int i=0;i<2;i++)
                {
			
                        double max_fitness=-1e+10;
                        int    max_index=-1;
			int r;
                        for(int j=0;j<tournament_size;j++)
                        {
				r=rand() % (genome_count);
                                if(j==0 || fitness_array[r]>max_fitness)
                                {
                                        max_index=r;
                                        max_fitness=fitness_array[r];
                                }
                        }
                        parent[i]=max_index;
			
                }
		
		for(int i=0;i<genome_size;i++)
		{
			double a =drand48();
			children[count_children][i]=a*genome[parent[0]][i]+
				(1.0-a)*genome[parent[1]][i];
			children[count_children+1][i]=a*genome[parent[1]][i]+
				(1.0-a)*genome[parent[0]][i];
		}
		
		/*	
		int pt1,pt2;
		pt1=rand() % genome_size;
		memcpy(children[count_children],
				genome[parent[0]],pt1 * sizeof(double));
		memcpy(&children[count_children][pt1],
			&genome[parent[1]][pt1],(genome_size-pt1)*sizeof(double));
		memcpy(children[count_children+1],
				genome[parent[1]],pt1 * sizeof(double));
		memcpy(&children[count_children+1][pt1],
			&genome[parent[0]][pt1],(genome_size-pt1)*sizeof(double));
		*/	
		count_children+=2;
		if(count_children>=nchildren) break;
	}
	
	
	for(int i=0;i<nchildren;i++)
	{
		memcpy(genome[genome_count-i-1],
				children[i],genome_size * sizeof(double));
	}
}

void	GenSolver::setElitism(int s)
{
	elitism = s;
}

void	GenSolver::mutate()
{
	int start = elitism * (int)(genome_count*selection_rate);
	start = 1;
	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
			double r=drand48();
			if(r<mutation_rate)
			{
			double percent=exp(-generation * 1.0/2.0);
			//	percent = 0.25;
				genome[i][j]*=(1.0+2.0*drand48()*percent-percent);
	
				
			}
		}
	}
}

void	GenSolver::calcFitnessArray()
{
	vector<double> g;
	g.resize(genome_size);
	double minf=-1e+10;
#pragma omp parallel for num_threads(MAXTHREADS) schedule(static) reduction(min:minf)
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
			g[j]=genome[i][j];	
		double f=fitness(g);
		fitness_array[i]=f;
		if(f>minf) minf=f;
		if(generation%10==0 && i%50==0) 
		{
		//	printf("%d:%lf ",i,minf);
		//	fflush(stdout);
		}
		for(int j=0;j<genome_size;j++) genome[i][j]=g[j];
	}
//	if(generation%10==0)
//	printf("\n");
}

int	GenSolver::getGeneration() const
{
	return generation;
}

int	GenSolver::getCount() const
{
	return genome_count;
}

int	GenSolver::getSize() const
{
	return genome_size;
}

void	GenSolver::nextGeneration()
{
	if(generation)
	mutate();
	calcFitnessArray();
	select();
	crossover();
	/*
	if((generation+1) % 10==0) 
	{
		for(int  i=0;i<20;i++)
			localSearch(rand() % genome_count);
		select();
	}
	*/	
	++generation;
}

void	GenSolver::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

void	GenSolver::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

double	GenSolver::getSelectionRate() const
{
	return selection_rate;
}

double	GenSolver::getMutationRate() const
{
	return mutation_rate;
}

double	GenSolver::getBestFitness() const
{
	return  fitness_array[0];
}

void	GenSolver::localSearch(int gpos)
{
	vector<double> g;
	g.resize(genome_size);
	int pos=gpos;
	for(int iters=1;iters<=100;iters++)
	{
		int randgenome=rand() % genome_count;
		int randpos=rand() % genome_size;
		for(int i=0;i<randpos;i++) g[i]=genome[pos][i];
		for(int i=randpos;i<genome_size;i++) g[i]=genome[randgenome][i];
		double f=fitness(g);
		if(fabs(f)<fabs(fitness_array[pos]))
		{
			for(int i=0;i<genome_size;i++) genome[pos][i]=g[i];
			fitness_array[pos]=f;
			printf("New Min %20.10lf\n",f);
		}
		else
		{
			for(int i=0;i<randpos;i++) g[i]=genome[randgenome][i];
			for(int i=randpos;i<genome_size;i++) g[i]=genome[pos][i];
			f=fitness(g);
			if(fabs(f)<fabs(fitness_array[pos]))
			{
				for(int i=0;i<genome_size;i++) genome[pos][i]=g[i];
				fitness_array[pos]=f;
				printf("New Min %20.10lf\n",f);
			}
		}
	}
	return;
}
vector<double> GenSolver::getBestGenome() const
{
	vector<double> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return g;
}
double	GenSolver::evaluateBestFitness() 
{
	vector<double> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];	
	return fitness(g);
}

void	GenSolver::setBest(double *x,double y)
{
	for(int i=0;i<genome_size;i++)
		genome[0][i]=x[i];
	fitness_array[0]=y;
}

void	GenSolver::local()
{
	vector<double> g;
	g.resize(genome_size);
	Neural *nn=(Neural *)problem;
	nn->setWeights(g);
	for(int i=0;i<g.size();i++) g[i]=genome[0][i];
	MinInfo Info;
	Info.p=problem;
	Info.iters=21;
	double v=fitness_array[0];
	v=tolmin(g,Info);
	for(int j=0;j<g.size();j++) genome[0][j]=g[j];
	fitness_array[0]=-v;
}

GenSolver::~GenSolver()
{
	for(int i=0;i<genome_count;i++)
	{
		delete[] children[i];
		delete[] genome[i];
	}
	delete[] genome;
	delete[] children;
	delete[] fitness_array;
}


void	GenSolve(Problem *p,Matrix &x,double &y,double mx,int flag,int gcount)
{
	const int genome_count =gcount;
	const int max_generations =10;
	GenSolver pop(genome_count,p,mx,flag);
	Neural *nn=(Neural *)p;
	vector<double> g;
	g.resize(x.size());
	double oldBest=-1e+100;
	double *xx=new double[x.size()];
	double x1=0,x2=0;
	double stopat=-1e+100;
	for(int i=0;i<max_generations;i++)
	{
		pop.nextGeneration();
		g=pop.getBestGenome();
		if(fabs(pop.getBestFitness())<1e-7) break;
		pop.evaluateBestFitness();
		double violate = nn->countViolate(20.0);
		x1+=fabs(pop.getBestFitness()-oldBest);
		x2+=fabs(pop.getBestFitness()-oldBest)*fabs(pop.getBestFitness()-oldBest);
		double variance=x2/(i+1)-x1/(i+1)*x1/(i+1);
		if(i>=10 && variance<=stopat) break;
	//	if(i%5==0)
		//	pop.local();
		if(pop.getBestFitness()>oldBest)
		{
			stopat=variance/2.0;
			oldBest=pop.getBestFitness();
		}
	}
	for(int i=0;i<g.size();i++) x[i]=g[i];
	y=-pop.getBestFitness();
	delete[] xx;	
}

void	GenSolve(Problem *p,Matrix &x,double &y,double mx,int flag)
{
	const int genome_count =200;
	const int max_generations =100;
	GenSolver pop(genome_count,p,mx,flag);
	Neural *nn=(Neural *)p;
	vector<double> g;
	g.resize(x.size());
	double oldBest=-1e+100;
	double *xx=new double[x.size()];
	double x1=0,x2=0;
	double stopat=-1e+100;
	for(int i=0;i<max_generations;i++)
	{
		pop.nextGeneration();
		g=pop.getBestGenome();
		if(fabs(pop.getBestFitness())<1e-7) break;
		pop.evaluateBestFitness();
		x1+=fabs(pop.getBestFitness()-oldBest);
		x2+=fabs(pop.getBestFitness()-oldBest)*fabs(pop.getBestFitness()-oldBest);
		double variance=x2/(i+1)-x1/(i+1)*x1/(i+1);
		if(i>=10 && variance<=stopat) break;
		//if(i%1==0 &&i)
		//	pop.local();
		if(pop.getBestFitness()>oldBest)
		{
			stopat=variance/2.0;
			oldBest=pop.getBestFitness();
        }
	}
	for(int i=0;i<g.size();i++) x[i]=g[i];
	y=-pop.getBestFitness();
	delete[] xx;	
}

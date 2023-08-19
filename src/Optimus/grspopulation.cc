# include <grspopulation.h>	
# include <iostream>
# include <stdlib.h>
# include <string.h>
#include <random>
# define MAX_RULE	256


GrsPopulation::GrsPopulation(int gcount,int gsize,Program *p)
{
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.1;
	genome_count   = gcount;
	genome_size    = gsize;
	generation     = 0;
	program        = p;

    rng=new std::mt19937(0);
    dist=new  std::uniform_int_distribution<std::mt19937::result_type> (0,RAND_MAX);

	genome=new int*[genome_count];
	children=new int*[genome_count];
	for(int i=0;i<genome_count;i++)
	{
		genome[i]=new int[genome_size];
		children[i]=new int[genome_size];
			for(int j=0;j<genome_size;j++)
                genome[i][j]=randint()%MAX_RULE;
	}
	fitness_array=new double[genome_count];
}

int     GrsPopulation::randint()
{
    return (*dist)(*rng);
}
void	GrsPopulation::reset()
{
	generation = 0;
	int i,j;
	for( i=0;i<genome_count;i++)
		for( j=0;j<genome_size;j++)
                genome[i][j]=randint()%MAX_RULE;
	for( i=0;i<genome_count;i++)
			fitness_array[i]=-1e+100;
}

double 	GrsPopulation::fitness(vector<int> &g)
{
	return program->fitness(g);
}

void	GrsPopulation::select()
{
	int itemp[genome_size];
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
				
				memcpy(itemp,genome[j],genome_size*sizeof(int));
				memcpy(genome[j],genome[j+1],genome_size*sizeof(int));
				memcpy(genome[j+1],itemp,genome_size*sizeof(int));
			}
		}
	}
}

void	GrsPopulation::crossover()
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
			
                        double max_fitness=-1e+100;
                        int    max_index=-1;
			int r;
                        for(int j=0;j<tournament_size;j++)
                        {
                r=randint() % (genome_count);
                                if(j==0 || fitness_array[r]>max_fitness)
                                {
                                        max_index=r;
                                        max_fitness=fitness_array[r];
                                }
                        }
                        parent[i]=max_index;
			
                }
		int pt1,pt2;
		/**/
		//if(fitness_array[parent[0]]==fitness_array[parent[1]] ) continue;
		/**/
        pt1=randint() % genome_size;
		memcpy(children[count_children],
				genome[parent[0]],pt1 * sizeof(int));
		memcpy(&children[count_children][pt1],
			&genome[parent[1]][pt1],(genome_size-pt1)*sizeof(int));
		memcpy(children[count_children+1],
				genome[parent[1]],pt1 * sizeof(int));
		memcpy(&children[count_children+1][pt1],
			&genome[parent[0]][pt1],(genome_size-pt1)*sizeof(int));
		count_children+=2;
		if(count_children>=nchildren) break;
	}
	
	
	for(int i=0;i<nchildren;i++)
	{
		memcpy(genome[genome_count-i-1],
				children[i],genome_size * sizeof(int));
	}
}

void	GrsPopulation::setElitism(int s)
{
	elitism = s;
}

void	GrsPopulation::mutate()
{
	int start = elitism * (int)(genome_count*selection_rate);
	start = elitism;
	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
            double r=randint()*1.0/RAND_MAX;
			if(r<mutation_rate)
			{
                genome[i][j]=randint() % MAX_RULE;
			}
		}
	}
}

void	GrsPopulation::calcFitnessArray()
{
	vector<int> g;
	g.resize(genome_size);

	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++) g[j]=genome[i][j];	
		double f=fitness(g);
		fitness_array[i]=f;
	}
}

int	GrsPopulation::getGeneration() const
{
	return generation;
}

int	GrsPopulation::getCount() const
{
	return genome_count;
}

int	GrsPopulation::getSize() const
{
	return genome_size;
}

void	GrsPopulation::nextGeneration()
{
	if(generation)
	mutate();
	calcFitnessArray();
	select();
	crossover();
	++generation;
}

void	GrsPopulation::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

void	GrsPopulation::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

double	GrsPopulation::getSelectionRate() const
{
	return selection_rate;
}

double	GrsPopulation::getMutationRate() const
{
	return mutation_rate;
}

double	GrsPopulation::getBestFitness() const
{
	return  fitness_array[0];
}

vector<int> GrsPopulation::getBestGenome() const
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return g;
}
double	GrsPopulation::evaluateBestFitness() 
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];	
	return fitness(g);
}

GrsPopulation::~GrsPopulation()
{
    delete rng;
    delete dist;
	for(int i=0;i<genome_count;i++)
	{
		delete[] children[i];
		delete[] genome[i];
	}
	delete[] genome;
	delete[] children;
	delete[] fitness_array;
}

# include <multi_population.h>	
# include <iostream>
# include <math.h>

# define MAX_RULE	256

static int myrand()
{
	return  rand();
}

MultiPopulation::MultiPopulation(int gcount,int gsize,int d,Program *p,Data &ix)
{
    elitism=1;
    initialx=ix;
	dimension      = d;
	selection_rate = 0.1;
	mutation_rate  = 0.1;
	genome_count   = gcount;
	genome_size    = gsize;
	generation     = 0;
	program        = p;

	double f;
	genome=new int*[dimension * genome_count];
	children=new int*[dimension * genome_count];
	for(int i=0;i<dimension * genome_count;i++)
	{
		genome[i]=new int[genome_size];
		children[i]=new int[genome_size];
		for(int j=0;j<genome_size;j++)
		{
			//genome[i][j]=rand()%MAX_RULE;
			genome[i][j]=myrand()%MAX_RULE;
		}
	}
	fitness_array=new double[dimension * genome_count];
	g.resize(genome_size*dimension);
}

void	MultiPopulation::reset()
{
	generation = 0;
	for(int i=0;i<dimension * genome_count;i++)
		for(int j=0;j<genome_size;j++)
				genome[i][j]=rand()%MAX_RULE;
	for(int i=0;i<genome_count;i++)
			fitness_array[i]=-1e+8;
}

double 	MultiPopulation::fitness(vector<int> &g)
{
  //  return program->fitness(g);
    return program->fitness(initialx,g);
}

double	MultiPopulation::totalfitness(int index)
{
	double s=0.0;
	for(int i=0;i<dimension;i++)
		s+=fitness_array[i*genome_count+index];
	return s;
}

void	MultiPopulation::select(int d)
{
	int s=d*genome_count;
	int *itemp=new int[genome_size];
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_count-1;j++)
		{
		if(fitness_array[s+j+1]>fitness_array[s+j])
		{
		double dtemp;
		dtemp=fitness_array[s+j];
		fitness_array[s+j]=fitness_array[s+j+1];
		fitness_array[s+j+1]=dtemp;
		memcpy(itemp,genome[s+j],genome_size*sizeof(int));
		memcpy(genome[s+j],genome[s+j+1],genome_size*sizeof(int));
		memcpy(genome[s+j+1],itemp,genome_size*sizeof(int));
		}
		}
	}
	delete[] itemp;
}

void	MultiPopulation::crossover(int d)
{
	int s=d*genome_count;
        int parent[2];
        int nchildren=(int)((1.0 - selection_rate) * genome_count);
	if(!(nchildren%2==0)) nchildren++;
        const int tournament_size =(genome_count<=100)?4:10;
        int count_children=0;
        while(1)
        {
                for(int i=0;i<2;i++)
                {
			
                        double max_fitness=-1e+10;
                        int    max_index=-1;
			int r;
                        for(int j=0;j<tournament_size;j++)
                        {
				//r=rand() % (genome_count);
				r=myrand() % genome_count;
                                if(j==0 || fitness_array[s+r]>max_fitness)
                                {
                                        max_index=s+r;
                                        max_fitness=fitness_array[s+r];
                                }
                        }
                        parent[i]=max_index;
                }
		int pt1,pt2;

		pt1=myrand() % genome_size;
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
		memcpy(genome[s+genome_count-i-1],
				children[i],genome_size * sizeof(int));
	}
}

void	MultiPopulation::setElitism(int s)
{
	elitism = s;
}

void	MultiPopulation::mutate(int d)
{
	int s=d*genome_count;
	int start = elitism * (int)(genome_count*selection_rate);
	start = 1;
	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
			double r=drand48();
			if(r<mutation_rate)
				genome[s+i][j]=myrand() % MAX_RULE;
		}
	}
}

int	MultiPopulation::valid(int index)
{
	if(fitness_array[index]<=-1e+8)  
	{
	return 0;
	}
	return 1;
}

void	MultiPopulation::calcFitnessArray()
{

	for(int i=0;i<genome_count;i++)
	{
		for(int d=0;d<dimension;d++)
		for(int j=0;j<genome_size;j++) 
			g[d*genome_size+j]=genome[d*genome_count+i][j];	
		double f=fitness(g);
		for(int d=0;d<dimension;d++) 
			fitness_array[d*genome_count+i]=f;
	}
}

int	MultiPopulation::getGeneration() const
{
	return generation;
}

int	MultiPopulation::getCount() const
{
	return genome_count;
}

int	MultiPopulation::getSize() const
{
	return genome_size;
}

void	MultiPopulation::nextGeneration()
{
	if(generation)
	{
		for(int d=0;d<dimension;d++) mutate(d);
	}
	calcFitnessArray();
	for(int d=0;d<dimension;d++)
	{
		select(d);
		crossover(d);
	}
	++generation;
}

void	MultiPopulation::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

void	MultiPopulation::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

double	MultiPopulation::getSelectionRate() const
{
	return selection_rate;
}

double	MultiPopulation::getMutationRate() const
{
	return mutation_rate;
}

double	MultiPopulation::getBestFitness() const
{
	return  fitness_array[0];
}

vector<int> MultiPopulation::getBestGenome()
{
	for(int d=0;d<dimension;d++)
	for(int i=0;i<genome_size;i++) g[d*genome_size+i]=genome[0+d*genome_count][i];
	return g;
}
double	MultiPopulation::evaluateBestFitness() 
{
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];	
	return fitness(g);
}

MultiPopulation::~MultiPopulation()
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

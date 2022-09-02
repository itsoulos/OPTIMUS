# ifndef __GRSPOPULATION__H
# include <program.h>
#include <random>

class GrsPopulation
{
	private:
		int	**children;
		int	**genome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		Program	*program;

		double 	fitness(vector<int> &g);
		void	select();
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;

        std::mt19937 *rng;
        std::uniform_int_distribution<std::mt19937::result_type> *dist;
        int randint();
    public:
		GrsPopulation(int gcount,int gsize,Program *p);
		void	setElitism(int s);
		int	getGeneration() const;
		int	getCount() const;
		int	getSize() const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
		vector<int> getBestGenome() const;
		void	reset();
		~GrsPopulation();
		
};
# define __GRSPOPULATION__H
# endif

# ifndef __MULTI_POPULATION__H
# include <program.h>
# include <Optimus/problem.h>
class MultiPopulation
{
	private:
		vector<int> g;
        Data initialx;
		int	**children;
		int	**genome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		int	dimension;
		Program* program;

		double 	fitness(vector<int> &g);
		void	select(int d);
		void	crossover(int d);
		void	mutate(int d);
		void	calcFitnessArray();
		int	elitism;
	public:
        MultiPopulation(int gcount,int gsize,int d,Program *p,Data &ix);
		int valid(int index);
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
		double	totalfitness(int index);
		vector<int> getBestGenome();
		void	reset();
		~MultiPopulation();
		
};
# define __POPULATION__H
# endif

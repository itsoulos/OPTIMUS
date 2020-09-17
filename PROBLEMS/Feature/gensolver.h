# ifndef __POPULATION__H
# include <problem.h>
# include <vector>
using namespace std;
class GenSolver
{
	private:
		double	**children;
		double	**genome;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		double  maxx;

		double 	fitness(vector<double> &g);
		void	select();
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
		Problem	*problem;
		int	small_tolmin_flag;
	public:
		void	local();
		GenSolver(int gcount,Problem *p,double mx,int f);
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
		vector<double> getBestGenome() const;
		void	reset();
		void	setBest(double *x,double y);
		void	localSearch(int pos);
		~GenSolver();
		
};
extern void GenSolve(Problem *p,Matrix &x,double &y,double mx,int f);
extern void GenSolve(Problem *p,Matrix &x,double &y,double mx,int f,int gcount);
extern void GenSolve2(Problem *p,Matrix &x,double &y,double mx,int f);
# define __POPULATION__H
# endif

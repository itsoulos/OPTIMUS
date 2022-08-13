# ifndef __POPULATION__H
# include <vector>
# include <neuralprogram.h>
using namespace std;
typedef vector<double> Matrix;
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
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
		NeuralProgram	*problem;
		int	small_tolmin_flag;
		Data lmargin,rmargin;
	public:
		GenSolver(int gcount,NeuralProgram *p,double mx,int f);
		GenSolver(int gcount,NeuralProgram *p,vector<double> &xx);
		void	select();
		void	local();
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
extern void GenSolve(NeuralProgram *p,vector<double> &x,double &y,double mx,int f);
extern void GenSolve2(NeuralProgram *p,vector<double> &x,double &y,double mx,int f);
# define __POPULATION__H
# endif

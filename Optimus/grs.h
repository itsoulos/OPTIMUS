# ifndef __GRS__H
# define __GRS__H

# include <rlsprogram.h>
# include <population.h>
# include <collection.h>


class Grs
{
	private:
		int maxGenerations;
		Problem *problem;
		RlsProgram *program;
		Population *pop;
	public:
		Grs(Problem *p);
		int	getGenomeCount() const;
		int	getGenomeLength() const;
		int	getMaxGenerations() const;
		void	setGenomeCount(int c);
		void	setGenomeLength(int l);
		void	setMaxGenerations(int g);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		void	setSelectionRate(double s);
		void	setMutationRate(double m);
		int	getFunctionEvaluations();
		int	getGradientEvaluations();
		void	Solve(Data &x,double &y);
		double	getMinimum() const;
		~Grs();
};
# endif

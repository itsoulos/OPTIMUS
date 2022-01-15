# ifndef __GENPRICE__H
# define __GENPRICE__H

# include <problem.h>
# include <collection.h>
# include <vector>
using namespace std;

class GenPrice
{
	private:
		Problem *problem;
		Data xmax,xmin,center;
		double fmax,fmin;
		int M;
		Collection *sample;
		Collection *sample2;
		double x1, x2, stopat, variance, oldBesty;
		int iters;
		int miters;
		int countEnter;
		int countFail;
	public:
		GenPrice(Problem *p);
		void   makeSample2();
		Data getNewPoint();
		void	Solve();
		void	getMinimum(Data &x,double &y);
		int	getM() const;
		void	setM(int m);
		bool    check();
		double	getFailPercent() const;
		~GenPrice();
};
# endif

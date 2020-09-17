# ifndef __PROBLEM__H
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <vector>
using namespace std;
typedef vector<double> Matrix;
typedef vector<double> Data;
class Problem;
typedef double (Problem::*FUNCTION)(Matrix);
typedef void   (Problem::*GRADIENT)(Matrix,Matrix &);


# define MARGINS_NONE	0
# define MARGINS_LEFT	1
# define MARGINS_RIGHT	2

class Problem
{
	protected:
		int      dimension;
		int	 has_margins;
		Matrix 	 lmargin;
		Matrix 	 rmargin;
	public:
		Problem(int d);
		Problem();
		void	setDimension(int d);
		void	setMargin(Matrix x1,Matrix x2);
		void	setLeftMargin(Matrix x1);
		void	setRightMargin(Matrix x2);
		virtual double  funmin(Matrix);
		virtual void   granal(Matrix x,Matrix &g);
		
		int		getDimension() const;
		void		getLeftMargin(Matrix &x1);
		void		getRightMargin(Matrix &x2);
		FUNCTION	getFunmin();
		GRADIENT	getGranal();
		void		getSample(Matrix &x);
		void		getSample(double *x);
		void		getSample(Matrix &x1,double rc,Matrix &x);
		int		isPointIn(Matrix x);
		int		hasMargins();	
		double		getLsStep(Matrix x,Matrix p);
		void		bound(Matrix &x);
		double		getGRMS(Matrix x,Matrix g);
		void		plot();
		void		plot(char *filename);
		double		getVolume();
		double		getLogVolume();
		double		getGradientCriterion(Matrix x1,Matrix x2);
		~Problem();
		
};

typedef struct MinInfo
{
	Problem *p;
	int	fevals;
	int	gevals;
	int	iters;
};

extern "C"{
extern double small_tolmin(Matrix &,MinInfo&);
}
# define __PROBLEM__H
# endif

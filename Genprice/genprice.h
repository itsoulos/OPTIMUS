# ifndef __GENPRICE__H
# define __GENPRICE__H

# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <collection.h>
# include "grs.h"
# include <vector>
# include "Genprice_global.h"
using namespace std;


class GenpriceInterface
{
public:
    virtual ~GenpriceInterface()
    {

    }
};
class GenPrice :public Optimizer, GenpriceInterface
{
	private:
        Collection *sample2;
        int iters;
		Data xmax,xmin,center;
		double fmax,fmin;
        Grs *Solver;
		int M;
        vector<int> index;
        Data xk;
	double x1,x2;
	double olddiff;
	double stopat,variance;
        int posmin,posmax;
        int dimension;
        int success=0;

		Collection *sample;
        virtual bool terminated();
        virtual void step();
        virtual void init();
        virtual void done();
	public:
		GenPrice(Problem *p);
		void	getMinimum(Data &x,double &y);
		int	getM() const;
		void	setM(int m);
        virtual ~GenPrice();
};

extern "C" GENPRICE_EXPORT Optimizer *createOptimizer(Problem *p);


# endif

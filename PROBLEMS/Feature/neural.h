# ifndef __NEURAL__H
# define __NEURAL__H
# include <model.h>
class Neural :public Model
{
	public:
		Neural(Mapper *m);
		double	countViolate(double limit);
		virtual double train1();
		virtual double train2();
		virtual double output(Matrix x);
		virtual void   getDeriv(Matrix x,Matrix &g);
		void	setWeights(Matrix x);
		~Neural();
};

# endif

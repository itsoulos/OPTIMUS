# ifndef __FC_RBF__H
# define __FC_RBF__H
# include <model.h>
# include <Rbf.h>
class Rbf :public Model
{
	private:
	        double *input,*centers,*variances,*weights;
	public:
		Rbf(Mapper *m);
		virtual double train1();
		virtual double train2();
		virtual double output(Matrix x);
		virtual void   getDeriv(Matrix x,Matrix &g);
		~Rbf();
};

# endif

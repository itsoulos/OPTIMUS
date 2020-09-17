# ifndef __MODEL__H
# define __MODEL__H

# include <problem.h>
# include <mapper.h>
# include "grand.h"
class Model :public Problem
{
	protected:
		int    isvalidation;
		Matrix 	weight;
		int	num_weights;
		int	pattern_dimension;
		int	original_dimension;
		vector<Matrix> 	origx;
		Matrix	  	origy;
		vector<Matrix> 	xpoint;
		Matrix		ypoint;
		Matrix		originalTesty;
		Matrix		originalResult;
		GRand 		*rnd;
	public:

		Mapper	*mapper;
		Model(Mapper *m);
		void	setPatternDimension(int d);
		void	setNumOfWeights(int w);
		void	setRand(GRand *r);
        void    readPatterns(vector<Matrix> &xdata,Matrix &ydata);
		void 	readPatterns(char *filename);
		void	replacePattern(int pos,Matrix x,double y);
		int	getPatternDimension() const;
		int	getOriginalDimension() const;
		int	getNumOfWeights() const;
		int	getNumPatterns() const;
		/*	BASIKH SHMEIOSI
		 *	train1: Kaleitai gia tin ekpaideysi toy genetikou.
		 *	train2: Kaleitai otan teleiosei i parapano ekpaideysi.
		 * */
		virtual	double 	train1()=0;
		virtual double	train2()=0;
		virtual double	output(Matrix x)=0;
		virtual void	getDeriv(Matrix x,Matrix &g)=0;
		
		virtual double	funmin(Matrix x);
		virtual void    granal(Matrix x,Matrix &g);
		void	transform(Matrix x,Matrix &xx);
		double  valError();
		void	enableValidation();
		double	testError(char *filename);
        double	testError(vector<Matrix> &xdata,Matrix &ydata);
		double	classTestError(char *filename,double &precision,double &recall);
        double	classTestError(vector<Matrix> &xdata,Matrix &ydata);
		double	classTrainError();
		void	print(char *train,char *itest,char *otest);

		void	randomizeWeights();
		void	getErrorMatrix(Matrix &original,Matrix &result);
		~Model();
};

# endif

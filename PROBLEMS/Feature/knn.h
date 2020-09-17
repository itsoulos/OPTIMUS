# ifndef __KNN__H
# define __KNN__H
# include <model.h>
typedef vector<int> IntVector;
class KNN :public Model
{
	private:
		void	sortArray(Matrix &x,vector<int> &index);
	public:
		KNN(Mapper *m);
		void	makeDistance(vector<Matrix> &testx,vector<Matrix> &distance);
		void	loadTest(char *filename,vector<Matrix> &testx,Matrix &testy);
		virtual double train1();
		virtual double train2();
		virtual double output(Matrix x);
		virtual void   getDeriv(Matrix x,Matrix &g);
		double	KNNtestError(vector<Matrix> &testx,Matrix &testy,vector<Matrix> &distance);
		~KNN();
};

# endif

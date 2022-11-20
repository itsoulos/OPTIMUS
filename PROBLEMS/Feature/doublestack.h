# ifndef __DOUBLESTACK__H
namespace FeatureProblem
{
class DoubleStack
{
	private:
		double *data;
		int counter;
	public:
		DoubleStack();
		int  size() const;
		void push(double x);
		double  top() const;
		double pop();
		void	clear();
		~DoubleStack();
};
}
# define __DOUBLESTACK__H
# endif

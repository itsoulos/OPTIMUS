# ifndef __DOUBLESTACK__H

/**
 * @brief The DoubleStack class implements a stack of double numbers.
 */
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

# define __DOUBLESTACK__H
# endif

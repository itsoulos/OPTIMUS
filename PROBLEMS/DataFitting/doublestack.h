# ifndef __DOUBLESTACK__H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

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

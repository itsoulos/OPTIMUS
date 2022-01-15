# ifndef __RLSPROGRAM__H
# define __RLSPROGRAM__H
# include <Optimus/optimizer.h>

# include <grs/population.h>
# include <grs/fparser.hh>

class RlsProgram :public Program
{
	private:
		FunctionParser	parser;
        Problem *problem;
		double	bestValue;
		double  fvalue;
		int dimension;
		double *x0;
		double *bestx0;
		Symbol Expr, Delim, BinaryOp, Function, 
			Add, Sub, Mult, Div,Lpar,Rpar,
			Sin, Cos, Exp, Log,X,Dot, 
			Terminal, Digit0, Digit1, DigitList;
		vector<Symbol> Digit;
		vector<Rule*> rule;
		void	makeTerminals();
		void	makeNonTerminals();
		void	makeRules();
		int	newRule();
	public:
		RlsProgram(Problem *P);
		void	setX0(double *X0,double f);
		void	getX0(double *X0);
		virtual double fitness(vector<int> &genome);
		double	getValue() const;
		double  getBestValue() const;
		void	getBestX0(double *X0);
		string printProgram(vector<int> &genome);
		void	clear();
		~RlsProgram();
};
# endif


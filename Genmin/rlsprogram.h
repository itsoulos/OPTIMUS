# ifndef __RLSPROGRAM__H
# define __RLSPROGRAM__H
# include <Optimus/problem.h>
# include "symbol.h"
# include "program.h"
class RlsProgram :public Program
{
	private:
		Problem *problem;
		int dimension;
		Symbol Minus,Expr, Delim,Dot, Digit0, DigitList;
		vector<Symbol> Digit;
		vector<Rule*> rule;
		void	makeTerminals();
		void	makeNonTerminals();
		void	makeRules();
		int	newRule();
        Data lmargin,rmargin;

	public:
		RlsProgram(Problem *P);
		virtual double fitness(vector<int> &genome);
        virtual double fitness(vector<double> &,vector<int> &genome);
		void    getX(vector<int> &genome,Data &x);
        void    getX(vector<double> &ix,vector<int> &genome,Data &x);

		string printProgram(vector<int> &genome);
		~RlsProgram();
};
# endif


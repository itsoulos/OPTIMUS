# ifndef __CPROGRAM__H
# include <symbol.h>
# include <rule.h>
# include <fparser.hh>

class Cprogram
{
	protected:
		string		vars;
		FunctionParser 	parser;
		int		dimension,pdimension;
		vector<Rule*> rule;

		Symbol		maxx,minx,meanx,stdx,xfunction,dnumber,Sig,sx;

		Symbol		Start, Expr, If,function, binaryop, terminal,
					XXlist,DigitList, Digit0, Digit1, 
				MinMax,Sin, Cos, Exp, Log, Abs, Sqrt,Avg,And,Or,Eq,Neq,Not,
				Min, PI, Max,Delim,Mod;
		Symbol		relop,boolExpr,Gt,Lt,Ge,Le;
		Symbol		Plus, Minus, Mult, Div, Pow;
		Symbol		Lpar, Rpar, Dot, Comma;
		Symbol		Tan, Int, Log10;
		Symbol		Mmax,Mmin,Msum,Mavg,Mnorm,Mexpr;
		Symbol		binexpr,funexpr;
		vector<Symbol>	Digit;
		vector<Symbol>	XX;
		int			newRule();
		void			makeTerminals();
		void			makeNonTerminals();
		void			makeRules();
	public:
		Cprogram(int dim,int pdim);
		int	Parse(string expr);
		double	Eval(const double *X);
		int	EvalError();
		Symbol	*getStartSymbol();
		~Cprogram();
};
# define __CPROGRAM__H
# endif

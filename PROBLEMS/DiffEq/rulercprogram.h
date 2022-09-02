#ifndef RULERCPROGRAM_H
#define RULERCPROGRAM_H

# include <symbol.h>
# include <derule.h>
# include <fparser.hh>

class RulerCprogram
{
    protected:
        string		vars;
        FunctionParser 	parser;
        int			dimension;
        vector<DeRule*> rule;
        Symbol		Start, Expr, function, binaryop, terminal,comma,boolexpr,ifexpr2,
                    XXlist,DigitList, Digit0, Digit1, MinMax, relop,boolop,ifexpr;
        Symbol		Sin, Cos, Exp, Log, Abs, Sqrt, Min, PI, Max,separator,cMyIf,cMyElse;
        Symbol		Plus, Minus, Mult, Div, Pow;
        Symbol		Lpar, Rpar, Dot, Comma;
        Symbol		Tan, Int, Log10;
        Symbol      cAnd,cOr,cEq,cNeq;
        Symbol      cGt,cGe,cLt,cLe,cNot,cIf;
        vector<Symbol>	Digit;
        vector<Symbol>	XX;
        int			newRule();
        void		makeTerminals();
        void		makeNonTerminals();
        void		makeRules();
    public:
        RulerCprogram(int dim);
        int	Parse(string expr);
        double	Eval(const double *X);
        int	EvalError();
        Symbol	*getStartSymbol();
        ~RulerCprogram();
};
# endif

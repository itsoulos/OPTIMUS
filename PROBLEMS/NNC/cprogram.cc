# include <cprogram.h>
# include <stdio.h>
# include <math.h>

int problemDimension;
Cprogram::Cprogram(int dim)
{
	parser.AddConstant("pi",M_PI);
	problemDimension = dimension;
	dimension = dim;
	makeTerminals();
	makeNonTerminals();
	makeRules();
}

int	Cprogram::newRule()
{
	int r;
	r=rule.size();
	rule.resize(r+1);
	rule[r]=new Rule();
	return r;
}

void	Cprogram::makeTerminals()
{
	PI.set("pi",1);
	Plus.set("+",1);
	Minus.set("-",1);
	Mult.set("*",1);
	Div.set("/",1);
	Pow.set("^",1);
	Comma.set(",",1);
	Dot.set(".",1);
	Lpar.set("(",1);
	Rpar.set(")",1);
	Sin.set("sin",1);
	Cos.set("cos",1);
	Exp.set("exp",1);
	Log.set("log",1);
	Abs.set("abs",1);
	Sqrt.set("sqrt",1);
	Tan.set("tan",1);
	Int.set("int",1);
	Log10.set("log10",1);
	Min.set("min",1);
	Max.set("max",1);
	XX.resize(dimension);
	vars="";
	for(int i=0;i<dimension;i++)
	{
		char str[100];
		sprintf(str,"x%d",i+1);
		XX[i].set(str,1);
		vars=vars+str;
		if(i<dimension-1) vars=vars+",";
	}
	Digit.resize(10);
	for(int i=0;i<10;i++)
	{
		char str[100];
		sprintf(str,"%d",i);
		Digit[i].set(str,1);
	}
}

void	Cprogram::makeNonTerminals()
{
	Start.set("START",0);
	DigitList.set("DIGITLIST",0);
	Digit0.set("DIGIT0",0);
	Digit1.set("DIGIT1",0);
	XXlist.set("XXLIST",0);
	Expr.set("EXPR",0);
	function.set("FUNCTION",0);
	binaryop.set("BINARYOP",0);
	terminal.set("TERMINAL",0);
	MinMax.set("MINMAX",0);
}

void	Cprogram::makeRules()
{
	int r;
	r=newRule();
	rule[r]->addSymbol(&Expr);
	Start.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&function);
        rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&binaryop);
	rule[r]->addSymbol(&Expr);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	/*
	r=newRule();
	rule[r]->addSymbol(&MinMax);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Comma);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);
*/
	r=newRule();
	rule[r]->addSymbol(&terminal);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sin);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Cos);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Exp);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Log);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Abs);
//	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sqrt);
//	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Tan);
//	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Int);
//	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Log10);
//	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Plus);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Minus);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Mult);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Div);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Pow);
//	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Min);
	MinMax.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Max);
	MinMax.addRule(rule[r]);
	
	
	r=newRule();
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&DigitList);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	terminal.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	terminal.addRule(rule[r]);


	for(int i=0;i<dimension;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&XX[i]);
		XXlist.addRule(rule[r]);
	}

	for(int i=0;i<10;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&Digit[i]);
		Digit0.addRule(rule[r]);
		if(i) Digit1.addRule(rule[r]);
	}
}

int		Cprogram::Parse(string expr)
{
	return (parser.Parse(expr,vars)==-1);
}

int		Cprogram::EvalError()
{
	return	parser.EvalError();
}

double	Cprogram::Eval( const double *X)
{
		return parser.Eval(X);
}

Symbol	*Cprogram::getStartSymbol()
{
	return &Start;
}

Cprogram::~Cprogram()
{
	for(int i=0;i<rule.size();i++)
		delete rule[i];
}

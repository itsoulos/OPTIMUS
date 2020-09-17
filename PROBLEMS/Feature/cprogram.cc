# include <cprogram.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <math.h>


double sig(const double *x)
{
	if(fabs(x[0])>10) return 1e+100;
	return 1.0/(1.0+exp(-x[0]));
}

Cprogram::Cprogram(int dim,int pdim)
{
	parser.AddFunction("sig",sig,1);

	dimension = dim;
	pdimension = pdim;
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
	Delim.set("#",1);
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
	Log10.set("sig",1);
	Min.set("min",1);
	Max.set("max",1);
	Mod.set("%",1);
	If.set("if",1);
	Gt.set(">",1);
	Ge.set(">=",1);
	Lt.set("<",1);
	Le.set("<=",1);
	boolExpr.set("jump",1);
	XX.resize(dimension);
	Avg.set("sig",1);
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


	minx.set("xmin",1);
	maxx.set("xmax",1);
	meanx.set("xmean",1);
	stdx.set("xstd",1);

	Sig.set("sig",1);
}

void	Cprogram::makeNonTerminals()
{
	Start.set("START",0);
	relop.set("REL",0);
	DigitList.set("DIGITLIST",0);
	Digit0.set("DIGIT0",0);
	Digit1.set("DIGIT1",0);
	XXlist.set("XXLIST",0);
	Expr.set("EXPR",0);
	function.set("FUNCTION",0);
	binaryop.set("BINARYOP",0);
	terminal.set("TERMINAL",0);
	MinMax.set("MINMAX",0);

	xfunction.set("XFUNCTION",0);	
	dnumber.set("DNUMBER",0);
	sx.set("SX",0);
}

static	void findDigits(int x,vector<int> &d)
{
	while(x)
	{
		int k=x%10;
		d.push_back(k);
		x=x/10;
	}
}

void	Cprogram::makeRules()
{
	int r;
        r=newRule();
        rule[r]->addSymbol(&Expr);
        Start.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&terminal);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	Expr.addRule(rule[r]);

	/** NEW RULES 
	r=newRule();
	rule[r]->addSymbol(&terminal);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	Expr.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&terminal);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&Plus);
	rule[r]->addSymbol(&Expr);
	Expr.addRule(rule[r]);
	 END NEW RULES **/

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&binaryop);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&function);
        rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	/** NEW RULES ******************/
	r=newRule();
	rule[r]->addSymbol(&xfunction);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Digit[0]);
	rule[r]->addSymbol(&Rpar);
//	XXlist.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&maxx);
	xfunction.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&meanx);
	xfunction.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&stdx);
	xfunction.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&minx);
	xfunction.addRule(rule[r]);

	/* NEW RULES ******************/

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
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sqrt);
	function.addRule(rule[r]);
	
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
	//binaryop.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	terminal.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Rpar);
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

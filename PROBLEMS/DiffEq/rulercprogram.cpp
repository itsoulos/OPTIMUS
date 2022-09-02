#include "rulercprogram.h"

# include <stdio.h>
# include <math.h>

//int problemDimension;

double mf(const double *x)
{
    return fabs(x[0])>1e-5?x[1]:x[2];
}

double myif(const double *x)
{
    if(fabs(x[0])>1e-7)
      return x[1];
    else return -1e+100;
}

double myelse(const double *x)
{
    return x[0];
}

RulerCprogram::RulerCprogram(int dim)
{
    parser.AddConstant("pi",M_PI);
    parser.AddFunction("mf",mf,3);
    parser.AddFunction("myif",myif,2);
    parser.AddFunction("myelse",myelse,1);

    //problemDimension = dimension;
    dimension = dim;
    makeTerminals();
    makeNonTerminals();
    makeRules();
}

int	RulerCprogram::newRule()
{
    int r;
    r=rule.size();
    rule.resize(r+1);
    rule[r]=new DeRule();
    return r;
}

void	RulerCprogram::makeTerminals()
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
    cAnd.set("&",1);
    cOr.set("|",1);
    cEq.set("=",1);
    cNeq.set("!=",1);
    cGt.set(">",1);
    cGe.set(">=",1);
    cLt.set("<",1);
    cLe.set("<=1",1);
    cNot.set("!",1);
    cIf.set("if",1);
    comma.set(",",1);
    separator.set("#",1);
    cMyIf.set("myif",1);
    cMyElse.set("myelse",1);
}

void	RulerCprogram::makeNonTerminals()
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
    relop.set("RELOP",0);
    boolop.set("BOOLOP",0);
    ifexpr.set("IFEXPR",0);
    boolexpr.set("BOOLEXPR",0);
    ifexpr2.set("IFEXPR2",0);
}

void	RulerCprogram::makeRules()
{
    int r;
    r=newRule();
    rule[r]->addSymbol(&ifexpr);
    Start.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cMyIf);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&boolexpr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&Rpar);
    ifexpr2.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cMyIf);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&boolexpr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&Rpar);
    rule[r]->addSymbol(&separator);
    rule[r]->addSymbol(&ifexpr2);
    ifexpr2.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&ifexpr2);
    rule[r]->addSymbol(&separator);
    rule[r]->addSymbol(&cMyElse);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&Rpar);
    ifexpr.addRule(rule[r]);


/*
    r=newRule();
    rule[r]->addSymbol(&Expr);
    ifexpr2.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&ifexpr);
    ifexpr2.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cIf);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&boolexpr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&Rpar);
    ifexpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cIf);
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&boolexpr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&comma);
    rule[r]->addSymbol(&ifexpr2);
    rule[r]->addSymbol(&Rpar);
    ifexpr.addRule(rule[r]);

*/
    r=newRule();
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&relop);
    rule[r]->addSymbol(&Expr);
    boolexpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&relop);
    rule[r]->addSymbol(&Expr);
    rule[r]->addSymbol(&boolop);
    rule[r]->addSymbol(&boolexpr);
    boolexpr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cGt);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cGe);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cLt);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cLe);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cEq);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cNeq);
    relop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cAnd);
    boolop.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&cOr);
    boolop.addRule(rule[r]);

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
  //  Expr.addRule(rule[r]);


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
  //  function.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Sqrt);
   // function.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Tan);
    //function.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Int);
    //function.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Log10);
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
    rule[r]->addSymbol(&Digit0);
    DigitList.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&DigitList);
        //rule[r]->addSymbol(&Digit0);
    DigitList.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&Digit0);
    rule[r]->addSymbol(&Digit0);
  //  DigitList.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    terminal.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&XXlist);
    terminal.addRule(rule[r]);
    Expr.addRule(rule[r]);

    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&binaryop);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Rpar);
 //   Expr.addRule(rule[r]);




    r=newRule();
    rule[r]->addSymbol(&Lpar);
    rule[r]->addSymbol(&DigitList);
    rule[r]->addSymbol(&Dot);
    rule[r]->addSymbol(&DigitList);

    rule[r]->addSymbol(&binaryop);
    rule[r]->addSymbol(&XXlist);
    rule[r]->addSymbol(&Rpar);
   // Expr.addRule(rule[r]);

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

int		RulerCprogram::Parse(string expr)
{
    return (parser.Parse(expr,vars)==-1);
}

int		RulerCprogram::EvalError()
{
    return	parser.EvalError();
}

double	RulerCprogram::Eval( const double *X)
{
        return parser.Eval(X);
}

Symbol	*RulerCprogram::getStartSymbol()
{
    return &Start;
}

RulerCprogram::~RulerCprogram()
{
    for(int i=0;i<rule.size();i++)
        delete rule[i];
}

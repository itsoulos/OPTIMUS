# include <sigprogram.h>
# include <math.h>


static double sig(const double *X)
{
	return 1.0/(1.0+exp(-X[0]));
}

static double remf(const double *X)
{
	return X[0]-(int)X[0];
}

SigProgram::SigProgram(int d)
	:Cprogram(d)
{
	int r;
	Start.cut(0);

	parser.AddFunction("sig",sig,1);
	parser.AddFunction("remf",remf,1);

	Sig.set("sig",1);
	s0.set("s0",0);
	sx.set("sx",0);
	SigFunction.set("SIGF",0);
	Abs.set("remf",1);

	r=newRule();
	rule[r]->addSymbol(&s0);
	Start.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&Sig);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&sx);
	rule[r]->addSymbol(&Plus);
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Rpar);
	s0.addRule(rule[r]);


	r=newRule();
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&Sig);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&sx);
	rule[r]->addSymbol(&Plus);
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Rpar);
	rule[r]->addSymbol(&Plus);
	rule[r]->addSymbol(&s0);
	s0.addRule(rule[r]);


	r=newRule();
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	sx.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Out);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&Plus);
	rule[r]->addSymbol(&sx);
	sx.addRule(rule[r]);


	/**/
	DigitList.cut(0);
	smallNumber.set("SMALLNUMBER",0);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	smallNumber.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	smallNumber.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&DigitList);
	DigitList.addRule(rule[r]);

	Out.set("OUT",0);
	
	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Rpar);
	Out.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Rpar);
	Out.addRule(rule[r]);

}

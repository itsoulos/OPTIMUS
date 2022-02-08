# include <rlsprogram.h>
# include <math.h>

RlsProgram::RlsProgram(Problem *P)
{
	problem = P;
	dimension = problem->getDimension();
	x0=new double[dimension];
	bestx0=new double[dimension];
	makeTerminals();
	makeNonTerminals();
	makeRules();
	int r=newRule();
	start_symbol=new Symbol();
	start_symbol->setName("START");
	start_symbol->setTerminalStatus(0);
	for(int i=0;i<dimension-1;i++)
	{
		rule[r]->addSymbol(&Expr);
		rule[r]->addSymbol(&Delim);
	}
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Delim);
	start_symbol->addRule(rule[r]);
	bestValue=1e+100;
}

void	RlsProgram::makeTerminals()
{
	Add.setName("+");
	Add.setTerminalStatus(1);
	Sub.setName("-");
	Sub.setTerminalStatus(1);
	Mult.setName("*");
	Mult.setTerminalStatus(1);
	Div.setName("/");
	Div.setTerminalStatus(1);
	Sin.setName("sin");
	Sin.setTerminalStatus(1);
	Cos.setName("cos");
	Cos.setTerminalStatus(1);
	Exp.setName("exp");
	Exp.setTerminalStatus(1);
	Log.setName("log");
	Log.setTerminalStatus(1);
	Digit.resize(10);
	for(int i=0;i<10;i++)
	{
		char str[10];
		sprintf(str,"%d",i);
		Digit[i].setName(str);
		Digit[i].setTerminalStatus(1);
	}
	X.setName("x");
	X.setTerminalStatus(1);
	Lpar.setName("(");
	Lpar.setTerminalStatus(1);
	Rpar.setName(")");
	Rpar.setTerminalStatus(1);
	Dot.setName(".");
	Dot.setTerminalStatus(1);
	Delim.setName("#");
	Delim.setTerminalStatus(1);
}

void	RlsProgram::setX0(double *X0,double f)
{
	if(isinf(f) || isnan(f)) return;
	for(int i=0;i<dimension;i++) 
		x0[i]=X0[i];
	fvalue=f;
	if(fvalue<bestValue) 
	{
		for(int i=0;i<dimension;i++) bestx0[i]=X0[i];
		bestValue=fvalue;
	}
}

void	RlsProgram::makeNonTerminals()
{
	Expr.setName("EXPR");
	Expr.setTerminalStatus(0);
	BinaryOp.setName("BINARYOP");
	BinaryOp.setTerminalStatus(0);
	Function.setName("FUNCTION");
	Function.setTerminalStatus(0);
	Terminal.setName("TERMINAL");
	Terminal.setTerminalStatus(0);
	DigitList.setName("DIGITLIST");
	DigitList.setTerminalStatus(0);
	Digit0.setName("DIGIT0");
	Digit0.setTerminalStatus(0);
	Digit1.setName("DIGIT1");
	Digit1.setTerminalStatus(0);
}

void	RlsProgram::makeRules()
{
	int r;
	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	//Expr.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&BinaryOp);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Function);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Terminal);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Add);
	BinaryOp.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sub);
	BinaryOp.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Mult);
	BinaryOp.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Div);
	BinaryOp.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sin);
	Function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Cos);
	Function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Exp);
	Function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Log);
	Function.addRule(rule[r]);
	
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
	Terminal.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Sub);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Rpar);
	Terminal.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&X);
	Terminal.addRule(rule[r]);

	for(int i=0;i<Digit.size();i++)
	{
		r=newRule();
		rule[r]->addSymbol(&Digit[i]);
		Digit0.addRule(rule[r]);
		if(i) Digit1.addRule(rule[r]);
	}
}

int	RlsProgram::newRule()
{
	int s=rule.size();
	rule.resize(s+1);
	rule[s]=new Rule();
	return s;
}

double	RlsProgram::fitness(vector<int> &genome)
{
	double	value=0.0;
	vector<double> dx;
	dx.resize(dimension);
	Data xx1;
	xx1.resize(dimension);
	Data xx2;
	xx2.resize(dimension);
	int ok_flag=0;
	vector<double> lmargin,rmargin;
	lmargin=problem->getLeftMargin();
	rmargin=problem->getRightMargin();
	int ok_flag1=0;
	int ok_flag2=0;
	int redo=0;
	string totalstr=printRandomProgram(genome,redo);
	if(redo>=REDO_MAX) return -1e+100;
	int lastpos=0;
	for(int i=0;i<dimension;i++)
	{
		string str="";
		for(int j=lastpos;totalstr[j]!='#';j++)
		{
			str=str+totalstr[j];
			lastpos++;
		}
		lastpos++;
		int icode=parser.Parse(str,"x");
		dx[i]=parser.Eval(&x0[i]);
		if(isnan(dx[i]) || isinf(dx[i])|| parser.EvalError())
		{
			return -1e+100;
			dx[i]=0.0;
			ok_flag++;
			ok_flag1++;
			ok_flag2++;
		}
	
		xx1[i]=x0[i]+dx[i];
		int pflag=0;
		if(xx1[i]>rmargin[i] || xx1[i]<lmargin[i]) 
		{
			ok_flag++;
			xx1[i]=x0[i];
			ok_flag1++;
		}
		xx2[i]=x0[i]-dx[i];
		if(xx2[i]>rmargin[i] || xx2[i]<lmargin[i]) 
		{
			ok_flag++;
			xx2[i]=x0[i];
			ok_flag2++;
		}

	}
	if(ok_flag>=dimension) return -fvalue;// {return -1e+100;}
	double val1,val2;
	if(problem->isPointIn(xx1) && ok_flag1<dimension)
		val1=problem->funmin(xx1);
	else
	{
		val1=fvalue+1;
	}
	if(val1<fvalue) 
	{
		if(isnan(val1) || isinf(val1)) return -1e+100;
		if(val1<bestValue)
		{
			for(int i=0;i<dimension;i++) bestx0[i]=xx1[i];
			bestValue=val1;
		}
		return -val1;
	}
	else
	{
		if(problem->isPointIn(xx2) && ok_flag2<dimension)
			val2=problem->funmin(xx2);
		else
			val2=fvalue+1;
		if(val2<fvalue)
		{	
			if(isnan(val2) || isinf(val2)) return -1e+100;
			if(val2<bestValue)
			{
				for(int i=0;i<dimension;i++) bestx0[i]=xx2[i];
				bestValue=val2;
			}
			return -val2;
		}
		else
			return -fvalue;
	}
	return 0.0;
}

string	RlsProgram::printProgram(vector<int> &genome)
{
	string s1="\n";
	vector<int> subgenome;
	subgenome.resize(genome.size()/dimension);
	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<genome.size()/dimension;j++)
			subgenome[j]=genome[i*genome.size()/dimension+j];
		int redo=0;
		string str=printRandomProgram(subgenome,redo);
		if(redo>=REDO_MAX) 
		{
			return  "";
		}
		s1=s1+"f(x)="+str+"\n";
	}
	return s1;
}

void	RlsProgram::getX0(double *X0)
{
	for(int i=0;i<dimension;i++) 
	{
		X0[i]=x0[i];
	}
}

double	RlsProgram::getValue() const
{
	return fvalue;
}

double	RlsProgram::getBestValue() const
{
	return bestValue;
}

void	RlsProgram::getBestX0(double *X0)
{
	for(int i=0;i<dimension;i++) X0[i]=bestx0[i];
}

void	RlsProgram::clear()
{
	bestValue = 1e+100;
}

RlsProgram::~RlsProgram()
{
	for(int i=0;i<rule.size();i++) delete rule[i];
	delete start_symbol;
	delete[] x0;
}


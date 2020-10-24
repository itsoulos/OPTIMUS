# include <rlsprogram.h>
# include <math.h>

RlsProgram::RlsProgram(Problem *P)
{
	problem = P;
	dimension = problem->getDimension();
	makeTerminals();
	makeNonTerminals();
	makeRules();
	int r=newRule();
	start_symbol=new Symbol();
	start_symbol->setName("START");
	start_symbol->setTerminalStatus(0);
	
	rule[r]->addSymbol(&Digit[0]);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	
	start_symbol->addRule(rule[r]);

    lmargin=problem->getLeftMargin();
    rmargin=problem->getRightMargin();
}

void	RlsProgram::makeTerminals()
{
	Digit.resize(10);
	for(int i=0;i<10;i++)
	{
		char str[10];
		sprintf(str,"%d",i);
		Digit[i].setName(str);
		Digit[i].setTerminalStatus(1);
	}
	Dot.setName(".");
	Dot.setTerminalStatus(1);
	Delim.setName("#");
	Delim.setTerminalStatus(1);
	Minus.setName("-");
	Minus.setTerminalStatus(1);
}

void	RlsProgram::makeNonTerminals()
{
	Expr.setName("EXPR");
	Expr.setTerminalStatus(0);
	DigitList.setName("DIGITLIST");
	DigitList.setTerminalStatus(0);
	Digit0.setName("DIGIT0");
	Digit0.setTerminalStatus(0);
}

void	RlsProgram::makeRules()
{
	int r;

	r=newRule();
	rule[r]->addSymbol(&Digit[0]);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&Digit[0]);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&DigitList);
	DigitList.addRule(rule[r]);

	for(int i=0;i<Digit.size();i++)
	{
		r=newRule();
		rule[r]->addSymbol(&Digit[i]);
		Digit0.addRule(rule[r]);
	}
}

int	RlsProgram::newRule()
{
	int s=rule.size();
	rule.resize(s+1);
	rule[s]=new Rule();
	return s;
}

double RlsProgram::fitness(vector<double> &xx,vector<int> &genome)
{
    vector<double> dx;
    dx.resize(dimension);
    vector<int> subgenome;
    subgenome.resize(genome.size()/dimension);
    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<subgenome.size();j++)
            subgenome[j]=genome[i*subgenome.size()+j];
        int redo=0;
        string str=printRandomProgram(subgenome,redo);
        if(redo>=REDO_MAX) return -1e+100;
        dx[i]=atof(str.c_str());
        dx[i]=lmargin[i]+(rmargin[i]-lmargin[i])*(dx[i]);
        xx[i]+=dx[i];
    }

    return -problem->funmin(xx);
}
double	RlsProgram::fitness(vector<int> &genome)
{
	double	value=0.0;
	vector<double> dx;
	dx.resize(dimension);

	
	vector<int> subgenome;
	subgenome.resize(genome.size()/dimension);
	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<subgenome.size();j++)
			subgenome[j]=genome[i*subgenome.size()+j];
		int redo=0;
		string str=printRandomProgram(subgenome,redo);
		if(redo>=REDO_MAX) return -1e+100;
		dx[i]=atof(str.c_str());
		dx[i]=lmargin[i]+(rmargin[i]-lmargin[i])*(dx[i]);
	}
    if(!problem->isPointIn(dx)) return 1e+100;
	return -problem->funmin(dx);
}
void   RlsProgram::getX(vector<double> &ix,vector<int> &genome,Data &x)
{
    double	value=0.0;
    vector<double> dx;
    dx.resize(dimension);
    int ok_flag=0;
    vector<int> subgenome;
    subgenome.resize(genome.size()/dimension);
    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<subgenome.size();j++)
            subgenome[j]=genome[i*subgenome.size()+j];
        int redo=0;
        string str=printRandomProgram(subgenome,redo);
        if(redo>=REDO_MAX) return ;
        dx[i]=atof(str.c_str());
        dx[i]=lmargin[i]+(rmargin[i]-lmargin[i])*(dx[i]);
        x[i]=ix[i]+dx[i];
    }
}
void	RlsProgram::getX(vector<int> &genome,Data &x)
{
	double	value=0.0;
	vector<double> dx;
	dx.resize(dimension);
    int ok_flag=0;
	vector<int> subgenome;
	subgenome.resize(genome.size()/dimension);
	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<subgenome.size();j++)
			subgenome[j]=genome[i*subgenome.size()+j];
		int redo=0;
		string str=printRandomProgram(subgenome,redo);
		if(redo>=REDO_MAX) return ;
		dx[i]=atof(str.c_str());
		dx[i]=lmargin[i]+(rmargin[i]-lmargin[i])*(dx[i]);
	}

	x=dx;
}

string	RlsProgram::printProgram(vector<int> &genome)
{
	string s1="";
	int redo=0;
	string totalstr=printRandomProgram(genome,redo);
	if(redo>=REDO_MAX) return "";
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
		s1=s1+" "+str;
	}
	s1="x=["+s1+"]";
	return s1;
}

RlsProgram::~RlsProgram()
{
	for(int i=0;i<rule.size();i++) delete rule[i];
	delete start_symbol;
}


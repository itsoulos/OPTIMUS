# include <program.h>		

Program::Program()
{
	start_symbol = NULL;
}

void	Program::setStartSymbol(Symbol *s)
{
	start_symbol = s;
}

Symbol	*Program::getStartSymbol() const
{
	return start_symbol;
}


string	Program::printRandomProgram(vector<int> &genome,int &redo)
{
#ifdef STOCHASTIC
	srand48(0);
#endif
	string str="";
	int count=0;
	Rule *r;
	int pos=genome[count]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome[count]%start_symbol->getCountRules());
	redo = 0;
	str=r->printRule(genome,count,redo);
	return str;
}


void	Program::updatePositive(vector<int> &genome)
{
#ifdef STOCHASTIC
	srand48(0);
	vector<Rule*> rule;
	vector<double> weight;
	int count=0,redo=0;
	string str="";
	Rule *r;
	int pos=genome[count]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome[count]%start_symbol->getCountRules());
	r->updatePositive(genome,count,redo,rule,weight);
	for(int i=0;i<rule.size();i++)
	{
		rule[i]->setWeight(weight[i]);
	}
#endif
}

void	Program::updateNegative(vector<int> &genome)
{
#ifdef STOCHASTIC
	srand48(0);
	vector<Rule*> rule;
	vector<double> weight;
	int count=0,redo=0;
	string str="";
	Rule *r;
	int pos=genome[count]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome[count]%start_symbol->getCountRules());
	r->updateNegative(genome,count,redo,rule,weight);
	for(int i=0;i<rule.size();i++)
	{
		rule[i]->setWeight(weight[i]);
	}
#endif
}

void	Program::updateAll(vector<int> &genome1,vector<int> &genome2)
{
#ifdef STOCHASTIC
	srand48(0);
	vector<Rule*> rule1;
	vector<Rule*> rule2;
	vector<double> weight1;
	vector<double> weight2;
	int count1=0,count2=0,redo1=0,redo2=0;
	Rule *r;
	int pos=genome1[count1]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome1[count1]%start_symbol->getCountRules());
	r->updatePositive(genome1,count1,redo1,rule1,weight1);
	r->updateNegative(genome2,count2,redo2,rule2,weight2);
	for(int i=0;i<rule1.size();i++)
	{
		rule1[i]->setWeight(weight1[i]);
	}
	for(int j=0;j<rule2.size();j++)
	{
		int found=0;
		for(int k=0;k<rule1.size();k++)
		{
			if(rule1[k]==rule2[j])
			{
				found=1;
				break;
			}
		}
		if(found) continue;
		rule2[j]->setWeight(weight2[j]);
	}
#endif
}

double	Program::fitness(vector<int> &genome)
{
	return 0.0;
}

Program::~Program()
{
}

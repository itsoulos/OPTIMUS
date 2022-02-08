# include <rule.h>
# include <iostream>
Rule::Rule()
{
	length =0;
}

void	Rule::addSymbol(Symbol *s)
{
	data.push_back(s);
	length++;
}

int	Rule::getSymbolPos(string s)
{
	for(int i=0;i<length;i++)
		if(data[i]->getName()==s) return i;
	return -1;
}

Symbol	*Rule::getSymbol(int pos) const
{
	if(pos<0 || pos>=length) return NULL;
	return data[pos];
}

void	Rule::setSymbol(int pos,Symbol *s)
{
	if(pos<0 || pos>=length) return; 
	data[pos]=s;
}

int	Rule::getLength() const
{
	return length;
}

string	Rule::printRule(vector<int> genome,int &pos,int &redo)
{

	string str="";
	string str2="";
	Rule *r;
	int old_pos;
	for(int i=0;i<length;i++)
	{
		Symbol *s=data[i];
		if(s->getTerminalStatus())
		{
			str=str+s->getName();	
		}
		else
		{
			if(pos>=genome.size()) {redo++;pos=0;}
			r=s->getRule((genome[pos]%s->getCountRules()));
			pos++;
			if(pos>=genome.size()) {redo++;pos=0;}
			if(redo>=REDO_MAX) return str;
			str2=r->printRule(genome,pos,redo);
			str=str+str2;
		}
	}
	return str;
}

double	Rule::getValue(vector<int> genome,int &pos,int &redo,DoubleStack &stack,double *X)
{
	for(int i=0;i<length;i++)
	{
		Symbol *s=data[i];
		if(s->getTerminalStatus())
		{
			string str=s->getName();
			if(str==string("+"))
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				cout<<"a ="<<a<<"b="<<b<<endl;
				stack.push(b+a);
			}
			else
			if(str=="-")
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				stack.push(b-a);
			}
			else
			if(str=="*")
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				stack.push(b*a);
			}
			else
			if(str=="x")
			{
				stack.push(X[0]);
			}
		}
		else
		{
		if(pos>=genome.size()) {redo++;pos=0;}
		int k=genome[pos]%s->getCountRules();
		Rule *r;
		double rnd;
		r=s->getRule(genome[pos]%s->getCountRules());
		pos++;
		if(pos>=genome.size()) {redo++;pos=0;}
		if(redo>=REDO_MAX) return 0;
		return (r->getValue(genome,pos,redo,stack,X));
		}
	}
	return stack.top();
}


Rule::~Rule()
{
}


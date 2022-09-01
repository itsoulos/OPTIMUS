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
	string str="";
	int count=0;
	Rule *r;
	int pos=0;
	r=start_symbol->getRule(0);
	redo = 0;
	str=r->printRule(genome,count,redo);
	lastsize= count;
	return str;
}
int Program::getLastSize()
{
	return lastsize;
}


double	Program::fitness(vector<int> &genome)
{
	return 0.0;
}

Program::~Program()
{
}

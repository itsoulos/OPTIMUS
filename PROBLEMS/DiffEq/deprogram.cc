# include <deprogram.h>

DeProgram::DeProgram()
{
	start_symbol = NULL;
}

void	DeProgram::setStartSymbol(Symbol *s)
{
	start_symbol = s;
}

Symbol	*DeProgram::getStartSymbol() const
{
	return start_symbol;
}


string	DeProgram::printRandomProgram(vector<int> &genome,int &redo)
{
	string str="";
	int count=0;
    DeRule *r;
	int pos=0;
	r=start_symbol->getRule(0);
	redo = 0;

	str=r->printRule(genome,count,redo);
	lastsize= count;
	return str;
}
int DeProgram::getLastSize()
{
	return lastsize;
}


double	DeProgram::fitness(vector<int> &genome)
{
	return 0.0;
}

DeProgram::~DeProgram()
{
}

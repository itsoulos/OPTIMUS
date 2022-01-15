# include <grs/program.h>

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
    int pos;
//#pragma omp single
{
    pos = genome[count]%start_symbol->getCountRules();

    //while (start_symbol->getRule(pos)==NULL);
    r = start_symbol->getRule(pos);

	redo = 0;
//#pragma omp critical
	str=r->printRule(genome,count,redo);
}
	return str;
}

double	Program::fitness(vector<int> &genome)
{
	return 0.0;
}

Program::~Program()
{
}

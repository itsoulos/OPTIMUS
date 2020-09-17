# include <symbol.h>
Symbol::Symbol()
{
	name = "";
	count_rules = 0;
	is_terminal = 1;
}


void	Symbol::set(string s,int status)
{
	setName(s);
	setTerminalStatus(status);
}

void	Symbol::setName(string s)
{
	name = s;
}

string	Symbol::getName() const
{
	return name;
}

void	Symbol::setTerminalStatus(int status)
{
	is_terminal=status %  2;
}

int	Symbol::getTerminalStatus() const
{
	return is_terminal;
}

void	Symbol::addRule(Rule *r)
{
	rule.push_back(r);
	count_rules++;
}

Rule	*Symbol::getRule(int pos) const
{
	if(pos<0 || pos>=count_rules) return NULL;
	return rule[pos];
}

int	Symbol::getCountRules() const
{
	return count_rules;
}

void	Symbol::cut(int N)
{
	count_rules=N;
	rule.resize(N);
}

Symbol::~Symbol()
{
}

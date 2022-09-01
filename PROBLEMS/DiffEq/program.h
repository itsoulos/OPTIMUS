# ifndef __PROGRAM__H
# include <symbol.h>
# include <rule.h>
# include <doublestack.h>
# include <vector>
using namespace std;

class Program
{
	protected:
		Symbol *start_symbol;
		int    lastsize;
	public:
		Program();
		void	setStartSymbol(Symbol *s);
		Symbol	*getStartSymbol() const;
		string	printRandomProgram(vector<int> &genome,int &redo);
		int	hasFailed();
		int	parse(vector<Symbol*> slist);
		int	getLastSize();
		virtual double	fitness(vector<int> &genome);
		~Program();
};

# define __PROGRAM__H
# endif

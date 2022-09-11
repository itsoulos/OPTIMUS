# ifndef __PROGRAM__H
# include <symbol.h>
# include <derule.h>
# include <doublestack.h>
# include <vector>
using namespace std;

class DeProgram
{
	protected:
		Symbol *start_symbol;
		int    lastsize;
	public:
        DeProgram();
		void	setStartSymbol(Symbol *s);
		Symbol	*getStartSymbol() const;
		string	printRandomProgram(vector<int> &genome,int &redo);
		int	hasFailed();
		int	parse(vector<Symbol*> slist);
		int	getLastSize();
		virtual double	fitness(vector<int> &genome);
        ~DeProgram();
};

# define __PROGRAM__H
# endif

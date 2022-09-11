# ifndef __RULE__H
# include <symbol.h>
# include <vector>
# define REDO_MAX	2
using namespace std;

class DeRule
{
	private:
		vector<Symbol*> data;	
		int	length;
	public:
        DeRule();
		void	addSymbol(Symbol *s);
		int	getSymbolPos(string s);
		Symbol	*getSymbol(int pos) const;
		void	setSymbol(int pos,Symbol *s);
		int	getLength() const;
		string	printRule(vector<int> genome,int &pos,int &redo);
        ~DeRule();
};
# define __RULE__H
# endif

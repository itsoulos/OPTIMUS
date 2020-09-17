# ifndef __RULE__H
# include <symbol.h>
# include <doublestack.h>
# include <vector>
# define REDO_MAX	2
using namespace std;

class Rule
{
	private:
		double	weight;
		vector<Symbol*> data;	
		int	length;
	public:
		Rule();
		void	addSymbol(Symbol *s);
		int	getSymbolPos(string s);
		Symbol	*getSymbol(int pos) const;
		void	setSymbol(int pos,Symbol *s);
		int	getLength() const;
		string	printRule(vector<int> genome,int &pos,int &redo);
		void	updatePositive(vector<int> genome,int &pos,int &redo,
				vector<Rule*> &rule,vector<double> &weight);
		void	updateNegative(vector<int> genome,int &pos,int &redo,
				vector<Rule*> &rule,vector<double> &weight);
		double	getWeight() const;
		void	setWeight(double w);
		~Rule();
};
# define __RULE__H
# endif

# ifndef __MYSYMBOL__H
# include <string>
# include <vector>
using namespace std;

class Rule;

/**
 * @brief The Symbol class represents a symbol to be used in
 *  the Grammatical Evolution procedure.
 */
class Symbol
{
	private:
		string name;
        vector<Rule*> rule;
		int	count_rules;
		int	is_terminal;
	public:
		Symbol();

		void	set(string s,int status);
		void	setName(string s);
		string	getName() const;
		
		void	setTerminalStatus(int status);
		int	getTerminalStatus() const;

        void	addRule(Rule *r);
		void	cut(int N);
        Rule	*getRule(int pos) const;
		int	getCountRules() const;
		~Symbol();
		
};

# define __MYSYMBOL__H
# endif

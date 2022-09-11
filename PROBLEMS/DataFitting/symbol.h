# ifndef __SYMBOL__H
# include <string>
# include <vector>
using namespace std;

class DeRule;

class Symbol
{
	private:
		string name;
        vector<DeRule*> rule;
		int	count_rules;
		int	is_terminal;
	public:
		Symbol();

		void	set(string s,int status);
		void	setName(string s);
		string	getName() const;
		
		void	setTerminalStatus(int status);
		int	getTerminalStatus() const;

        void	addRule(DeRule *r);
		void	cut(int N);
        DeRule	*getRule(int pos) const;
		int	getCountRules() const;
		~Symbol();
		
};

# define __SYMBOL__H
# endif

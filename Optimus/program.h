# ifndef __PROGRAM__H
# include "symbol.h"
# include "rule.h"
# include "doublestack.h"
# include <vector>
using namespace std;

/**
 * @brief The Program class implements the Genetic Program used in the Grammatical Evolution process.
 */
class Program
{
	protected:
		Symbol *start_symbol;
	public:
        /**
         * @brief Program the constructor of the class.
         */
		Program();

        /**
         * @brief setStartSymbol sets the start symbol of the genetic grammar.
         * @param s
         */
		void	setStartSymbol(Symbol *s);
        /**
         * @brief getStartSymbol returns the start symbol of the genetic grammar.
         * @return
         */
		Symbol	*getStartSymbol() const;
        /**
         * @brief printRandomProgram, constructs in string format a new program.
         * @param chromosome, the chromosome used in the genetic process.
         * @param redo
         * @return
         */
        string	printRandomProgram(vector<int> &chromosome,int &redo);
		int	hasFailed();
		int	parse(vector<Symbol*> slist);
        /**
         * @brief fitness, return the fitness value for a given chromosome.
         * @param chromosome
         * @return
         */
        virtual double	fitness(vector<int> &chromosome);
		~Program();
};

# define __PROGRAM__H
# endif

# ifndef __SIGPROGRAM__H
# include "cprogram.h"
# include <string>
using namespace std;

/**
 * @brief The SigProgram class constructs  the grammar for
 * Constructed Neural networks by the Grammatical Evolution procedure.
 */
class	SigProgram :public Cprogram
{
	private:
		Symbol  Sig,SigFunction,Tanh,Abs,
			s0,sx,Out,smallNumber;
	public:
		SigProgram(int d);
};

# define __SIGPROGRAM__H
# endif

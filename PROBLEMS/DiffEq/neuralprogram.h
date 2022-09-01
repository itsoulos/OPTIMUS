# ifndef __NEURALPROGRAM__H
# define __NEURALPROGRAM__H
# include <program.h>
# include <sigprogram.h>
# include <neuralparser.h>
# include <QString>
# include <QFile>
# include <QTextStream>
# include <QIODevice>
typedef vector<double> Data;
class NeuralProgram :
	public Program
{
	protected:
		int 		dimension;
		SigProgram	*program;
		int		multiple;
	public:
		NeuralParser		*neuralparser;
		vector<NeuralParser*> 	nparser;
		NeuralProgram(int Dimension);
        NeuralProgram();
        void    setDimension(int Dimension);
		void		enableMultiple(int K);
		int		ismultiple();
		string  printProgram(vector<int> &genome);
		int	getElements(vector<int> &genome,string &str1);
		double 	fitness(vector<int> &genome);
		double  TestError(vector<int>  &genome);
         void	getDeriv(Data &g);

         double	getTrainError();
         double	getTrainError(Data x);
         double	getTestError();
         double 	penalty1();
        double 	penalty2();
        double 	penalty3();
         double 	penalty4();
         double  getPartError();
         double  getPartError(Data &value);
         void    printOutput(QString filename)
        {

        };
		int getWeightSize() ;
		~NeuralProgram();
};

# endif

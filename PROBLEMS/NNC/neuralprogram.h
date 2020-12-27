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
		virtual void	getDeriv(Data &g);

		virtual double	getTrainError();
		virtual double	getTrainError(Data x);
		virtual double	getTestError();
		virtual double 	penalty1();
		virtual double 	penalty2();
		virtual double 	penalty3();
		virtual double 	penalty4();
		virtual double  getPartError();
		virtual double  getPartError(Data &value);
        	virtual void    printOutput(QString filename)=0;
		int getWeightSize() ;
		~NeuralProgram();
};

# endif

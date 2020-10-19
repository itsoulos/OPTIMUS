# ifndef __CLASSPROGRAM__H
# include <program.h>
# include <cprogram.h>
# include <QVector>
# include <vector>
using namespace std;

typedef QVector<double> Data;

class ClassProgram	:public Program
{
	private:
		vector<double> vclass;
		vector<string> pstring;
		vector<int>    pgenome;
        	QVector<Data> trainx;
        	Data          trainy;
		vector<double>  outy;
		Cprogram *program;
		vector<double> mapper;
		int dimension,nclass;
	public:
        ClassProgram(QVector<Data> &x,Data &y);
        ClassProgram();
        void            setData(QVector<Data> &x,Data &y);
        string          printF(vector<int> &genome);
        int             findMapper(double x);
		virtual double 	fitness(vector<int> &genome);
        double          getClassError(vector<int> &genome,QVector<Data> &x,Data &y);
        int             getClass() const;
		~ClassProgram();
};
# define __CLASSPROGRAM__H
# endif

# ifndef __NNCNEURALPROGRAM__H
# define __NNCNEURALPROGRAM__H
# include <neuralprogram.h>
# include <QString>
typedef vector<double> Data;

class NNCNeuralProgram :
	public NeuralProgram
{
	private:
		vector<Data>	train_xpoint;
		Data		xmax;
		Data		xmin;
		Data		train_ypoint;
		double		*xtemp;
		vector<Data>	test_xpoint;
		Data		test_ypoint;
		Data		categ;
	public:
        NNCNeuralProgram();
        void setAll1(int Dimension,QString TrainFile,QString TestFile);
        void setAll(int Dimension,vector<Data> &xx,Data &y,vector<Data> &tx,Data &ty);
        NNCNeuralProgram(int Dimension,QString TrainFile,QString TestFile);
		int	isvalidation;
		virtual double	getTrainError();
		virtual double  getTestError();
		virtual void	getDeriv(Data &g);
		double	getClassTestError(vector<int> &genome);
		int	getTrainSize() const;
		int	getTestSize() const;
        virtual void    printOutput(QString filename);
		~NNCNeuralProgram();
};

# endif

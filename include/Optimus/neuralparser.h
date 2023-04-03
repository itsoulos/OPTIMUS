# ifndef __NEURALPARSER__H
# define __NEURALPARSER__H
# include <string>
# include <vector>
using namespace std;
typedef vector<double> Data;


struct Multiplier
{
	double	m;
	int	index;
};

struct Node
{
	double out;
	vector<Multiplier> mult;	
	double bias;
};

class NeuralParser
{
	private:
		int dimension;
		vector<int> 	fixstatus;
		vector<Node> 	node;
		vector<double>	weight;
		Data left,right;
	public:
		void setleftmargin(Data &x);
		void setrightmargin(Data &x);
		Data getleftmargin();
		Data getrightmargin();
		int   sigcount,violcount;
		NeuralParser(int Dimension);
		int	getDimension() const;
		void	makeVector(string str);
		void	getWeights(vector<double> &w);
		void	getFixStatus(vector<int> &status);	
		void	setWeights(vector<double> w);
		double	eval(vector<double> xpoint);
		double	eval(double *xpoint);
		double  evalDeriv(vector<double> xpoint,int pos);
		double	evalDeriv2(vector<double> xpoint,int pos);
		void	getDeriv(vector<double> xpoint,vector<double> &g);
		void	getXDeriv(vector<double> xpoint,int pos,vector<double> &g);
		void	getX2Deriv(vector<double> xpoint,int pos,vector<double> &g);
		virtual double	valError();
		void	getMargins(Data &l,Data &r);
		string  print();
		int getWeightSize();
		~NeuralParser();
};
# endif

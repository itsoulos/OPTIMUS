# ifndef __MAPPER__H
# define __MAPPER__H
# include <fparser.hh>
# include <string>
using namespace std;
typedef vector<double> Matrix;
class Mapper
{
	private:
		int dimension;
		string vars;
        double scale_factor;
		vector<FunctionParser*> parser;
		vector<int> foundX;
		int countx;
	public:
        Mapper(int d,double scale);
		void	setExpr(vector<string> s);
		int	map(Matrix x,Matrix &x1);
		~Mapper();
};
# endif

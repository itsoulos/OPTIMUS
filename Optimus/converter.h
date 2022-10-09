# ifndef __CONVERTER__H
# define __CONVERTER__H
# include <string>
# include <vector>
using namespace std;
typedef vector<double> Data;

# include <string.h>
# include <stdio.h>
# include <stdlib.h>

class Converter
{
	private:
		int	    sigcount;
		int	    dimension;
		int	    xcounter;
		int	    nodecounter;
		int	    nodes;
		Data	    weight;
		vector<int>	list;
		void        makeOut(vector<int> &g,int &pos);
		void	    makeWeight(vector<int> &g,int &pos);
		void        makeBias(vector<int> &g,int &pos);
		void        makeNode(vector<int> &g,int &pos);
		void	    makeX(vector<int> &g,int &pos);
		void	    makeNumber(double x,vector<int> &g,int &pos);
		void	    add(int x,vector<int> &g,int &pos);
	public:
		Converter(Data x,int s,int d);
		void convert(vector<int> &g);
};
# endif

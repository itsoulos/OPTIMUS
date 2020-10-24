# ifndef __COLLECTION__H
# include <vector>
using namespace std;
typedef vector<double> Data;

class Collection
{
	private:
		int dimension;
		vector<Data> xpoint;
		Data ypoint;
	public:
		Collection(int d);
		void	addSample(Data x,double y);
		void	replaceSample(int pos,Data x,double y);
		void	replaceSample(int pos,Data x);
		void	replaceSample(int pos,double y);
		void	getSampleX(int pos,Data &x);
		void	getSampleY(int pos,double &y);
		double	getSampleY(int pos);
		void	sortDesc();
		int	isSampleIn(Data x,double y);
		int	getDimension();
		int	getSize() const;
		void	clear();
		void	Cut(int N);
		~Collection();
};

# define __COLLECTION__H
# endif

#ifndef RBF_H
#define RBF_H
# include <vector>
using namespace std;
typedef vector<double> Data;

extern double   RBFgetValue(int nodes,Data &w,Data &point);
extern double   RBFgetGradient(int nodes,Data &w,Data &point);

#endif // RBF_H

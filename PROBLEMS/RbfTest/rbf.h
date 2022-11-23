#ifndef RBF_H
#define RBF_H
# include <dataset.h>
# include <kmeans.h>
# include <QVector>
# include <vector>
#include <adept.h>
# include <armadillo>
using namespace arma;
using namespace  std;
typedef QVector<Data> Matrix;
namespace RbfTestName
{
class Rbf
{
private:
    Data weight;
    QVector<Data> centroid;
    Data variance;
    Dataset *trainSet,*testSet;
    int   nweights;//centroids
    int   pattern_dimension;
     Data    classVector;
     Data px;
    Matrix A,RealOutput;
    mat armaA;
    vec armaRealOutput;
    void    init_arrays();
    double  gauss_function(Data &pattern,Data &center,double sigma);
    Matrix  matrix_transpose(Matrix &x);
    Matrix  matrix_mult(Matrix &x,Matrix &y);
    Matrix  matrix_inverse(Matrix x,bool &ok);
    Matrix  matrix_pseudoinverse(Matrix &x,bool &ok);
public:
    Rbf();
    void setWeights(Data &x);
    void setWeights(vector<double> &x);
    Data getWeights();
    void    setNumberOfWeights(int K);
    int     getNumberOfWeights();
    void    setTrainSet(Dataset *t);
    void    setTestSet(Dataset *t);
    bool    train();
    double	getOutput(Data &w,Data &pattern);
    void	setParameters(vector<double> &x);
    double  getOutput(Data &pattern);
    double  getClass(Data &pattern);
    double  getTrainError();
    adept::adouble getTrainError(QVector<adept::adouble> &x);
    double  getTrainError(Data &x);
    double  getTestError();
    double  getClassError();
    double  product(Data &x,Data &y);
    ~Rbf();
};
}
#endif // RBF_H

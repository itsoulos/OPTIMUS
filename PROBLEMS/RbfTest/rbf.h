#ifndef RBF_H
#define RBF_H
# include <dataset.h>
# include <kmeans.h>
# include <QVector>
# include <vector>
#include <adept.h>
using namespace  std;
typedef QVector<Data> Matrix;
class Rbf
{
private:
    Data weight;
    QVector<Data> centroid;
    Data variance;
    kmeans *alg;
    Dataset *trainSet,*testSet;
    int   nweights;//centroids
    int   pattern_dimension;
     Data    classVector;
    Matrix A,RealOutput;
    void    init_arrays();
    double  gauss_function(Data &pattern,Data &center,double sigma);
    Matrix  matrix_transpose(Matrix &x);
    Matrix  matrix_mult(Matrix &x,Matrix &y);
    Matrix  matrix_inverse(Matrix x);
    Matrix  matrix_pseudoinverse(Matrix &x);
public:
    Rbf();
    void setWeights(Data &x);
    void setWeights(vector<double> &x);
    Data getWeights();
    void    setNumberOfWeights(int K);
    int     getNumberOfWeights();
    void    setTrainSet(Dataset *t);
    void    setTestSet(Dataset *t);
    void    train();
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

#endif // RBF_H

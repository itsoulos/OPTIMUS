#ifndef DATASET_H
#define DATASET_H
# include <vector>
# include <string>
# include "problem.h"
using namespace std;

typedef vector<double> Data;
typedef vector<Data>  Matrix;

/**
 * @brief The Dataset class, this is a general purpose class
 * to describe datasets used in machine learning problems.
 */
class Dataset
{
private:
    Matrix xpoint;
    Data ypoint;
public:
    /**
     * @brief Dataset, constructs a new dataset with x,y as the points
     * @param x
     * @param y
     */
    Dataset(Matrix &x,Data &y);

    /**
     * @brief Dataset, constructs a new dataset using uniform points from a Problem
     * @param p, the problem to take random samples.
     * @param N, the number of samples.
     */
    Dataset(Problem *p,int N);

    /**
     * @brief getx
     * @param pos
     * @return the xpoint at position pos
     */
    Data    getx(int pos) const;

    /**
     * @brief gety
     * @param pos
     * @return the ypoint at position pos
     */
    double  gety(int pos) const;

    /**
     * @brief getdimension
     * @return  the dimension of the dataset.
     */
    int     getdimension() const;

    /**
     * @brief getpatterns
     * @return  the number of patterns in dataset.
     */
    int     getpatterns() const;
    /**
     * The destructor of the class.
     */

    void addPoint(Data &x,double y);
    ~Dataset();
};

#endif // DATASET_H

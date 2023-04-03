#ifndef __COLLECTION__H
#include <vector>
using namespace std;
typedef vector<double> Data;

/**
 * @brief The Collection class creates a collection of points and function values.
 */
class Collection
{
    /**
     * @brief Collection description.
     * **/
private:
    int dimension;
    vector<Data> xpoint;
    Data ypoint;

public:
    /**
     * @brief Collection, Constructor of the Class
     * @param d is dimension of vector.
     * **/
    Collection(int d);
    /**
     * @brief addSample, sample addition  .
     * @param x is sample.
     * @param y value of sample.
     * **/
    void addSample(Data x, double y);
    /**
     * @brief replaceSample, sample replacement.
     * @param pos is position of sample.
     * @param x the sample for replacement
     * @param y value of sample
     * **/
    void replaceSample(int pos, Data x, double y);
    /* @brief replaceSample, sample replacement.
     * @param pos is position of sample.
     * @param x the sample for replacement
     * */
    void replaceSample(int pos, Data x);
    /**
     * @brief replaceSample, sample replacement.
     * @param pos is position of sample.
     * @param y value of sample
     * **/
    void replaceSample(int pos, double y);
    /**
     * @brief getSampleX, sample return.
     * @param pos is position of sample.
     * @param x the sample for replacement
     * **/
    void getSampleX(int pos, Data &x);
    /**
     * @brief getSampleΥ, sample return.
     * @param pos is position of sample.
     * @param y value of sample
     * **/
    void getSampleY(int pos, double &y);
    /**
     * @brief getSampleΥ, sample return.
     * @param pos is position of sample.
     * @return the value of sample
     * **/
    double getSampleY(int pos);
    /**
     * @brief sortDesc, sorting the collectionn.
     * **/
    void sortDesc();
    /**
     * @brief isSampleIn, check if it is inside.
     * @param x is sample.
     * @param y value of sample.
     * @return 0 if it is not inside.
     * **/
    int isSampleIn(Data x, double y);
    /**
     * @brief getDimension.
     * @return dimension of vector.
     * **/
    int getDimension();
    /**
     * @brief getSize.
     * @return size of collection
     * **/
    int getSize() const;
    /**
     * @brief clear, clear of collection.
     * **/
    void clear();
    /**
     * @brief Cut, cut of collection.
     * @param N is new size.
     * **/
    void Cut(int N);
    /**
     * @brief ~Collection, is destructor.
     **/
    ~Collection();
};

#define __COLLECTION__H
#endif

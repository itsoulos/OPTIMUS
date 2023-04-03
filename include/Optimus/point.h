#ifndef POINT__H
#define POINT__H

#include <vector>
#include <cassert>
#include <cmath>
#include <string>

/**
 * @brief The Point class implements a help class used in the K-Means.
 */
class Point
{
private:
    int pointId, clusterId;
    int dimensions;
    std::vector<double> values;

public:
    /** @brief Point, is the constructor of the class.
     **/
    Point();
    /**
     * @brief Point, is the constructor of the class.
     * @param id is identity of point
     * @param line is the point as a string
     **/
    Point(int id, std::string line);
    /** @brief Point, is the constructor of the class.
     * @param id is identity of point
     * @param line is the point as a vbector
     **/
    Point(int id, std::vector<double> v);
    /** @brief Point, is the constructor of the class.
     * @param id is identity of point
     * @param cid is identity cluster of point
     * @param line is the point as a vbector
     **/
    Point(int id, int cid, std::vector<double> vdata);
    /**
     * @brief getDimensions
     * @return dimension of sample.
     */
    int getDimensions();
    /**
     * @brief getCluster
     * @return identity of cluster.
     */
    int getCluster();
    /**
     * @brief getCluster
     * @return identity of point.
     */
    int getID();
    /**
     * @brief setCluster.
     * @param val is identity Cluster
     **/
    void setCluster(int val);
    /**
     * @brief getVal
     * @return value of point.
     */
    double getVal(int pos);
    /**
     * @brief getData
     * @return the data from point.
     */
    std::vector<double> getData();
    /**
     * @brief distance (static)
     * @return the distance between two points.
     */
    static double distance(Point &p1, Point &p2);
};
#endif

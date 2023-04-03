#ifndef CLUSTER__H
#define CLUSTER__H

#include "point.h"
/**
 * @brief description of cluster of points
 */
class Cluster
{
private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;

public:
    /**
     * @brief Cluster, Constructor of the Method      *
     * @param clusterId is identity of cluster.
     * @param centroid is centroid of cluster.
     * **/
    Cluster(int clusterId, Point centroid);
    /**
     * @brief addPoint, adding point in cluster.
     * @param p is the point to be added in cluster.
     */
    void addPoint(Point p);
    /**
     * @brief removePoint, adding point in cluster.
     * @param p is the point to be removed from cluster.
     */
    bool removePoint(int pointId);
    /**
     * @return the value of id.
     */
    int getId();
    /**
     * @param pos is the location of point.
     * @return the point in location pos.
     */
    Point getPoint(int pos);
    /**
     * @return the number of points.
     */
    int getSize();
    /**
     * @param pos is the location of centroid.
     * @return the centroid in location pos.
     */
    double getCentroidByPos(int pos);
    /**
     * @param pos is the location of centroid.
     * @param val is value of centroid in location pos.
     */
    void setCentroidByPos(int pos, double val);
};

#endif

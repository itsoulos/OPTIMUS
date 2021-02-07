#ifndef CLUSTER__H
#define CLUSTER__H

#include "point.h"

class Cluster
{
private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;

public:
    Cluster(int clusterId, Point centroid);
    void addPoint(Point p);
    bool removePoint(int pointId);
    int getId();
    Point getPoint(int pos);
    int getSize();
    double getCentroidByPos(int pos);
    void setCentroidByPos(int pos, double val);
};

#endif

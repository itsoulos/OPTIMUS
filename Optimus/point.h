#ifndef POINT__H
#define POINT__H

#include <vector>
#include <cassert>
#include <cmath>
class Point
{
private:
    int pointId, clusterId;
    int dimensions;
    std::vector<double> values;

public:
    Point();
    Point(int id, std::string line);
    Point(int id, std::vector<double> v);
    Point(int id, int cid, std::vector<double> vdata);
    int getDimensions();
    int getCluster();
    int getID();
    void setCluster(int val);
    double getVal(int pos);
    std::vector<double> getData();
    static double distance(Point &p1, Point &p2);
};
#endif

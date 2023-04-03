#include <vector>
#include <sstream>
#include <istream>
#include "point.h"

Point::Point()
{
    dimensions = 0;
    pointId = 0;
    clusterId = 0;
}

Point::Point(int id, std::string line)
{
    dimensions = 0;
    pointId = id;
    std::stringstream is(line);
    double val;
    while (is >> val)
    {
        values.push_back(val);
        dimensions++;
    }
    clusterId = 0; //Initially not assigned to any cluster
}

Point::Point(int id, std::vector<double> v)
{
    dimensions = 0;
    pointId = id;
    for (double val : v)
    {
        values.push_back(val);
        dimensions++;
    }
    clusterId = 0; //Initially not assigned to any cluster
}
Point::Point(int id, int cid, std::vector<double> vdata)
{
    this->dimensions = 0;
    this->pointId = id;
    this->clusterId = cid;
    for (double val : vdata)
    {
        values.push_back(val);
        dimensions++;
    }
}
std::vector<double> Point::getData()
{
    std::vector<double> tmp;
    for (int idx = 0; idx < this->getDimensions(); idx++)
        tmp.push_back(this->getVal(idx));
    return tmp;
}
int Point::getDimensions()
{
    return dimensions;
}

int Point::getCluster()
{
    return clusterId;
}

int Point::getID()
{
    return pointId;
}

void Point::setCluster(int val)
{
    clusterId = val;
}

double Point::getVal(int pos)
{
    return values[pos];
}
double Point::distance(Point &p1, Point &p2)
{
    //assert(p1.getDimensions() == p2.getDimensions());
    double dist = 0.0;

    for (int idx = 0; idx < p1.getDimensions(); ++idx)
    {
        const double tmp = p1.getVal(idx) - p2.getVal(idx);
        dist += tmp * tmp;
    }
    return sqrt(dist);
}

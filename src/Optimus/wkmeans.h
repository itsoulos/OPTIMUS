#ifndef WMEANS__H
#define WMEANS__H

#include "cluster.h"
#include "point.h"

class WKMeans
{
private:
    int K, minK, iters, dimensions, total_points;
    std::vector<Cluster> clusters;
    std::vector<Point> means;
    std::vector<std::vector<Point>> allMeansWithPoints;
    std::vector<int> sizes;
    int getNearestClusterId(Point &point);
    int calculateWeight(Point &point,double);
    double calculateWSum(std::vector<Point>);
public:
    WKMeans(int K, int iterations, int minK);
    void run(std::vector<Point> &all_points);
    std::vector<Point> getMeans();
    std::vector<std::vector<Point>> getPointsOfAllMeans();
    void deleteMean(int);
    void setCenters(int);
    std::vector<int> getSizes();
    std::vector<Point> getPointsOfMean(int pos);
};

#endif

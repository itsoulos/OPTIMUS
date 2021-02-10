#ifndef KMEANS__H
#define KMEANS__H

#include "cluster.h"
#include "point.h"

class KMeans
{
private:
    int K, minK, iters, dimensions, total_points;
    std::vector<Cluster> clusters;
    int getNearestClusterId(Point point);
    std::vector<Point> means;
    std::vector<std::vector<Point>> PMeans;
    std::vector<int> sizes;

public:
    KMeans(int K, int iterations, int minK);
    void run(std::vector<Point> &all_points);
    std::vector<Point> getMeans();
    std::vector<std::vector<Point>> pointsOfMeans();
    void deleteMean(int);
    void setCenters(int);
    std::vector<int> getSizes();
};

#endif

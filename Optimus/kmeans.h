#ifndef KMEANS__H
#define KMEANS__H

#include "cluster.h"
#include "point.h"

class KMeans
{
private:
    int K, iters, dimensions, total_points;
    std::vector<Cluster> clusters;
    int getNearestClusterId(Point point);
    std::vector<Point>  means;
    std::vector<std::vector<Point>> PMeans;
public:
    KMeans(int K, int iterations);
    void run(std::vector<Point> &all_points);
    std::vector<Point> getMeans();
    std::vector<std::vector<Point>> pointsOfMeans();
};

#endif

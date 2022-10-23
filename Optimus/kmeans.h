#ifndef KMEANS__H
#define KMEANS__H

#include "cluster.h"
#include "point.h"

/**
 * @brief The KMeans class implements the K-Mean clustering algorithm.
 */
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
    /** @brief KMeans the constructor of the class.
     *  @param K is the number of centers
     *  @param iterations is the number of iterations
     *  @param minK is the minimum number of centers
     * **/
    KMeans(int K, int iterations, int minK);
    /** @brief run, execution of the algorithm.
     *  @param all_points all samples from uniform distribution
     * **/
    void run(std::vector<Point> &all_points);
    /**
     * @brief getMeans
     * @return one vector from points, centers.
     */
    std::vector<Point> getMeans();
    /**
     * @brief pointsOfMeans
     * @return one double vector with points from all centers.
     */
    std::vector<std::vector<Point>> pointsOfMeans();
    /** @brief deleteMean, delete the center.
     *  @param idx the position of deleted center
     * **/
    void deleteMean(int idx);
    /** @brief setCenter.
     *  @param the nuber of centers
     * **/
    void setCenters(int centers);
    /**
     * @brief getSizes
     * @return the number of points of any center.
     */
    std::vector<int> getSizes();
};

#endif

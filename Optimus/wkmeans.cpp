#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "wkmeans.h"

WKMeans::WKMeans(int K, int iterations, int minK)
{
    this->K = K;
    this->iters = iterations;
    this->minK = minK;
    this->means.resize(K);
    this->allMeansWithPoints.resize(K);
}

int WKMeans::getNearestClusterId(Point &point)
{

    //ypothesi oti to proto einai mikroteri diadromi
    double sum = 0.0, min_dist;
    int NearestClusterId;

    for (int i = 0; i < dimensions; i++)
    {
        sum += pow(clusters[0].getCentroidByPos(i) - point.getVal(i), 2.0);
    }

    min_dist = sqrt(sum);
    NearestClusterId = clusters[0].getId();
    //euresi tis pio mikris diadromis
    for (int i = 1; i < K; i++)
    {
        double dist;
        sum = 0.0;

        for (int j = 0; j < dimensions; j++)
        {
            sum += pow(clusters[i].getCentroidByPos(j) - point.getVal(j), 2.0);
        }

        dist = sqrt(sum);

        if (dist < min_dist)
        {
            min_dist = dist;
            NearestClusterId = clusters[i].getId();
        }
    }
    return NearestClusterId;
}
double WKMeans::calculateWSum(std::vector<Point> points)
{
    double sum = 0.0,dist;

    for (int i = 0; i < K; i++)
    {
        dist = 0.0;
        for (int j = 0; j < points.size(); j++)
        {

            for (int d = 0; d < dimensions; d++)
            {
                dist += pow(clusters[i].getCentroidByPos(d) - points[j].getVal(d), 2.0);
            }
            sum += sqrt(dist);
        }
    }

    return sum;
}
int WKMeans::calculateWeight(Point &point, double wsum)
{
    point.setSizeOfWeights(K);
    std::vector <double> tmp;
    double sum,dist;
    for (int i = 0; i < K; i++)
    {
        dist=0.0;
        sum = 0.0;
        for (int j = 0; j < dimensions; j++)
        {
            sum += pow(clusters[i].getCentroidByPos(j) - point.getVal(j), 2.0);
        }
        dist = sqrt(sum);
        tmp.push_back(1.0 / (pow((dist / wsum), (1.0 / 2.03 -1.0))));
    }

    point.setWeights(tmp);

    return  std::max_element(tmp.begin(),tmp.end()) - tmp.begin();
}
void WKMeans::run(std::vector<Point> &all_points)
{

    total_points = all_points.size();
    dimensions = all_points[0].getDimensions();

    std::vector<int> used_pointIds;

    for (int i = 1; i <= K; i++)
    {

        while (true)
        {
            int index = rand() % total_points;

            if (std::find(used_pointIds.begin(), used_pointIds.end(), index) == used_pointIds.end())
            {
                used_pointIds.push_back(index);
                all_points[index].setCluster(i);
                Cluster cluster(i, all_points[index]);
                clusters.push_back(cluster);
                break;
            }
        }
    }


    int iter = 1;
    std::vector<double> tmpWeights;
    tmpWeights.resize(K);
    double wsum;
    while (true)
    {
        bool done = true;

        for (int i = 0; i < total_points; i++)
        {
            tmpWeights=all_points[i].getWeights();
            int currentClusterId = all_points[i].getCluster();
            int nearestClusterId = getNearestClusterId(all_points[i]);

            wsum = calculateWSum(all_points);
            int clusterPosition = calculateWeight(all_points[i], wsum);

            if (all_points[i].getWeights() != tmpWeights)
            {

                if (currentClusterId != -1)
                {
                    for (int j = 0; j < K; j++)
                    {
                        if (clusters[j].getId() == currentClusterId)
                        {
                            clusters[j].removePoint(all_points[i].getID());
                        }
                    }
                }

                for (int j = 0; j < K; j++)
                {
                    if (clusters[j].getId() == clusterPosition)
                    {
                        clusters[j].addPoint(all_points[i]);
                    }
                }
                all_points[i].setCluster(clusterPosition);

                done = false;
            }
        }

        for (int i = 0; i < K; i++)
        {
            int ClusterSize = clusters[i].getSize();

            for (int j = 0; j < dimensions; j++)
            {
                double sum = 0.0;
                if (ClusterSize > 0)
                {
                    for (int p = 0; p < ClusterSize; p++)
                        sum += clusters[i].getPoint(p).getVal(j);
                    clusters[i].setCentroidByPos(j, sum / ClusterSize);
                }
            }
        }
        //wsum = calculateWSum(all_points);
        //for (int i = 0; i < total_points; i++)
        //{
        //    calculateWeight(all_points[i], wsum);
        //}
        if (done || iter >= iters)
        {
            std::cout << "Clustering completed in iteration : " << iter << std::endl << std::endl;
            break;
        }
        iter++;
    }

    sizes.clear();
    sizes.resize(K);
    for (int i = 0; i < K; i++)
        sizes[i] = clusters[i].getSize();

    std::ofstream outfile;
    outfile.open("clusters.txt");

    if (outfile.is_open())
    {

        std::vector<double> tmp;
        for (int i = 0; i < K; i++)
        {
            //std::cout << "Cluster " << clusters[i].getId() << " centroid : ";
            for (int j = 0; j < dimensions; j++)
            {
                tmp.push_back(clusters[i].getCentroidByPos(j));
                //std::cout << clusters[i].getCentroidByPos(j) << " ";    //Output to console
                //outfile << clusters[i].getCentroidByPos(j) << " "; //Output to file
            }
            Point p(i, clusters[i].getId(), tmp);
            means.at(i) = p;
            tmp.clear();
            //std::cout << std::endl;
            //outfile << std::endl;
        }

        outfile.close();
    }
    else
    {
        //std::cout << "Error: Unable to write to clusters.txt";
    }
}

std::vector<Point> WKMeans::getMeans()
{

    return means;
}

std::vector<std::vector<Point>> WKMeans::getPointsOfAllMeans()
{
    allMeansWithPoints.clear();
    allMeansWithPoints.resize(K);
    std::vector<Point> pp;
    std::vector<double> ppp;
    for(int i=0; i<K; i++)
    {
        for(int j=0; j<clusters[i].getSize(); j++)
        {
            for (int e = 0; e < dimensions; e++)
            {
                ppp.push_back(clusters[i].getPoint(j).getVal(e));
            }
            Point p(clusters[i].getId(), ppp);
            pp.push_back(p);
            ppp.clear();
        }
        allMeansWithPoints[i] = pp;
        pp.clear();
    }
    return allMeansWithPoints;
}
std::vector<int> WKMeans::getSizes()
{

    return sizes;
}

void WKMeans::deleteMean(int idx)
{
    if (K >= this->minK)
    {
        K--;
        std::vector<Point> tmp = allMeansWithPoints.at(idx);
        means.erase(means.begin() + idx);
    }
}
void WKMeans::setCenters(int centers)
{
    this->K = centers;
}
std::vector<Point> WKMeans::getPointsOfMean(int pos)
{
    std::vector<Point> tmp;
    for(int j=0; j<clusters[pos].getSize(); j++)
        tmp.push_back(clusters[pos].getPoint(j));
    return tmp;
}

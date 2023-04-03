#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "kmeans.h"

KMeans::KMeans(int K, int iterations, int minK)
{
    this->K = K;
    this->iters = iterations;
    this->minK = minK;
    this->means.resize(K);
    this->PMeans.resize(K);
}

int KMeans::getNearestClusterId(Point point)
{
    double sum = 0.0, min_dist;
    int NearestClusterId;

    for (int i = 0; i < dimensions; i++)
    {
        sum += pow(clusters[0].getCentroidByPos(i) - point.getVal(i), 2.0);
    }

    min_dist = sqrt(sum);
    NearestClusterId = clusters[0].getId();

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

void KMeans::run(std::vector<Point> &all_points)
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
    while (true)
    {

        bool done = true;

        for (int i = 0; i < total_points; i++)
        {
            int currentClusterId = all_points[i].getCluster();
            int nearestClusterId = getNearestClusterId(all_points[i]);

            if (currentClusterId != nearestClusterId)
            {
                if (currentClusterId != 0)
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
                    if (clusters[j].getId() == nearestClusterId)
                    {
                        clusters[j].addPoint(all_points[i]);
                    }
                }
                all_points[i].setCluster(nearestClusterId);
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
                    clusters[i].setCentroidByPos(j, (double)sum / (double)ClusterSize);
                }
            }
        }

        if (done || iter >= iters)
        {
            //std::cout << "Clustering completed in iteration : " << iter << std::endl << std::endl;
            break;
        }
        iter++;
    }

    PMeans.clear();
    PMeans.resize(K);
    std::vector<Point> pp;
    std::vector<double> ppp;

    for (int i = 0; i < K; i++)
    {
        //std::cout << "Points in cluster " << clusters[i].getId() << " : ";
        for (int j = 0; j < clusters[j].getSize(); j++)
        {
            for (int e = 0; e < dimensions; e++)
            {
                ppp.push_back(clusters[i].getCentroidByPos(e));
            }
            Point p(clusters[i].getId(), ppp);
            pp.push_back(p);
            ppp.clear();
            //std::cout << clusters[i].getPoint(j).getID() << " ";
        }
        PMeans[i] = pp;
        pp.clear();
        //std::cout << std::endl << std::endl;
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

std::vector<Point> KMeans::getMeans()
{

    return means;
}

std::vector<std::vector<Point>> KMeans::pointsOfMeans()
{
    return PMeans;
}
std::vector<int> KMeans::getSizes()
{

    return sizes;
}

void KMeans::deleteMean(int idx)
{
    if (K >= this->minK)
    {
        K--;
        std::vector<Point> tmp = PMeans.at(idx);
        means.erase(means.begin() + idx);
    }
}
void KMeans::setCenters(int centers)
{
    this->K = centers;
}

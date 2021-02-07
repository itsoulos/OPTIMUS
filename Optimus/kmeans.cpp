#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "kmeans.h"

KMeans::KMeans(int K, int iterations)
{
    this->K = K;
    this->iters = iterations;
    this->means.clear();
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
    means.clear();
    total_points = all_points.size();
    dimensions = all_points[0].getDimensions();

    //Initializing Clusters
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
    //std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;

    //std::cout << "Running K-Means Clustering.." << std::endl;

    int iter = 1;
    while (true)
    {
        //std::cout << "Iter - " << iter << "/" << iters << std::endl;
        bool done = true;

        // Add all points to their nearest cluster
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

        // Recalculating the center of each cluster
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

        if (done || iter >= iters)
        {
            //std::cout << "Clustering completed in iteration : " << iter << std::endl << std::endl;
            break;
        }
        iter++;
    }

    //Print pointIds in each cluster
    for (int i = 0; i < K; i++)
    {
        //std::cout << "Points in cluster " << clusters[i].getId() << " : ";
        for (int j = 0; j < clusters[i].getSize(); j++)
        {
            //std::cout << clusters[i].getPoint(j).getID() << " ";
        }
        //std::cout << std::endl << std::endl;
    }
    //std::cout << "========================" << std::endl << std::endl;

    //Write cluster centers to file
    std::ofstream outfile;
    outfile.open("clusters.txt");

    if (outfile.is_open())
    {

        std::vector<double> tmp;
        for (int i = 0; i < K; i++)
        {
            //std::cout << "Cluster " << clusters[i].getId() << " centroid : ";
            //means.push_back(clusters[i].getPoint(i));
            for (int j = 0; j < dimensions; j++)
            {
                tmp.push_back(clusters[i].getCentroidByPos(j) );
                //std::cout << clusters[i].getCentroidByPos(j) << " ";    //Output to console
                //outfile << clusters[i].getCentroidByPos(j) << " "; //Output to file
            }
            //std::cout<<i<<std::endl;
            Point p(i, clusters[i].getId(), tmp);
            means.push_back(p);
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

std::vector<Point> KMeans::getMeans(){

    return means;
}

std::vector<std::vector<Point>> KMeans::pointsOfMeans()
{
    std::vector<std::vector<Point>> tmp;
    std::vector<int> idx;
    for (int i = 0; i < K; i++)
    {
        //std::cout << "Points in cluster " << clusters[i].getId() << " : \n";
        for (int j = 0; j < clusters[i].getSize(); j++)
        {
            //std::cout << clusters[i].getPoint(j).getID() << " ";
            idx.push_back(clusters[i].getPoint(j).getID());
        }
        //std::cout << std::endl << std::endl;
    }
    for (int i = 0; i < K; i++)
    {
        //for (int d=0;d<clusters[0].getPoint(0).getDimensions();d++)
        //{
        //    tmp[i].push_back(clusters[i].getPoint(j).getVal(d));
        //}
    }

    return PMeans;
}

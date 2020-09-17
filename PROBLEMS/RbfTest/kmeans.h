#ifndef KMEANS_H
#define KMEANS_H
# include <dataset.h>

class kmeans
{
private:
    /**
      dataset: To dataset me ta dedomena
      center : to kentro kathe omadas
      member : gia kathe protypo se poia omada einai
      team   : to plithos omadon
    */
    Dataset *dataset;
    QVector<Data> center;
    QVector<int>  member;
    int team;
    QVector<int> teamMembers;


    void initCenters();
    int  nearestTeam(Data &x);
    void updateCenters();
    double distance(Data &x,Data &y);
public:
    kmeans(Dataset *d,int nteams);
    void    runAlgorithm();
    QVector<Data> getCenters();
    Data          getVariances();
};

#endif // KMEANS_H

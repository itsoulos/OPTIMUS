#ifndef DATAPROBLEM_H
#define DATAPROBLEM_H

#include <Optimus/intervalproblem.h>
# include <Optimus/dataset.h>

class DataProblem : public IntervalProblem
{
private:
    QString trainName;
    QString testName;
    Dataset *trainSet;
    Dataset *testSet;
public:
    DataProblem(QString trname,QString tsname);
    QString getTrainName() const;
    QString getTestName() const;
    int getDimension() const;
    Dataset *getTrainSet();
    Dataset *getTestSet();
    ~DataProblem();
};

#endif // DATAPROBLEM_H

#ifndef DATAMODEL_H
#define DATAMODEL_H

# include <dataset.h>
class DataModel
{
protected:
    DataSet *trainSet,*testSet;
public:
    DataModel(DataSet *tr,DataSet *tt);
    virtual double  eval(Data &point)=0;
    virtual double  evalDeriv(Data &point,int pos)=0;
    virtual double  evalSecondDeriv(Data &point,int pos)=0;
    virtual double  getTrainError();
    virtual double  getTestError();
    virtual double  getClassError();
    virtual ~DataModel();
};

#endif // DATAMODEL_H

#include "datamodel.h"
# include <math.h>

DataModel::DataModel(DataSet *tr,DataSet *tt)
{
    trainSet = tr;
    testSet = tt;
}

double  DataModel::getTrainError()
{
    double sum  = 0.0;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xp = trainSet->getpoint(i);
        double val = eval(xp);
	if(isnan(val) || isinf(val) || fabs(val)>=1e+8) return 1e+100;
        double v = trainSet->gety(i);
        sum+= (val -v)*(val -v);
    }
    return sum;
}

double  DataModel::getTestError()
{
    double sum  = 0.0;
    for(int i=0;i<testSet->getpatterns();i++)
    {
        Data xp = testSet->getpoint(i);
        double val = eval(xp);
        double v = testSet->gety(i);
        sum+= (val -v)*(val -v);
    }
    return sum;
}

double  DataModel::getClassError()
{
    double sum  = 0.0;
    Data dclass;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
	    double y  = trainSet->gety(i);
	    bool found = false;
	    for(int j=0;j<dclass.size();j++)
	    {
		    if(fabs(dclass[j]-y)<1e-6) {found=true;break;}
	    }
	    if(!found) dclass.push_back(y);
    }
    for(int i=0;i<testSet->getpatterns();i++)
    {
        Data xp = testSet->getpoint(i);
        double val = eval(xp);
        double v = testSet->gety(i);
	double dmin =1e+100;
	int imin =-1;
	for(int j=0;j<dclass.size();j++)
	{
		if(fabs(dclass[j]-val)<dmin)
		{
			dmin = fabs(dclass[j]-val);
			imin = j;
		}

	}
       if(fabs(dclass[imin]-v)>1e-6) sum+=1.0;
    }
    return sum*100.0/testSet->getpatterns();
}

DataModel::~DataModel()
{

}

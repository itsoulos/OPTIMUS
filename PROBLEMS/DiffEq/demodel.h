#ifndef DEMODEL_H
#define DEMODEL_H
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <vector>
# include <string>
using namespace std;

typedef vector<double> Data;

class DeModel
{
public:
    DeModel();
    virtual double eval(Data &x)=0;
    virtual double evalDeriv(Data &x,int pos)=0;
    virtual double evalSecondDeriv(Data &x,int pos)=0;
    ~DeModel();
};

#endif // DEMODEL_H

#include "gdfmodel.h"
GdfModel::GdfModel(int d)
{
    dimension = d;
    program = new Cprogram (d);
}

double  GdfModel::eval(Data &x)
{

}

double  GdfModel::evalDeriv(Data &x,int pos)
{

}

double  GdfModel::evalSecondDeriv(Data &x,int pos)
{

}

GdfModel::~GdfModel()
{

}

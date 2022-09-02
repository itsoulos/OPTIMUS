#include "gdfmodel.h"
GdfModel::GdfModel(int d)
{
    dimension = d;
    cprogram = new Cprogram (d);
    program = new DeProgram();
    program->setStartSymbol(cprogram->getStartSymbol());
}

void    GdfModel::setChromosome(vector<int> &g)
{
    int redo_max= 0;
    string str=program->printRandomProgram(g,redo_max);
    if(redo_max>=REDO_MAX)
    {
        lastError = true;
        return;
    }
    lastExpr = str;
    if(!cprogram->Parse(str)) lastError = true;
    else lastError = false;
}

double  GdfModel::eval(Data &x)
{
    if(lastError) return 1e+10;
    double *xx=new double[dimension];
    for(int i=0;i<dimension;i++)xx[i]=x[i];
    double v= cprogram->Eval(xx);
    delete[]xx;
    if(cprogram->EvalError()!=0)
        return 1e+10;
    return v;
}

double  GdfModel::evalDeriv(Data &x,int pos)
{
    if(lastError) return 1e+10;

    double *xx=new double[dimension];
    for(int i=0;i<dimension;i++)xx[i]=x[i];
    double v= cprogram->EvalDeriv(xx,pos);
    delete[]xx;
    if(cprogram->EvalError()!=0)
        return 1e+10;
    return v;
}

double  GdfModel::evalSecondDeriv(Data &x,int pos)
{
    if(lastError) return 1e+10;

    double *xx=new double[dimension];
    for(int i=0;i<dimension;i++)xx[i]=x[i];
    double v= cprogram->EvalDeriv2(xx,pos);
    delete[]xx;
    if(cprogram->EvalError()!=0)
        return 1e+10;
    return v;
}

string  GdfModel::getLastExpr() const
{
    return lastExpr;
}

GdfModel::~GdfModel()
{

}

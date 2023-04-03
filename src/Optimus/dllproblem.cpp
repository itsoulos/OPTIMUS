#include "dllproblem.h"
# include <QDebug>

DllProblem::DllProblem(QString filename,QJsonObject settings)
    :IntervalProblem(1)
{
    library=new QLibrary(filename);
    library->load();
    Init=(INIT)library->resolve("init");
    if(Init==NULL)
        Init=(INIT)library->resolve("init_");

    init(settings);


    getdim=(GETDIMENSION)library->resolve("getdimension");
    if(getdim==NULL)
        getdim=(GETDIMENSION)library->resolve("getdimension_");
    int dim=(getdim!=NULL)?getdim():1;

    this->setDimension(dim);
    GetMargins=(GETMARGINS)library->resolve("getmargins");
    if(GetMargins==NULL)
        GetMargins=(GETMARGINS)library->resolve("getmargins_");
    GetMargins(margin);

    Funmin=(FUNMIN)library->resolve("funmin");
    if(Funmin==NULL)
        Funmin=(FUNMIN)library->resolve("funmin_");
    Granal=(GRANAL)library->resolve("granal");
    if(Granal==NULL)
        Granal=(GRANAL)library->resolve("granal_");
    Done=(DONE)library->resolve("done");
    if(Done==NULL)
        Done=(DONE)library->resolve("done_");

    Setp=(SETP)library->resolve("setParameter");
    if(Setp==NULL)
        Setp=(SETP)library->resolve("setParameter_");
    Sampler=(SAMPLER)library->resolve("getSample");
    if(Sampler==NULL)
        Sampler=(SAMPLER)library->resolve("getSample_");
}

void    DllProblem::init(QJsonObject x)
{
    if(Init!=NULL)
        Init(x);
}

double DllProblem::funmin(Data &x)
{
    return Funmin(x);
}

void    DllProblem::granal(Data &x, Data &g)
{
    Granal(x,g);
}


void DllProblem::setParameter(QString name, QVariant value)
{
    if(Setp!=NULL)
        Setp(name,value);
}


QJsonObject DllProblem::done(Data &x)
{
    if(Done!=NULL)
        return Done(x);
    return QJsonObject();
}


void DllProblem::getSample(Data &x)
{
    if(Sampler!=NULL)
        Sampler(x);
    else
        IntervalProblem::getSample(x);

}
DllProblem::~DllProblem()
{
    library->unload();
    delete library;
}

#include "optimizer.h"

Optimizer::Optimizer(Problem *p)
{
    myProblem=p;
}

void Optimizer::setProblem(Problem *p)
{
    myProblem=p;
}

void Optimizer::setSettings(QJsonObject settings)
{
    QStringList keys=settings.keys();
    for(int i=0;i<keys.size();i++)
    {
        if(params.contains(keys[i]))
            params[keys[i]]=settings[keys[i]].toString();
    }
}

void Optimizer::setThreads(int t)
{
    threads=t;
}

Problem         *Optimizer::getProblem()
{
    return myProblem;
}

void Optimizer::Solve()
{
    init();
    do
    {
        step();
    }while(!terminated());
    done();
}

void          Optimizer::addParameter(QString name,QString value,QString description)
{
    params[name]=value;
    params[name+"_desc"]=description;
}

QString        Optimizer::getParameterDescription(QString name)
{
    return params[name+"_desc"].toString();
}

QString        Optimizer::getParameter(QString name)
{
    return params[name].toString();
}

QJsonObject    Optimizer::getParameters()
{
    return params;
}

bool   Optimizer::terminated()
{
    return false;
}

void  Optimizer::step()
{

}
void  Optimizer::init()
{

}

void Optimizer::done()
{

}

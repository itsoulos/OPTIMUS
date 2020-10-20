#include "optimizer.h"

Optimizer::Optimizer(Problem *p)
{
    myProblem=p;
    addParameter("localsearch_method","bfgs","The desired local search method(bfgs,gradient,adam,lbfgs)");
    addParameter("bfgs_iterations","2001","The maximum number of iterations for BFGS");
    addParameter("gradient_rate","0.01","The learning rate for gradient descent");
    addParameter("gradient_iterations","10000","The maximum number of iterations for gradient descent");
    addParameter("adam_b1","0.9","The parameter b1 of Adam algorithm");
    addParameter("adam_b2","0.999","The parameter b2 of Adam algorithm");
    addParameter("adam_rate","0.01","The learning rate for Adam algorithm");
    addParameter("adam_iterations","10000","The maximum number of iterations for Adam algorithm");
}


double  Optimizer::localSearch(Data &x)
{
    QString method=params["localsearch_method"].toString();
    if(method=="bfgs")
    {
        Tolmin mt(myProblem);
        return mt.Solve(x,params["bfgs_iterations"].toString().toInt());
    }
    else
    if(method=="gradient")
    {
        GradientDescent des(myProblem);
        des.setIters(params["gradient_iterations"].toString().toInt());
        des.setRate(params["gradient_rate"].toString().toDouble());
        return des.Solve(x);
    }
    else
    if(method=="adam")
    {
        Adam at(myProblem);
        at.setB1(params["adam_b1"].toString().toDouble());
        at.setB2(params["adam_b2"].toString().toDouble());
        at.setLearningRate(params["adam_rate"].toString().toDouble());
        at.setIterations(params["adam_iterations"].toString().toInt());
        return at.Solve(x);
    }
    else
    if(method=="lbfgs")
    {
        Lbfgs lt(myProblem);
        return lt.Solve(x);
    }
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

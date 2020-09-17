# include <math.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <omp.h>
# include <interval.h>
#include <adept.h>
#include <armadillo>
#pragma GCC optimize("-Ofast")
using namespace std;

//-------------------------------------
// [0, pattern_dim)                   |
// [pattern_dim 2*pattern_dim)        |
// ..                                 |
// [(n-1)*pattern_dim n*pattern_dim)  |
// ^^ CENTROIDS ^^                    |
// [n*pattern_dim n*pattern_dim+n)    |
// ^^ VARIANCES ^^                    |
// n = nodes                          |
//-------------------------------------

// export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/nikolas/anaconda3/pkgs/hdf5-1.10.4-hb1b8bf9_0/lib/

extern "C"
{

QString trainName="xy.data";
QString testName="xy.data";
int nodes=10;
int dimension=0;
typedef vector<double> Data;
vector<Data> trainx;
Data trainy;
vector<Data> testx;
Data testy;
Data dclass;
double initialLeft=-1000.0;
double initialRight= 1000.0;
Interval maxWidth;
int failCount=0;
int normalTrain=0;

void loadTrain()
{
   QFile fp(trainName);
   fp.open(QIODevice::ReadOnly|QIODevice::Text);
   QTextStream st(&fp);
   st>>dimension;
   int count;
   st>>count;
   trainx.resize(count);
   trainy.resize(count);
   for(int i=0;i<count;i++)
   {
       trainx[i].resize(dimension);
       for(int j=0;j<dimension;j++)
       {
           st>>trainx[i][j];
       }
       st>>trainy[i];
       bool found=false;
       for(int j=0;j<dclass.size();j++)
       {
           if(fabs(dclass[j]-trainy[i])<1e-7)
           {
               found=true;
               break;
           }

       }
       if(!found)
           dclass.push_back(trainy[i]);
   }
   fp.close();
}


void setParameter(QString name,QVariant value)
{

    if(name=="normalTrain")
    {
        normalTrain=value.toInt();
    }
}

void loadTest()
{
    QFile fp(testName);
    fp.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream st(&fp);
    st>>dimension;
    int count;
    st>>count;
    testx.resize(count);
    testy.resize(count);
    for(int i=0;i<count;i++)
    {
        testx[i].resize(dimension);
        for(int j=0;j<dimension;j++)
        {
            st>>testx[i][j];
        }
        st>>testy[i];
    }
    fp.close();
}

void    init(QJsonObject data)
{
    if(data.contains("trainName"))
        trainName=data["trainName"].toString();
    if(data.contains("testName"))
        testName=data["testName"].toString();
    if(data.contains("nodes"))
        nodes=data["nodes"].toInt();
    if(data.contains("initialLeft"))
        initialLeft=data["initialLeft"].toDouble();
    if(data.contains("initialRight"))
        initialRight=data["initialRight"].toDouble();
    loadTrain();  
    if(testName!="xy.data") loadTest();
}

int	getdimension()
{
    return (dimension+1) * nodes;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(initialLeft,initialRight);
    }
}

int icount=0;
double neuronOutput( vector<double> &x, vector<double> &patt, unsigned pattDim, unsigned offset ){
    double out = 0;
    for(unsigned i = 0; i < pattDim;i++){
        out += (patt[i] - x[offset*pattDim + i]) * (patt[i] - x[offset*pattDim + i]);
    }
    double df=(-out / (x[nodes*pattDim+offset] * x[nodes*pattDim+offset]) );
    if(fabs(df)>100) return 1e+8;
    //if(fabs(df)>100)return 1.0;// return 1000;
    return exp(df);
}

arma::vec train( vector<double> &x ){
    arma::mat A = arma::zeros(trainx.size(),nodes);
    arma::vec B(trainy.size());
    for(unsigned i = 0; i < trainy.size(); i++){
        B.at(i) = trainy[i];
	icount = 0;
        for(unsigned j = 0; j < nodes;j++){
            A.at(i,j) = neuronOutput(x, trainx[i], trainx[0].size() , j);
        }
    }
    arma::vec RetVal= arma::vec(arma::pinv(A)*B);
    if(RetVal.has_nan() || RetVal.has_inf()) {
        RetVal = arma::zeros(arma::size(RetVal));
        }
    return RetVal;
}

double	funmin(vector<double> &x)
{
    double errorSum=0.0;
    
    arma::vec Linear = train(x);

    for(unsigned i = 0; i < trainx.size(); i++){
        Data pattern = trainx[i];
        arma::vec neuronOuts(nodes);
	icount=0;
        for(unsigned j = 0; j < nodes;j++){
            neuronOuts[j] = neuronOutput(x,pattern,pattern.size(),j);
        }
        double tempOut = arma::dot(neuronOuts,Linear);
        errorSum += ( tempOut - trainy[i] ) * ( tempOut - trainy[i] );
    }

    return errorSum;
}

adept::adouble aneuronOutput( vector<adept::adouble> &x, vector<double> &patt, unsigned pattDim, unsigned offset ){
    adept::adouble out = 0;
    for(unsigned i = 0; i < pattDim;i++){
        out += (patt[i] - x[offset*pattDim + i]) * (patt[i] - x[offset*pattDim + i]);
    }
    adept::adouble darg = out / (x[nodes*pattDim + offset] * x[nodes*pattDim + offset]);
    if(fabs(darg)>100) return 1e+8;
    return exp(-out / (x[nodes*pattDim + offset] * x[nodes*pattDim + offset]) );
}

adept::adouble afunmin( vector<adept::adouble> &x, vector<double> &x1 ){
    adept::adouble errorSum=0.0;
    
    arma::vec Linear = train(x1);

    for(unsigned i = 0; i < trainx.size(); i++){
        Data pattern = trainx[i];
        vector<adept::adouble> neuronOuts(nodes);
        for(unsigned j = 0; j < nodes;j++){
            neuronOuts[j] = aneuronOutput(x,pattern,pattern.size(),j);
        }
        adept::adouble tempOut = 0;
        for(unsigned j = 0; j < nodes; j++) tempOut+= neuronOuts[j]*Linear[j];
        errorSum += ( tempOut - trainy[i] ) * ( tempOut - trainy[i] );
    }

    return errorSum;
}

static double dmax(double a,double b){return a>b?a:b;}

void    granal2(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<x.size();i++)
            {
                    double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
                    x[i]+=eps;
                    double v1=funmin(x);
                    x[i]-=2.0 *eps;
                    double v2=funmin(x);
                    g[i]=(v1-v2)/(2.0 * eps);
                    x[i]+=eps;
            }

}

void    granal(vector<double> &x,vector<double> &g)
{
    g = vector<double>(x.size(),0.0);
    adept::Stack s;
    std::vector<adept::adouble> ax(g.size());
    for(unsigned i = 0; i < x.size(); i++) ax[i] = x[i];
    s.new_recording();

    adept::adouble error = afunmin(ax, x);

    error.set_gradient(1.0);

    s.compute_adjoint();

    for(unsigned i = 0; i < x.size();i++) {
        g[i] = ax[i].get_gradient();
    }
}


QString toString(Data &x)
{
    QString s="";
    for(int i=1;i<=nodes;i++)
    {
        QString inner="";
        for(int j=1;j<=dimension;j++)
        {
            QString isLeftParenthesis="(";
            QString isRightParenthesis=")";
            double node=x[(dimension+2)*i-(dimension+1)+j-1];
           /* if(node>=0)
            {
                isLeftParenthesis="";
                isRightParenthesis="";
            }*/
            if(fabs(node)<1e-7) continue;

            inner=inner+isLeftParenthesis+QString::number(node)+isRightParenthesis+"*x"+QString::number(j);
            if(j!=dimension) inner=inner+"+";
        }
        QString isLeftParenthesis="(";
        QString isRightParenthesis=")";
        double bias=x[(dimension+2)*i-(dimension+1)+i-1];
        /*if(bias>=0)
        {
            isLeftParenthesis="";
            isRightParenthesis="";
        }*/
        QString isPlus="+";
        if(inner.isEmpty()) continue;
        if(inner.endsWith("+")) isPlus="";

        if(fabs(bias)>1e-7)
            inner=inner+isPlus+isLeftParenthesis+QString::number(bias)+isRightParenthesis;
        double outer=x[(dimension+2)*i-(dimension+1)-1];
        if(fabs(outer)<1e-7) continue;
        isLeftParenthesis="(";
        isRightParenthesis=")";
       /* if(outer>=0)
        {
            isLeftParenthesis="";
            isRightParenthesis="";
        }*/
        s=s+isLeftParenthesis+QString::number(outer)+isRightParenthesis+"*sig("+inner+")";
        if(i!=nodes) s=s+"+";
    }
    if(s.endsWith("+"))
        s=s.remove(s.size()-1,1);
    return s;
}

double nearestClass(double y)
{
    int ifound=-1;
    double dmin=1e+100;
    for(int i=0;i<dclass.size();i++)
    {
        if(fabs(dclass[i]-y)<dmin)
        {
            dmin=fabs(dclass[i]-y);
            ifound=i;
        }
    }
    return dclass[ifound];
}
QJsonObject    done(Data &x)
{
    double sum=0.0;
    double per=0.0;
    double classError=0.0;
    int fcount=0;
    
    arma::vec Linear = train(x);
    for(int i=0;i<testx.size();i++)
    {
	       Data pattern = testx[i];
        arma::vec neuronOuts(nodes);
        for(unsigned j = 0; j < nodes;j++){
            neuronOuts[j] = neuronOutput(x,pattern,pattern.size(),j);
        }
        double tempOut = arma::dot(neuronOuts,Linear);

        per=tempOut-testy[i];
        classError+=fabs(testy[i]-nearestClass(tempOut))>1e-7;
        sum+=per * per;
    }
    QJsonObject result;
    result["nodes"]=nodes;
    result["testError"]=sum;
    result["classError"]=classError*100.0/testy.size();
    result["string"]=toString(x);
    return result;
}

}

# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <omp.h>
# include <adept.h>
# include <adept_arrays.h>
# include <omp.h>
using namespace std;
# define ADEPT_GRANAL
//adept::Stack stack;
//NNTEST EXAMPLE  // normal train != 1 grad
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
double initialLeft=-100.0;
double initialRight= 100.0;
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
    return (dimension+2) * nodes;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(initialLeft,initialRight);
    }
}
adept::adouble	adsig(adept::adouble x)
{
	return 1.0/(1.0+exp(-x));
}

double	dsig(double x)
{
    return 1.0/(1.0+exp(-x));
}

double	dgetValue(Data &node,Data &x,int &fcount)
{
	double arg=0.0;
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		arg=0.0;
        for(int j=1;j<=dimension;j++)
		{
            int pos=(dimension+2)*i-(dimension+1)+j-1;
			arg+=node[pos]*x[j-1];
		}
        arg+=node[(dimension+2)*i-1];
        fcount+=(fabs(arg)>=10.0);
        per+=node[(dimension+2)*i-(dimension+1)-1]*dsig(arg);
	}
	return per;
}

adept::adouble	adgetValue(adept::aVector &node,Data &x,int &fcount)
{
    adept::adouble arg=0.0;
    adept::adouble per=0.0;
    for(int i=1;i<=nodes;i++)
    {
        arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j-1;
            arg+=node[pos]*x[j-1];
        }
        arg+=node[(dimension+2)*i-1];
        fcount+=(fabs(arg)>=10.0);
        per+=node[(dimension+2)*i-(dimension+1)-1]*adsig(arg);
    }
    return per;
}

void	getDeriv(Data &node,Data &x,Data &G)
{
	double arg;
        double f,f2;
        for(int i=1;i<=nodes;i++)
        {
                arg = 0.0;
                for(int j=1;j<=dimension;j++)
                {
                        arg+=node[(dimension+2)*i-(dimension+1)+j-1]*x[j-1];
                }
                arg+=node[(dimension+2)*i-1];
                f=dsig(arg);
                f2=f*(1.0-f);
                G[(dimension+2)*i-1]=node[(dimension+2)*i-(dimension+1)-1]*f2;
                G[(dimension+2)*i-(dimension+1)-1]=f;
#pragma omp simd
                for(int k=1;k<=dimension;k++)
                {
                        G[(dimension+2)*i-(dimension+1)+k-1]=
                                x[k-1]*f2*node[(dimension+2)*i-(dimension+1)-1];
                }
	}
}

adept::adouble afunmin(adept::aVector &Weights);

void getOriginalGranal(Data &node,Data &g)
{
    for(int i=0;i<g.size();i++) g[i]=0.0;
    Data gtemp;
    gtemp.resize(g.size());
    int fcount=0;
    for(int i=0;i<trainx.size();i++)
	{
        getDeriv(node,trainx[i],gtemp);
        double per=dgetValue(node,trainx[i],fcount)-trainy[i];
#pragma omp simd
        for(int j=0;j<g.size();j++)	g[j]+=gtemp[j]*per;
	}
    for(int j=0;j<(dimension+2)*nodes;j++) g[j]*=2.0;
}

void	getGradient(Data &node,Data &g)
{
    adept::Stack stack;
    g.resize(node.size());
    for(int i=0;i<g.size();i++) g[i]=0.0;
    Data gtemp;
    gtemp.resize(g.size());
    int fcount=0;
    QJsonObject    done(Data &x);
#ifdef ADEPT_GRANAL 
    stack.continue_recording();

    adept::aVector w(node.size());
    for(unsigned i = 0; i < node.size();i++) w[i] = node[i];
    stack.new_recording();
    adept::adouble error;


    error = afunmin(w);
    error.set_gradient(1.0);
    stack.compute_adjoint();
    for(unsigned i = 0; i < node.size();i++) {
        g[i] = w[i].get_gradient();
        //std::cout << i << " " << g[i] << std::endl;
    }
    stack.pause_recording();
#else 
    
    for(int i=0;i<trainx.size();i++)
	{
        getDeriv(node,trainx[i],gtemp);
        double per=dgetValue(node,trainx[i],fcount)-trainy[i];
#pragma omp simd
        for(int j=0;j<g.size();j++)	g[j]+=gtemp[j]*per;
	}
    for(int j=0;j<(dimension+2)*nodes;j++) g[j]*=2.0;
#endif
}

adept::adouble afunmin(adept::aVector &Weights){
    adept::adouble sum=0.0;
    adept::adouble per=0.0;
    int fcount=0;
    Data A;
    //std::cout << Weights.size()<< std::endl;
    A.resize(Weights.size());
    for(unsigned i = 0; i < A.size();i++) {
        //std::cout << i << " " << Weights[i].value() << std::endl;
        A[i] = double(Weights[i].value());
    }
    //std::cout << Weights.size() << " " << trainx.size() << " " << trainy.size() << std::endl;
    for(int i=0;i<trainx.size();i++)
    {
        per=adgetValue(Weights,trainx[i],fcount)-trainy[i];
        sum+=per * per;
    }
    //std::cout << sum.value() << std::endl;
    if(normalTrain==1) return sum;
    return sum+100.0*pow(fcount*1.0/(nodes * trainx.size()),2.0);

}

double	funmin(vector<double> &x)
{
    double sum=0.0;
    double per=0.0;
    int fcount=0;
    for(int i=0;i<trainx.size();i++)
    {
        per=dgetValue(x,trainx[i],fcount)-trainy[i];
        sum+=per * per;
    }
    if(normalTrain==1) return sum;
    return sum+100.0*pow(fcount*1.0/(nodes * trainx.size()),2.0);
}

void    granal(vector<double> &x,vector<double> &g)
{
	if(normalTrain==1) getOriginalGranal(x,g);
	else
    getGradient(x,g);
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

QJsonObject    done(Data &x)
{
    double sum=0.0;
    double per=0.0;
    double classError=0.0;
    FILE *fp;
    if(omp_get_thread_num()==0)
	    fp=fopen("neural.plot","a");
    int fcount=0;
    for(int i=0;i<testx.size();i++)
    {
        double neuralOutput=dgetValue(x,testx[i],fcount);
        per=neuralOutput-testy[i];
        classError+=fabs(testy[i]-nearestClass(neuralOutput))>1e-7;
        sum+=per * per;
	if(omp_get_thread_num()==0)
		fprintf(fp,"%lf %lf %lf\n",testx[i][0],neuralOutput,testy[i]);
    }
    if(omp_get_thread_num()==0)
	    fclose(fp);
    QJsonObject result;
    result["nodes"]=nodes;
    result["testError"]=sum;
    result["classError"]=classError*100.0/testy.size();
    result["string"]=toString(x);
    return result;
}

}

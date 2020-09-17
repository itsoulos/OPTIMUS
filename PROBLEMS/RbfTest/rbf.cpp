#include "rbf.h"
# include <math.h>
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <adept.h>
#pragma GCC optimize("-Ofast")
Rbf::Rbf()
{
    nweights=1;
    pattern_dimension=1;
    centroid.resize(nweights);
    variance.resize(nweights);
    weight.resize(nweights);
   trainSet=NULL;
    testSet=NULL;
    alg=NULL;
}

void    Rbf::init_arrays()
{
    int i,j;
    centroid.resize(nweights);
    variance.resize(nweights);
    weight.resize(nweights);
    for(i=0;i<nweights;i++)
    {
        centroid[i].resize(pattern_dimension);
        for(j=0;j<pattern_dimension;j++)
            centroid[i][j]=0.0;
        variance[i]=0.0;
        weight[i]=2.0 * (rand()*1.0/RAND_MAX-1.0);
    }
}

void Rbf::setWeights(vector<double> &x)
{
    for(int i=0;i<weight.size();i++)
        weight[i]=x[i];
}

void Rbf::setWeights(Data &x)
{
    weight=x;
}

Data Rbf::getWeights()
{
    return weight;
}

void    Rbf::setNumberOfWeights(int K)
{
    if(K>0)    nweights=K;
}

int     Rbf::getNumberOfWeights()
{
    return nweights;
}

void    Rbf::setTrainSet(Dataset *t)
{
    int i;
    trainSet=t;
    pattern_dimension=t->getdimension();
    init_arrays();
    for(i=0;i<trainSet->getpatterns();i++)
    {
        double y=trainSet->gety(i);
        if(classVector.indexOf(y)==-1)
            classVector<<y;
    }
}

void    Rbf::setTestSet(Dataset *t)
{
    if(trainSet==NULL) return;
    if(t->getdimension()==trainSet->getdimension()) testSet=t;
}

double  distance(Data &x, Data &y)
{
    double s=0.0;
    int i;
    for(i=0;i<x.size();i++)
        s+=pow(x[i]-y[i],2.0);
    return sqrt(s);
}

void	Rbf::setParameters(vector<double> &x)
{
	int icount=0;
    int d=trainSet->getdimension();
  for(int i=0;i<centroid.size();i++)
	{
	
        for(int j=0;j<d;j++)
        {
			centroid[i][j]=x[icount++];
        }

		variance[i]=x[icount++];
	}
}

double  Rbf::gauss_function(Data &pattern,Data &center,double sigma)
{
    double p=distance(pattern,center);
    return exp(-p*p/(sigma * sigma));
}

Matrix  Rbf::matrix_transpose(Matrix &x)
{
    Matrix xx;
    xx.resize(x[0].size());
    int i,j;
    for(i=0;i<xx.size();i++)
    {
        xx[i].resize(x.size());
        for(j=0;j<x.size();j++)
        {
            xx[i][j]=x[j][i];
        }
    }
    return xx;
}

Matrix  Rbf::matrix_mult(Matrix &x,Matrix &y)
{
    int m=x.size();
    int p=x[0].size();
    int n=y[0].size();
    if(p!=y.size())
    {
        printf("Impossible to multiple \n");

    }
    else
    {
        Matrix z;
        z.resize(m);
        int i,j,k;
        for(i=0;i<m;i++) z[i].resize(n);
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                z[i][j]=0.0;
                for(k=0;k<p;k++)
                {
                    z[i][j]=z[i][j]+x[i][k]*y[k][j];
                }
            }
        }
        return z;
    }
}

Matrix  Rbf::matrix_inverse(Matrix x)
{
    Matrix c=x;
    int npivot;
    double det;
    int pass, row, col, maxrow, i, j, error_flag;
    double temp, pivot, mult;
    int n=x.size();
     for(i=0; i<n; ++i) {
        for(j=0; j<n; ++j) {
            if(i==j) {
            c[i][j] = 1.0;
            } else {
            c[i][j] = 0.0;
              }
        }
       }

       det=1.0;
       npivot=0;


       for(pass=0; pass<n; ++pass) {
        maxrow=pass;
        for(row=pass; row<n; ++row)
            if(fabs(x[row][pass]) > fabs(x[maxrow][pass]))
            maxrow=row;

        if(maxrow != pass)
            ++npivot;

        for(col=0; col<n; ++col) {
            temp=x[pass][col];
            x[pass][col] = x[maxrow][col];
            x[maxrow][col] = temp;
            temp = c[pass][col];
            c[pass][col] = c[maxrow][col];
            c[maxrow][col] = temp;
        }


        pivot = x[pass][pass];
        det *= pivot;

        if(fabs(det) < 1.0e-40) {
            //printf("Matrix is singular\n");
            return c;
        }


        for(col=0; col<n; ++col) {
            x[pass][col] = x[pass][col]/pivot;
            c[pass][col] = c[pass][col]/pivot;
        }

        for(row=0; row<n; ++row) {
            if(row != pass) {
                mult = x[row][pass];
                for(col=0; col<n; ++col) {
                    x[row][col] = x[row][col] - x[pass][col] * mult;
                    c[row][col] = c[row][col] - c[pass][col] * mult;
                }
            }
        }

       }

       if(npivot % 2 != 0)
        det = det * (-1.0);
      return c;
}

Matrix  Rbf::matrix_pseudoinverse(Matrix &a)
{
    Matrix b=matrix_transpose(a);
    Matrix e=matrix_mult(b,a);
    Matrix d=matrix_inverse(e);
    Matrix c=matrix_mult(d,b);
    return c;
}

void    Rbf::train()
{
    //phase1
    /*
    if(alg==NULL)
    {
	    alg=new kmeans(trainSet,nweights);
		alg->runAlgorithm();
		centroid=alg->getCenters();
		variance=alg->getVariances();
    }*/

    //phase2
    int i,j;
    if(A.size()==0)
    {
	    A.resize(trainSet->getpatterns());
    		RealOutput.resize(trainSet->getpatterns());

    	for(i=0;i<A.size();i++)
    	{
		RealOutput[i].resize(1);
        	A[i].resize(centroid.size());
    	}
    }

    for(i=0;i<A.size();i++)
    {
        RealOutput[i][0]=trainSet->gety(i);
        Data x=trainSet->getpoint(i);
        for(j=0;j<centroid.size();j++)
        {
          A[i][j]=gauss_function(x,centroid[j],variance[j]);

        }
    }

    Matrix pA=matrix_pseudoinverse(A);
    Matrix pW=matrix_mult(pA,RealOutput);
    for(i=0;i<pW.size();i++)
        weight[i]=pW[i][0];

}

double  Rbf::getOutput(Data &pattern)
{
    Data px;
    int j;
    for(j=0;j<centroid.size();j++)
      px<<gauss_function(pattern,centroid[j],variance[j]);
    return product(weight,px);
}
double  Rbf::getOutput(Data &w,Data &pattern)
{
    Data px;
    int j;
    for(j=0;j<centroid.size();j++)
      px<<gauss_function(pattern,centroid[j],variance[j]);
    return product(w,px);
}

double  Rbf::getClass(Data &pattern)
{
    double f=getOutput(pattern);
    int minClass=0;
    double minDist=fabs(classVector[0]-f);
    int i;
    for(i=0;i<classVector.size();i++)
    {
        double dist=fabs(classVector[i]-f);
        if(dist<minDist)
        {
            minClass=i;
            minDist=dist;
        }
    }
    return classVector[minClass];
}

double	Rbf::getTrainError(Data &w)
{
    if(trainSet==NULL) return -1.0;
    double sum=0.0;
    int i;
    for(i=0;i<trainSet->getpatterns();i++)
    {
        Data x=trainSet->getpoint(i);
        sum+=pow(getOutput(w,x)-trainSet->gety(i),2.0);
    }
    return sum;
}

double  Rbf::getTrainError()
{
    if(trainSet==NULL) return -1.0;
    double sum=0.0;
    int i;
    for(i=0;i<trainSet->getpatterns();i++)
    {
        Data x=trainSet->getpoint(i);
        sum+=pow(getOutput(x)-trainSet->gety(i),2.0);
    }
    return sum;
}

double  Rbf::product(Data &x, Data &y)
{
    int i;
    double sum=0.0;
#pragma omp simd reduction(+:sum)
    for(i=0;i<x.size();i++)
        sum+=x[i]*y[i];
    return sum;
}

double  Rbf::getTestError()
{
    if(testSet==NULL) return -1.0;
    double sum=0.0;
    int i;
    for(i=0;i<testSet->getpatterns();i++)
    {
        Data x=testSet->getpoint(i);
        sum+=pow(getOutput(x)-testSet->gety(i),2.0);
    }
    return sum;
}

double  Rbf::getClassError()
{
    if(testSet==NULL) return -1.0;
    int missed=0;
    int i;
    for(i=0;i<testSet->getpatterns();i++)
    {
        Data pattern=testSet->getpoint(i);

        double d=getClass(pattern);
        double y=testSet->gety(i);
        if(fabs(d-y)>1e-5) missed++;
    }
    return missed * 100.0/testSet->getpatterns();
}

adept::adouble agauss_function(Data &pattern,QVector<adept::adouble> &center,adept::adouble sigma)
{
    adept::adouble p=0.0;// distance(pattern,center);
    for(unsigned i = 0; i < pattern.size();i++) p+= (pattern[i]-center[i])*(pattern[i]-center[i]);
    return exp(-p/(sigma * sigma));
}
/*
double  Rbf::getOutput(Data &pattern)
{
    Data px;
    int j;
    for(j=0;j<centroid.size();j++)
      px<<gauss_function(pattern,centroid[j],variance[j]);
    return product(weight,px);
}*/

adept::adouble Rbf::getTrainError(QVector<adept::adouble> &x){
	
	
	QVector<QVector<adept::adouble>> acentroid;
	QVector<adept::adouble> avariance;
	avariance.resize(variance.size());
	acentroid.resize(centroid.size());
	int icount=0;
	for(unsigned i = 0; i < centroid.size();i++){
		acentroid[i].resize(centroid[i].size());
		for(unsigned j = 0; j < centroid[i].size();j++){
			acentroid[i][j]=x[icount++];
		}
		avariance[i]=x[icount++];
	}
	
	adept::adouble sum=0.0;
    int i;
    for(i=0;i<trainSet->getpatterns();i++)
    {
        Data x=trainSet->getpoint(i);
        adept::adouble tempOut=0.0;
        QVector<adept::adouble> temppx(centroid.size());
        for(unsigned j=0;j<centroid.size();j++)
            temppx[j]=agauss_function(x,acentroid[j],avariance[j]);
		for(unsigned j = 0; j < temppx.size();j++) tempOut += weight[j] * temppx[j];
		sum+=pow(tempOut-trainSet->gety(i),2.0);
        //sum+=pow(getOutput(x)-trainSet->gety(i),2.0);
    }
    return sum;
	
}


Rbf::~Rbf()
{
	if(alg!=NULL) delete alg;
}

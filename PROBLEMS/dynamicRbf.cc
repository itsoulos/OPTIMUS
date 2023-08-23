# include <math.h>
# include <stdlib.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <omp.h>
# include <interval.h>
# include <adept_arrays.h>

#include <adept.h>
#include <armadillo>
#pragma GCC optimize("-Ofast")
using namespace std;
//# define KMEANS
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

typedef vector<double> Data;

/* ---------------------------------------------------- */
/* Matrix Multiplication                                */
/* Computes the product:  [C] = [A][B]                  */
/* Input Parameters:                                    */
/*      a : pointer to matrix [A] (m rows, p columns)   */
/*      b : pointer to matrix [B] (p rows, n columns)   */
/*      c : pointer to matrix [C] (result of [A][B])    */
/*      m : rows of [A] and [C]                         */
/*      p : columns of [A] and rows of [B]              */
/*      n : columns of [B] and [C]                      */
/* ---------------------------------------------------- */


void matrix_mult(double * a, double * b, double * c,
                 int m, int p, int n)
{
   int i,j,k;

   for(i=0; i<m; ++i)
    for(j=0; j<n; ++j) {
        c[i*n + j]=0.0;
        for(k=0; k<p; ++k)
            c[i*n + j] = c[i*n +j] + a[i*p +k] * b[k*n +j];
    }

}



/* ---------------------------------------------------- */
/* Transpose Matrix                                     */
/* Computes the transpose of matrix [A]                 */
/* The result matrix is [C]                             */
/* Input Parameters:                                    */
/*      a : pointer to matrix [A] (m rows, n columns)   */
/*      c : pointer to matrix [C] (transpose of [A])    */
/*      m : rows of [A] and columns of [C]              */
/*      n : columns of [A] and rows of [C]              */
/* ---------------------------------------------------- */


void matrix_transpose(double * a, double * c, int m, int n)
{
   int i,j;

   for(i=0; i<m; ++i)
    for(j=0; j<n; ++j)
        c[j*m + i] = a[i*n + j];
}



/* ---------------------------------------------------- */
/* Inverse Matrix                                       */
/* Computes the inverse of matrix [A]                   */
/* The result matrix is [C]                             */
/* Input Parameters:                                    */
/*      a   : pointer to matrix [A] (n rows, n columns) */
/*      c   : pointer to matrix [C] (inverse of [A])    */
/*      n   : order of matrix (n x n)                   */
/*      det : determinant of [A]                        */
/* Returns:                                             */
/*      0   : if successful                             */
/*      1   : if matrix is singular                     */
/* ---------------------------------------------------- */


int matrix_inverse(double * a, double * c, int n, double *det)
{
   int npivot; /* no of times rows are interchanged */
   int pass, row, col, maxrow, i, j, error_flag;
   double temp, pivot, mult;

   /* Initialization */
   /* store the identity matrix */
   /* it will be replaced by the inverse */
   for(i=0; i<n; ++i) {
    for(j=0; j<n; ++j) {
        if(i==j) {
        c[i*n + j] = 1.0;
        } else {
        c[i*n + j] = 0.0;
          }
    }
   }

   *det=1.0;
   npivot=0;

   /* Partial pivoting
      For each pass find the largest element in the column
      containing the pivot element. The pivot column is pass.
      maxrow is the row containing the largest element in
      the pivot column. It is initially set equal to the
      pivot row. */

   for(pass=0; pass<n; ++pass) {
    maxrow=pass;
    for(row=pass; row<n; ++row)
        if(fabs(a[row*n + pass]) > fabs(a[maxrow*n + pass]))
        maxrow=row;

    /* increment npivot if rows are to be interchanged */
    if(maxrow != pass)
        ++npivot;

    /* Interchange pivot row with the row containing the largest
           element in the pivot column in both [A] and [C] */
    for(col=0; col<n; ++col) {
        temp=a[pass*n + col];
        a[pass*n + col] = a[maxrow*n + col];
        a[maxrow*n + col] = temp;
        temp = c[pass*n + col];
        c[pass*n + col] = c[maxrow*n + col];
        c[maxrow*n + col] = temp;
    }

    /* Compute determinant and check for singularity.
       Determinant is the product of the pivot elements.
       Pivot element is a[pass][pass] */
    pivot = a[pass*n + pass];
    *det *= pivot;


    if(fabs(*det) < 1.0e-40) {
        /* Matrix is singular. Set error flag and exit */
        error_flag=1;
        return(error_flag);
    }


    /* Normalization.
       Divide pivot row by the pivot element so pivot element
       is reduced to 1 */
    for(col=0; col<n; ++col) {
        a[pass*n + col] = a[pass*n + col]/pivot;
        c[pass*n + col] = c[pass*n + col]/pivot;
    }

    /* Elimination.
       Reduce all elements in pivot column to zero */
    for(row=0; row<n; ++row) {
        if(row != pass) {
            mult = a[row*n + pass];
            for(col=0; col<n; ++col) {
                a[row*n + col] = a[row*n + col] - a[pass*n + col] * mult;
                c[row*n + col] = c[row*n + col] - c[pass*n + col] * mult;
            }
        }
    }

   }

   if(npivot % 2 != 0)
    *det = *det * (-1.0);
    error_flag = 0;
    return(error_flag);

}



/* ---------------------------------------------------- */
/* Pseudo Inverse Matrix                                */
/* Computes the pseudo inverse matrix of [A]            */
/* The result is matrix [C] = ([A]^T * [A])^-1 * [A]^T  */
/* Input Parameters:                                    */
/*      a : pointer to matrix [A] (m rows, n columns)   */
/*      c : pointer to matrix [C] (n rows, m columns)   */
/*      m : rows of [A] and columns of [C]              */
/*      n : columns of [A] and rows of [C]              */
/* ---------------------------------------------------- */


void matrix_pseudo_inverse(double * a, double * c, int m, int n)
{
   double * b = (double*)malloc(sizeof(double)*n*m);   /* transpose of [a] */
   //double * e = (double*)malloc(sizeof(double)*m*n);   /* product [b][a] */
  double * e = (double*)malloc(sizeof(double)*n*n);   /* product [b][a] */
  // double * e = (double*)malloc(sizeof(double)*m*m);   /* product [b][a] */
   double * d = (double*)malloc(sizeof(double)*n*n);   /* inverse of [c] */
   double det;

   /*   [b] = [a]^T    */
   /*   [e] = [b][a]   */
   /*   [d] = [e]^-1   */
   /*   [c] = [d][b]   */

   matrix_transpose(a,b,m,n);
   matrix_mult(b,a,e,n,m,n);
   matrix_inverse(e,d,n,&det);
   matrix_mult(d,b,c,n,n,m);

   free(d);
   free(e);
   free(b);

}




/* ---------------------------------------------------- */
/* Matrix Addition                                      */
/* Computes the sum of matrices:  [C] = [A] + [B]       */
/* Input Parameters:                                    */
/*      a : pointer to matrix [A] (m rows, n columns)   */
/*      b : pointer to matrix [B] (m rows, n columns)   */
/*      c : pointer to matrix [C] (m rows, n columns)   */
/*      m : rows of [A], [B] and [C]                    */
/*      b : columns of [A], [B] and [C]                 */
/* ---------------------------------------------------- */


void matrix_add(double * a, double * b, double * c, int m, int n)
{
   int i,j;

   for(i=0; i<m; ++i)            /* loop over rows    */
    for(j=0; j<n; ++j)       /* loop over columns */
        c[n*i + j] = a[n*i +j] + b[n*i + j];

}




/* ---------------------------------------------------- */
/* Matrix Substraction                                  */
/* Computes the difference of matrices: [C] = [A] - [B] */
/* Input Parameters:                                    */
/*      a : pointer to matrix [A] (m rows, n columns)   */
/*      b : pointer to matrix [B] (m rows, n columns)   */
/*      c : pointer to matrix [C] (m rows, n columns)   */
/*      m : rows of [A], [B] and [C]                    */
/*      b : columns of [A], [B] and [C]                 */
/* ---------------------------------------------------- */


void matrix_sub(double * a, double * b, double * c, int m, int n)
{
   int i,j;

   for(i=0; i<m; ++i)            /* loop over rows    */
    for(j=0; j<n; ++j)       /* loop over columns */
        c[n*i + j] = a[n*i +j] - b[n*i + j];

}
/* -------------------------------------------- */
/* Trains an RBF Neural Netowrk                 */
/* Input Parameters:                            */
/*      in_n      : number of inputs            */
/*                  (in the input layer)        */
/*      hid_n     : number of synapses          */
/*                  in the hidden layer         */
/*      out_n     : number of outputs           */
/*                  (in the output layer)       */
/*      samples_n : number of training samples  */
/*      centers   : pointer to centers matrix   */
/*                  (hid_n rows, in_n columns)  */
/*      variances : pointer to variances matrix */
/*                  (hid_n rows, in_n columns)  */
/*      weights   : pointer to weights matrix   */
/*                  (hid_n columns)             */
/*      input     : pointer to input vector     */
/*                  (in_n columns)              */
/*      output    : pointer to output vector    */
/*                  (out_n columns)             */
/* -------------------------------------------- */

double violate;
int train_rbf(int in_n, int hid_n, int out_n, int samples_n,
          double * centers, double * variances,
          double * weights, double * input, double * output)
{

   int i,j;
   double result, var_diag=0.05;
   double in_cen[in_n], in_cen_tr[in_n];
   double G[samples_n][hid_n];
   double Gp[hid_n][samples_n];

   // -----------------------------------
    var_diag=0;
    for(i=0; i<hid_n; i++) {
        for(j=0; j<in_n; j++) {
            var_diag += variances[i*in_n + j];
        }
    }
    if(var_diag<0.00000001) var_diag=0.001;
   // -----------------------------------

    int violcount=0;
   for(i=0; i<samples_n; i++) {
    for(j=0; j<hid_n; j++) {
        /* Ypologizw ton pinaka Input-Centers gia kathe hidden neuron */
        matrix_sub(&input[i*in_n],&centers[j*in_n],&in_cen[0],1,in_n);

        /* Ypologizw ton anastrofo pinaka tou prohgoumenou */
        matrix_transpose(&in_cen[0],&in_cen_tr[0],in_n,1);

        /* Pollaplasiazw ta dyo parapanw gia na parw ena pinaka stoixeio */
        matrix_mult(&in_cen_tr[0],&in_cen[0],&result,1,in_n,1);


        G[i][j] = exp((-1.0*result)/(2.0 * var_diag));


    }
   }
   matrix_pseudo_inverse(&G[0][0],&Gp[0][0],samples_n,hid_n);

   /* Kanw ton pol/smo [Gp]*[Output] gia na brw ta weights */

   matrix_mult(&Gp[0][0],output,weights,hid_n,samples_n,out_n);
 return 0;
}



/* -------------------------------------------- */
/* Creates an RBF Neural Netowrk                */
/* Input Parameters:                            */
/*      in_n      : number of inputs            */
/*                  (in the input layer)        */
/*      hid_n     : number of synapses          */
/*                  in the hidden layer         */
/*      out_n     : number of outputs           */
/*                  (in the output layer)       */
/*      centers   : pointer to centers matrix   */
/*                  (hid_n rows, in_n columns)  */
/*      variances : pointer to variances matrix */
/*                  (hid_n rows, in_n columns)  */
/*      weights   : pointer to weights matrix   */
/*                  (hid_n rows, out_n columns) */
/*      input     : pointer to input vector     */
/*                  (1 row, in_n columns)       */
/*      output    : pointer to output vector    */
/*                  (1 row, out_n columns)      */
/* -------------------------------------------- */

int create_rbf(int in_n, int hid_n, int out_n,
          double * centers, double * variances,
          double * weights, double * input, double * output)
{

   int i,j;
   double result, var_diag=0.05;
   double in_cen[in_n], in_cen_tr[in_n];
   double G[hid_n];

   // -----------------------------------
    var_diag=0;
    for(i=0; i<hid_n; i++) {
        for(j=0; j<in_n; j++) {
            var_diag += variances[i*in_n + j];
        }
    }
    if(var_diag<0.00000001) var_diag=0.001;
   // -----------------------------------

   for(i=0; i<hid_n; i++) {
    /* Ypologizw ton pinaka Input-Centers gia kathe hidden neuron */
    matrix_sub(&input[0],&centers[i*in_n],&in_cen[0],1,in_n);

    /* Ypologizw ton anastrofo pinaka tou prohgoumenou */
    matrix_transpose(&in_cen[0],&in_cen_tr[0],in_n,1);

    /* Pollaplasiazw ta dyo parapanw gia na parw ena pinaka stoixeio */
    matrix_mult(&in_cen_tr[0],&in_cen[0],&result,1,in_n,1);

    G[i] = exp((-1.0*result)/(2.0 * var_diag));

    /* Auto einai gia bias = 1 */
    /* G[hid_n-1]=1; */
   }

   /* Kanw ton pol/smo [G]*[Weights] gia na brw tin exodo */

   matrix_mult(&G[0],weights,output,1,hid_n,out_n);

 return 0;
}

QString trainName="xy.data";
QString testName="xy.data";
int nodes=10;
int dimension=0;
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

vector<int> num_of_cluster_members;
vector<double> xleft,xright;
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
   xleft.resize(dimension);
   xright.resize(dimension);

   for(int i=0;i<count;i++)
   {
       trainx[i].resize(dimension);
       for(int j=0;j<dimension;j++)
       {
              st>>trainx[i][j];
           if(i==0 || trainx[i][j]<xleft[j]) xleft[j]=trainx[i][j];
           if(i==0 || trainx[i][j]>xright[j]) xright[j]=trainx[i][j];

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

double  getDistance(Data &x,Data &y)
{
    double sum = 0.0;
    for(int i=0;i<x.size();i++)
        sum+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum/x.size());
}
void runKmeans(vector<Data> &point,int K,
               vector<Data> &center,
               vector<Data> &variances,
               vector<int> &teamElements)
{
    int pop = point.size();
    center.resize(K);
    variances.resize(K);
    vector<Data> copyCenter;
    copyCenter.resize(K);

    vector<int> belong;
    belong.resize(pop);
    teamElements.resize(K);
    int dim = point[0].size();
 for(int i=0;i<K;i++)
 {

     teamElements[i]=0;
 }

    for(int i=0;i<pop;i++)
    {
        belong[i]=rand() % K;
        teamElements[belong[i]]++;
    }

    for(int i=0;i<K;i++)
    {

        center[i].resize(dim);
        variances[i].resize(dim);
        for(int j=0;j<dim;j++)
        {
            center[i][j]=0.0;
            variances[i][j]=0.0;
        }
    }
    for(int j=0;j<point.size();j++)
        {
         for(int k=0;k<dim;k++)
         {
             center[belong[j]][k]+=point[j][k];
         }
     }
    for(int i=0;i<K;i++)
    {
        for(int j=0;j<dim;j++)
            center[i][j]/=teamElements[i]>1?teamElements[i]:1;
    }


    int iteration = 1;
    double oldDist = 1e+100;
    while(true)
    {
        copyCenter = center;
        for(int i=0;i<K;i++) teamElements[i]=0;

        for(int i=0;i<point.size();i++)
        {
            int minCenterIndex = -1;
            double minCenterDist = 1e+100;
            for(int j=0;j<K;j++)
            {
                double d = getDistance(point[i],center[j]);
                if(d<minCenterDist)
                {
                    minCenterDist = d;
                    minCenterIndex = j;
                }
            }
            belong[i]=minCenterIndex;
            teamElements[minCenterIndex]++;

        }

        for(int i=0;i<K;i++)
        {
           for(int j=0;j<dim;j++)
               center[i][j]=0.0;
        }
           for(int j=0;j<point.size();j++)
           {
            for(int k=0;k<dim;k++)
            {
                center[belong[j]][k]+=point[j][k];
            }
           }

           for(int i=0;i<K;i++)
            for(int k=0;k<dim;k++)
               center[i][k]/=teamElements[i]>1?teamElements[i]:1;

        double totalDistance = 0.0;
        for(int i=0;i<K;i++)
        {
            totalDistance+=getDistance(center[i],copyCenter[i]);
        }
        if(totalDistance<1e-6) break;
        if(iteration>1 && fabs(totalDistance-oldDist)<1e-8) break;
        iteration++;
        oldDist = totalDistance;
    }

    for(int i=0;i<point.size();i++)
    {
        for(int j=0;j<dim;j++)
        {
            variances[belong[i]][j]+=pow(point[i][j]-center[belong[i]][j],2.0);
        }
    }

    for(int i=0; i<K; i++) {
        for(int j=0; j<dim; j++) {

            if(teamElements[i]>1)
            variances[i][j] /= (double)teamElements[i];
            else
            {
                variances[i][j]=0.01;
            }
        }
    }
}
vector<Data> centerValues;
vector<Data> varianceValues;
bool haveInitialized = false;
void    init(QJsonObject data)
{
    if(trainx.size()!=0) return;
    if(data.contains("trainName"))
        trainName=data["trainName"].toString();
    if(data.contains("testName"))
        testName=data["testName"].toString();
    if(data.contains("nodes"))
        nodes=data["nodes"].toString().toInt();
    if(data.contains("initialLeft"))
        initialLeft=data["initialLeft"].toDouble();
    if(data.contains("initialRight"))
        initialRight=data["initialRight"].toDouble();
    loadTrain();  
    if(testName!="xy.data") loadTest();
}


int	getdimension()
{

    if(!haveInitialized)
    {
        haveInitialized = true;
        vector<int> teamElements;


        runKmeans(trainx,nodes,
                       centerValues,
                       varianceValues,
                       teamElements);

        for(int i=0;i<nodes;i++)
        {
            double pc  = teamElements[i]*1.0/trainx.size();
            printf("PC[%d]=%lf\n",i,pc*100.0);
            double sumVariance = 0.0;
            for(int j=0;j<varianceValues[i].size();j++)
                sumVariance+=varianceValues[i][j]*varianceValues[i][j];
            sumVariance = sqrt(sumVariance/varianceValues[i].size());

        }
        bool termFlag = false;
        while(!termFlag && centerValues.size()>2)
        {
            termFlag = true;
            for(int i=0;i<centerValues.size();i++)
            {
                double minDist = 1e+100;
                int    imin    = -1;
                for(int j=0;j<i;j++)
                {

                    double d = getDistance(centerValues[i],centerValues[j]);
                    if(d<minDist)
                    {
                        minDist = d;
                        imin = j;
                    }
                }
                //check1

                double pc  = teamElements[i]*1.0/trainx.size();
                bool toDelete = false;
                double limit = 1.0/nodes;
                if(pc<limit/4.0)
                {
                    toDelete = true;
                }
                if(!toDelete && minDist<1e-6)
                {
                    toDelete = true;
                }
                if(toDelete)
                {
                    termFlag = false;
                    for(int j=i;j<centerValues.size()-1;j++)
                    {
                        centerValues[j]=centerValues[j+1];
                        varianceValues[j]=varianceValues[j+1];
                        teamElements[j]=teamElements[j+1];
                    }
                    centerValues.pop_back();
                    varianceValues.pop_back();
                    teamElements.pop_back();
                   break;
                }


            }
        }
        nodes = centerValues.size();
    }
    return (dimension+1) * nodes;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(initialLeft,initialRight);
    }
	
    if(varianceValues.size()!=0)
    {
            int icount=0;
	    	double f=3.0;
            for(int i=0;i<nodes;i++)
            {
                for(int j=0;j<trainx[0].size();j++)
                {

                    double cx=fabs(centerValues[i][j]);
                   // if(fabs(cx)<5.0) cx=5.0;
		    //x[icount++]=Interval(-f * fabs(cx),f*fabs(cx));
		    //x[icount++]=Interval(-f * fabs(cx),f*fabs(cx));
		    //if(cx<1e+2) cx=1e+2;
	x[icount++]=Interval(-f*cx,f*cx);
                }
            }
#ifdef KMEANS
            for(int i=0;i<nodes;i++)
            {
                double norm=0.0;
        double maxvx=0.0;
                for(int j=0;j<trainx[0].size();j++)
                {
                    double vx=variances[i * trainx[0].size()+j];
            if(fabs(vx)>maxvx) maxvx=fabs(vx);
        }
        if(maxvx<0.1) maxvx=1.0;
        for(int j=0;j<trainx[0].size();j++)
        {
                    x[icount++]=Interval(-f*maxvx,f * maxvx);

                }

            }
#else
	    for(int i=0;i<nodes;i++)
	    {
		double maxvx=0.0;
                for(int j=0;j<trainx[0].size();j++)
                {
                    double vx=varianceValues[i][j];
                    maxvx+=vx;
		}
     //   if(maxvx<0.1) maxvx=1.0;
	//if(fabs(maxvx)>100) maxvx = 100;
        //x[icount++]=Interval(-f * maxvx,f * maxvx);
	x[icount++]=Interval(0.01,f * maxvx);
//		    x[icount++]=Interval(-f * fabs(maxvx),f*fabs(maxvx));
		   
	    }
#endif

    }
}

int icount=0;
double neuronOutput( vector<double> &x, vector<double> &patt, unsigned pattDim, unsigned offset ){
    double out = 0;
    for(unsigned i = 0; i < pattDim;i++){
        out += (patt[i] - x[offset*pattDim + i]) * (patt[i] - x[offset*pattDim + i]);
    }
    double df=(-out / (x[nodes*pattDim+offset] * x[nodes*pattDim+offset]) );
//    if(fabs(df)>100) return 1e+8;
  //  if(fabs(df)>100)return 1.0;// return 1000;
  if(exp(df)<1e-15) return 1e-15;
    return exp(df);
}

arma::vec train( vector<double> &x,bool &ok ){
	ok = true;
    arma::mat A = arma::zeros(trainx.size(),nodes);
    arma::vec B(trainy.size());
    for(unsigned i = 0; i < trainy.size(); i++){
        B.at(i) = trainy[i];
	icount = 0;
        for(unsigned j = 0; j < nodes;j++){
            A.at(i,j) = neuronOutput(x, trainx[i], trainx[0].size() , j);
        }
    }
    
    arma::vec RetVal;
    try{
   RetVal=arma::vec(arma::pinv(A)*B);
   
   // RetVal=arma::vec(arma::pinv(A,1e-10,"dc")*B);
    }
    catch(std::runtime_error & e)
    {
        RetVal = arma::zeros(arma::size(RetVal));
	ok = false;
    }
    if(RetVal.has_nan() || RetVal.has_inf()) {
        RetVal = arma::zeros(arma::size(RetVal));
        }
    for(int i=0;i<nodes;i++)
    {
	    if(RetVal[i]<initialLeft) RetVal[i]=initialLeft;
	    if(RetVal[i]>initialRight) RetVal[i]=initialRight;
    }
    return RetVal;
}

double	funmin(vector<double> &x)
{
#ifdef KMEANS
    int pattern_dimension=trainx[0].size();
    double *weights=new double[nodes];
    int icount=0;
    memcpy(centers,x.data(),nodes * pattern_dimension*sizeof(double));
    double *d=x.data();
    memcpy(variances,&d[nodes*pattern_dimension],nodes*pattern_dimension*sizeof(double));
    train_rbf(trainx[0].size(),nodes,1,trainx.size(),
                            centers,variances,weights,xinput,yinput);
    double sum=0.0;
    double *xt=new double[pattern_dimension];
    double penalty=0.0;
	double norm = 0.0;
    for(int i=0;i<nodes;i++)
       {
	       if(fabs(weights[i])>1000)
	       {
           penalty=1;
        norm=norm+(weights[i]-1000.0)*(weights[i]-1000.0);
	       }
    }

    norm =  sqrt(norm);
    if(penalty) penalty=norm;
    for(int i=0;i<(int)trainx.size();i++)
    {
        double outv[1];
        for(int j=0;j<pattern_dimension;j++) xt[j]=trainx[i][j];
       create_rbf(trainx[0].size(),nodes,1,
                                       centers,variances,weights,xt,outv);
        sum+=(trainy[i]-outv[0])*(trainy[i]-outv[0]);
    }
    delete[] weights;
    delete[] xt;
    return sum*1.0+1.0 * penalty;
#else
    double errorSum=0.0;  
    bool ok;
    arma::vec Linear = train(x,ok);
    if(!ok) return 1e+10;
    double norm = 0.0;
    for(int j=0;j<nodes;j++)

        norm+=(Linear(j))*(Linear(j));
    norm = sqrt(norm);

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

  //if(norm>1000) return errorSum*(1.0+norm);
  return errorSum;
#endif
}

adept::adouble aneuronOutput( vector<adept::adouble> &x, vector<double> &patt, unsigned pattDim, unsigned offset ){
    adept::adouble out = 0;
    for(unsigned i = 0; i < pattDim;i++){
        int index = offset * pattDim+i;
        out += (patt[i] - x[index]) * (patt[i] - x[index]);
    }
    int outIndex = nodes * pattDim + offset;
    adept::adouble darg = out / (x[outIndex] * x[outIndex]);
    //if(exp(-darg)<1e-15) return 1e-15;
//    if(fabs(darg)>100) return 1e+8;
    return exp(-darg );
}

adept::adouble afunmin( vector<adept::adouble> &x, vector<double> &x1 ){
    adept::adouble errorSum=0.0;
    
    bool ok;
    arma::vec Linear = train(x1,ok);
    if(!ok) return 1e+10;

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

     double norm = 0.0;
    for(int j=0;j<nodes;j++)

        norm+=(Linear(j))*(Linear(j));
    norm = sqrt(norm);
    //if(norm>1000) return errorSum*(1.0+norm);
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
      adept::Stack stack;
    stack.continue_recording();

    std::vector<adept::adouble> w(x.size());
    for(unsigned i = 0; i < x.size();i++) w[i]=x[i];// w[i] = node[i];
    stack.new_recording();
    adept::adouble error;


    error = afunmin(w,x);
    error.set_gradient(1.0);
    stack.compute_adjoint();
    for(unsigned i = 0; i < x.size();i++) {
        g[i] = w[i].get_gradient();
        //std::cout << i << " " << g[i] << std::endl;
    }
    stack.pause_recording();

/*
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
    }*/
}


QString toString(Data &x)
{
    return "RBF";
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
#ifdef KMEANS
    funmin(x);
    double *weights=new double[nodes];
    train_rbf(dimension,nodes,1,trainx.size(),
                            centers,variances,weights,xinput,yinput);
    double *xt=new double[dimension];
    printf("WEIGHTS ");
    for(int i=0;i<nodes;i++)
	    printf("%lf ",weights[i]);
    printf("\n");

    for(int i=0;i<(int)testx.size();i++)
    {
        double outv[1];
	printf("TEST ");
        for(int j=0;j<dimension;j++) 
	{
		xt[j]=testx[i][j];
		printf("%lf ",xt[j]);
	}
       create_rbf(dimension,nodes,1,centers,variances,weights,xt,outv);
        sum+=(testy[i]-outv[0])*(testy[i]-outv[0]);
	printf("(%lf -> %lf )\n",testy[i],outv[0]);
        classError+=fabs(testy[i]-nearestClass(outv[0]))>1e-7;

    }
    delete[] weights;
#else
    bool ok;
    arma::vec Linear = train(x,ok);

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
    printf("CLASSERROR=%.2lf%% TESTERROR=%10.5lf\n",
           classError*100.0/testy.size(),sum);
     haveInitialized = false;
#endif
    QJsonObject result;
    result["nodes"]=nodes;
    result["testError"]=sum;
    result["classError"]=classError*100.0/testy.size();
    result["string"]=toString(x);

    return result;
}

}

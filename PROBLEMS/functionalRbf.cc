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
void Kmeans(double * data_vectors,
            vector<double> &centers,
            vector<double> &variances,
            int m, int n, int K,
            vector<int>& num_of_cluster_members)
{
    int i=0;
    int j=0;
    int l=0;
    int k=0;
    double * new_centers = (double*)malloc(sizeof(double)*K*n);
    int **cluster_members=new int*[K];
    for(int i=0;i<K;i++)
        cluster_members[i]=new int[m];
    num_of_cluster_members.resize(K);

    double distance=0;
    double total_distance=0;
    double min_distance=0;
    int min_center=0;
    int match=0;
    int cur_match=0;
    int new_cen=0;
    int *random_centers=new int[K];
    int found=1;
    int iterations=0;


    // Assign a random center to each example in the training set
    for(i=0; i<K; i++)
    {
        do{
            random_centers[i]=(int)((m-1) * drand48());
        }while(random_centers[i]>m);

    }
    // Search the dataset and assign duplicate examples to different centers
    for(i=0; i<K; i++)
        for(j=0; j<K; j++)
            if(i!=j)
            if(random_centers[i] == random_centers[j]) {
                for(l=0; l<m; l++) {
                found=1;
                for(k=0; k<K; k++) {
                    if(l==random_centers[k])
                        found=0;
                }
                if(found==1) {
                    new_cen=l;
                    break;
                }
                }
                random_centers[j]=new_cen;
            }
    //for(i=0; i<K; i++)  printf("Random center is: %d %d\n", i, random_centers[i]);


    // Create the initial random centers
    for(i=0; i<K; i++) {
        for(j=0; j<n; j++) {
            //if(random_centers[i]==m)
            //	printf("error \n");
          centers[i*n + j] = data_vectors[random_centers[i]*n + j];
          new_centers[i*n + j] = 0;
          variances[i*n + j] = 0;
        }
        num_of_cluster_members[i]=0;
        for(j=0; j<m; j++)
            cluster_members[i][j]=0;
    }

    // Main K-Means loop starts here
    iterations=0;
    while(1) {

    /* Loop over all points in the dataset */
    for(i=0; i<m; i++) {

       /* Estimate the closest center to point i */
       for(j=0; j<K; j++) {
        distance=0;
        for(l=0; l<n; l++) {
            distance += pow((data_vectors[i*n + l] - centers[j*n + l]),2.0);
        }

        if(j==0) {
           min_distance = distance;
           min_center = j;
           continue;
        }

        if(distance < min_distance) {
           min_distance = distance;
           min_center = j;
        }
       }

       for(l=0; l<n; l++)
        new_centers[min_center*n + l] += data_vectors[i*n + l];
       cluster_members[min_center][num_of_cluster_members[min_center]] = i;
       num_of_cluster_members[min_center]++;


    }

       /* Estimate the new centers */
       for(i=0; i<K; i++) {
        for(l=0; l<n; l++) {
            //GIANNIS
            if(num_of_cluster_members[i])
           new_centers[i*n + l] /= (double)num_of_cluster_members[i];
            //GIANNIS
           if(finite(new_centers[i*n + l]) == 0)
            new_centers[i*n + l] = 0;
        }
       }

    //for(i=0; i<K; i++) {
    //	printf("Cluster members [%d]:  %d\n", i, num_of_cluster_members[i]);
    //}


    /* Here we print the total distance for each pass */
    for(i=0; i<K; i++) {
      for(j=0; j<num_of_cluster_members[i]; j++) {
        for(l=0; l<n; l++) {
        total_distance += pow((data_vectors[cluster_members[i][j]*n + l] - centers[i*n + l]),2.0);
        }
      }
    }
    //printf("Total distance: %f\n", total_distance);
    total_distance=0;

       /* Check if converges */
       match=0;
       for(i=0; i<K; i++) {
        cur_match=0;
        for(j=0; j<n; j++) {
           if(new_centers[i*n + j] == centers[i*n + j])
            cur_match++;
        }
        if(cur_match == n)
            match++;
       }

       /* If the centers remain the same: terminate */
       if(match == K)
        break;


       for(i=0; i<K; i++) {
        for(j=0; j<n; j++) {
            centers[i*n + j] = new_centers[i*n + j];
            new_centers[i*n + j]=0;
        }
        num_of_cluster_members[i]=0;
       }

       iterations++;
       if(iterations>2000) break;
    }                        /* telos tou while(1) */


    /* ----------------- YPOLOGISMOS VARIANCE -------------- */

    for(i=0; i<K; i++) {
        for(j=0; j<num_of_cluster_members[i]; j++) {
            for(l=0; l<n; l++) {
                variances[i*n + l] += pow((data_vectors[cluster_members[i][j]*n + l] - centers[i*n + l]),2.0);
            }
        }
    }

    for(i=0; i<K; i++) {
        for(j=0; j<n; j++) {
            //GIANNIS
            if(num_of_cluster_members[i]>1)
            variances[i*n + j] /= (double)num_of_cluster_members[i];
            else
            {
                variances[i*n+j]=0.01;
            }
        }
    }



   free(new_centers);
   delete[] random_centers;
   for(int i=0;i<K;i++) delete[] cluster_members[i];
   delete[] cluster_members;
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
double *xinput=0;
double *yinput=0;
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

vector<double> centers;
vector<double> variances;

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
        nodes=data["nodes"].toString().toInt();
    if(data.contains("initialLeft"))
        initialLeft=data["initialLeft"].toDouble();
    if(data.contains("initialRight"))
        initialRight=data["initialRight"].toDouble();
    loadTrain();  
    if(testName!="xy.data") loadTest();
    bool redo=false;
    int posRedo=-1;
    int startNodes=nodes;
//#ifdef KMEANS

    xinput=new double[ trainx.size() * trainx[0].size()];
    yinput=new double[ trainx.size()];

    int icount=0;
    for(int i=0;i<trainx.size();i++)
    {
        for(int j=0;j<trainx[0].size();j++)
            xinput[icount++]=trainx[i][j];
        yinput[i]=trainy[i];
    }
    centers.resize(nodes * trainx[0].size());
    variances.resize(nodes * trainx[0].size());
        again:

    Kmeans(xinput,centers,variances,trainx.size(),trainx[0].size(),nodes,
            num_of_cluster_members);
    for(int i=0;i<nodes;i++)
    {
        double f=num_of_cluster_members[i]*1.0/trainx.size();
        double limitDown=1.0/(2.0 * startNodes);
        if(f<limitDown)
        {
           redo=true;
           posRedo=i;
        }
        printf("members[%d]=%.2lf%%\n",i,num_of_cluster_members[i]*100.0/trainx.size());
    }
    for(int i=0;i<nodes;i++)
    {
        for(int j=0;j<dimension;j++)
        {
            if(i==0 || centers[i*dimension+j]<xleft[j])  xleft[j]=centers[i*dimension+j];
            if(i==0 || centers[i*dimension+j]>xright[j]) xright[j]=centers[i*dimension+j];
        }
    }

    for(int i=0;i<nodes;i++)
    {
        double minDist=1e+100;
        for(int j=0;j<i;j++)
        {
            double dist=0.0;

            for(int k=0;k<dimension;k++)
            {
                double diff=centers[i*dimension+k]-centers[j*dimension+k];
                diff=diff/(xright[k]-xleft[k]);

                dist+=fabs(diff) ;
            }
            dist/=dimension;
            if(dist<minDist) minDist=dist;

        }
        if(i)
         printf("Dist[%d]=%10.2lf\n",i,minDist);
        //if(minDist<0.1) redo=true;
        if(redo)
        {
           /* nodes = nodes-1;
            centers.erase(centers.begin()+posRedo * dimension,
                          centers.begin()+posRedo * dimension+dimension);
            variances.erase(variances.begin()+posRedo * dimension,
                          variances.begin()+posRedo * dimension+dimension);
            redo=false;
            goto again;*/
        }
    }

//#endif
}

int	getdimension()
{
#ifdef KMEANS
    return 2*(trainx[0].size())*nodes;
#else
    return (dimension+1) * nodes;
#endif
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(initialLeft,initialRight);
    }
	
    if(variances.size()!=0)
    {
            int icount=0;
	    	double f=3.0;
            for(int i=0;i<nodes;i++)
            {
                for(int j=0;j<trainx[0].size();j++)
                {

                    double cx=fabs(centers[i * trainx[0].size()+j]);
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
                    double vx=variances[i * trainx[0].size()+j];
                    maxvx+=vx;
		}
     //   if(maxvx<0.1) maxvx=1.0;
	//if(fabs(maxvx)>100) maxvx = 100;
        //x[icount++]=Interval(-f * maxvx,f * maxvx);
	x[icount++]=Interval(-f * maxvx,f * maxvx);
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
    if(fabs(df)>100) return 1e+8;
  //  if(fabs(df)>100)return 1.0;// return 1000;
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
    
    arma::vec RetVal;
   // try{
    RetVal=arma::vec(arma::pinv(A)*B);
    //RetVal=arma::vec(arma::pinv(A,1e-10,"dc")*B);
    //}catch(std::runtime_error & e)
    //{
     //   RetVal = arma::zeros(arma::size(RetVal));
    //}
    if(RetVal.has_nan() || RetVal.has_inf()) {
        RetVal = arma::zeros(arma::size(RetVal));
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
    arma::vec Linear = train(x);
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
    printf("CLASSERROR=%.2lf%% TESTERROR=%10.5lf\n",
           classError*100.0/testy.size(),sum);
#endif
    QJsonObject result;
    result["nodes"]=nodes;
    result["testError"]=sum;
    result["classError"]=classError*100.0/testy.size();
    result["string"]=toString(x);
    return result;
}

}

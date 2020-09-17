#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
# include "grand.h"

/* ---------------------------------------------- */
/* K-Means Clusteriing Algorithm                  */
/* Reads data from data_vectors matrix,           */
/* implements kmeans clustering and returns the   */ 
/* results to centers matrix along with the       */ 
/* variances into the variances matrix.           */ 
/* Input Parameters:                              */
/*      data_vectors : pointer to input data      */
/*                     matrix (m rows, n columns) */ 
/*      centers      : pointer to centers matrix  */
/*                     (K rows, n columns)        */
/*      variances    : pointer to the variances   */
/*                     matrix (n rows, n columns) */
/*      m            : number of data vectors     */
/*      n            : dimension of data vector   */
/*      K            : number of centers          */ 
/* ---------------------------------------------- */


void Kmeans(GRand *rnd,double * data_vectors, double * centers, 
            double * variances, int m, int n, int K);

#include "matrix_functions.h"

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



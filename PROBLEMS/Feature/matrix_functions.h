#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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
                 int m, int p, int n);



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


void matrix_transpose(double * a, double * c, int m, int n);





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


int matrix_inverse(double * a, double * c, int n, double * det);




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


void matrix_pseudo_inverse(double * a, double * c, int m, int n);






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


void matrix_add(double * a, double * b, double * c, int m, int n);





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


void matrix_sub(double * a, double * b, double * c, int m, int n);




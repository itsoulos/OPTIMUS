/* RBF Network Creation */

#include "Rbf.h"
#include "matrix_functions.h"
#include "kmeans.h"

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

/* --------------------------------------------------------
   for(i=0; i<samples_n; i++) {
	for(j=0; j<hid_n; j++) {
		printf("%f\t", G[i][j]); 
	}
	printf("\n");
   }
   printf("\n\n");
   for(i=0; i<hid_n; i++) {
	for(j=0; j<samples_n; j++) {
		printf("%f\t", Gp[i][j]); 
	}
	printf("\n");
   }
   printf("\n\n");
   for(i=0; i<hid_n; i++) {
	for(j=0; j<out_n; j++) {
		printf("%f\t", weights[i*out_n + j]);
	}
	printf("\n");
   }
---------------------------------------------------------- */ 

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


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> // for execv 
#include <string.h>

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

int train_rbf(int in_n, int hid_n, int out_n, int samples_n,
	      double * centers, double * variances, 
	      double * weights, double * input, double * output);



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
/*                  (hid_n columns)             */
/*      input     : pointer to input vector     */
/*                  (1 row, in_n columns)       */
/*      output    : pointer to output vector    */
/*                  (1 row, out_n columns)      */
/* -------------------------------------------- */

int create_rbf(int in_n, int hid_n, int out_n, 
	      double * centers, double * variances, 
	      double * weights, double * input, double * output);



/* -------------------------------------------- */
/* RBF Network parameter structure              */
/*						*/
/*	int input				*/
/*	int hidden				*/
/*	int output				*/
/*	double weights[hidden][output] 		*/
/*	double centers[hidden][input] 		*/
/* 	double variances[hidden][input] 	*/
/* -------------------------------------------- */


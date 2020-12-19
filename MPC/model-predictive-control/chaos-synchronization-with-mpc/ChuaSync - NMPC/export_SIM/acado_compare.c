/*
 *    This file was auto-generated by ACADO Code Generation Tool.
 *    
 *    ACADO Code Generation tool is a sub-package of ACADO toolkit --
 *    A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2014 by Boris Houska, Hans Joachim Ferreau,
 *    Milan Vukov, Rien Quirynen, KU Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC)
 *    under supervision of Moritz Diehl. All rights reserved.
 *    
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *    
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *    
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *    
 */


#include "acado_common.h"


#include <stdio.h>
#include "acado_auxiliary_sim_functions.h"

/* SOME CONVENIENT DEFINTIONS: */
/* --------------------------------------------------------------- */
   #define h           0.02      /* length of one simulation interval   */
   #define RESULTS_NAME	  "results.txt"
   #define REF_NAME  "ref.txt"
/* --------------------------------------------------------------- */


/* GLOBAL VARIABLES FOR THE ACADO REAL-TIME ALGORITHM: */
/* --------------------------------------------------- */
   ACADOworkspace acadoWorkspace;
   ACADOvariables acadoVariables;



/* A TEMPLATE FOR TESTING THE INTEGRATOR: */
/* ---------------------------------------------------- */
int main(){

   /* INTRODUCE AUXILIARY VAIRABLES: */
   /* ------------------------------ */
      FILE *file, *ref;
      int i, j, nil;
      real_t x[ACADO_NX+ACADO_NXA];
      real_t xRef[ACADO_NX+ACADO_NXA];
      real_t maxErr, meanErr, maxErrX, meanErrX, maxErrXA, meanErrXA, temp;
      const ACADOworkspace nullWork2 = {0};
 	  acadoWorkspace = nullWork2;


   /* START EVALUATION RESULTS: */
   /* ---------------------------------------- */
      meanErrX = 0;
      meanErrXA = 0;
      file = fopen(RESULTS_NAME,"r");
      ref = fopen(REF_NAME,"r");
      for( i = 0; i < (ACADO_NX+ACADO_NXA)*(1+ACADO_NX+ACADO_NU)+1; i++ ) {
      		nil = fscanf( file, "%lf", &temp );
      		nil = fscanf( ref, "%lf", &temp );
      }
      printf( " STATES:\n" );

      for( i = 1; i <= ACADO_N; i++ ) {
      		nil = fscanf( file, "%lf", &temp );
      		nil = fscanf( ref, "%lf", &temp );

      		maxErrX = 0;
      		for( j = 0; j < ACADO_NX; j++ ) {
      			nil = fscanf( file, "%lf", &x[j] );
      			nil = fscanf( ref, "%lf", &xRef[j] );
      			temp = fabs(x[j] - xRef[j])/fabs(xRef[j]);
      			if( temp > maxErrX ) maxErrX = temp;
      			if( isnan(x[j]) ) maxErrX = sqrt(-1);
      		}

      		maxErrXA = 0;
      		for( j = 0; j < ACADO_NXA; j++ ) {
      			nil = fscanf( file, "%lf", &x[ACADO_NX+j] );
      			nil = fscanf( ref, "%lf", &xRef[ACADO_NX+j] );
      			temp = fabs(x[ACADO_NX+j] - xRef[ACADO_NX+j])/fabs(xRef[ACADO_NX+j]);
      			if( temp > maxErrXA ) maxErrXA = temp;
      			if( isnan(x[ACADO_NX+j]) ) maxErrXA = sqrt(-1);
      		}

      		printf( "MAX ERROR AT %.3f s:   %.4e \n", i*h, maxErrX );
			meanErrX += maxErrX;
			meanErrXA += maxErrXA;

      		for( j = 0; j < (ACADO_NX+ACADO_NXA)*(ACADO_NX+ACADO_NU); j++ ) {
      			nil = fscanf( file, "%lf", &temp );
      			nil = fscanf( ref, "%lf", &temp );
      		}
      }
	  meanErrX = meanErrX/ACADO_N;
	  meanErrXA = meanErrXA/ACADO_N;
      printf( "\n" );
      printf( "TOTAL MEAN ERROR:   %.4e \n", meanErrX );
      printf( "\n\n" );

      return 0;
}
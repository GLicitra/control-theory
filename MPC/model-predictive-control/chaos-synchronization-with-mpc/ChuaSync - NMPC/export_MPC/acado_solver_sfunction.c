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


/*
 * ACADO OCP solver wrapper.
 */

#include <string.h>
#include "acado_solver_sfunction.h"
#include "acado_auxiliary_functions.h"

/* Make instances of ACADO structures. */
ACADOvariables acadoVariables;
ACADOworkspace acadoWorkspace;

/* Initialized indicator. */
static unsigned initialized = 0;

void acado_step(acadoinput* inData, acadooutput* outData)
{
	timer tmr;
	
	/* Start measuring time. */
	tic( &tmr );
	
	/* Copy all outside world data here. */	
	memcpy(&acadoVariables, &(inData->data), sizeof( ACADOvariables ));
	
	/* Call solver */
	switch ( inData->control )
	{
		case 0:
			/* Simple operational mode. Run one RTI with optional shifting. */
			
			if ( !initialized )
			{				
				if (inData->initialization == 1)
				{
					initializeNodesByForwardSimulation();
				}
				
				initialized = 1;
			}
			else if (inData->shifting == 1 || inData->shifting == 2)
			{
			
#if ACADO_QP_SOLVER == ACADO_QPDUNES
				shiftQpData();
#endif
			
				shiftStates(inData->shifting, 0, 0);
				shiftControls( 0 );
			}
			
			preparationStep();
			
			outData->status = feedbackStep();
			
			outData->kktValue = getKKT();
			outData->objValue = getObjective();
			
#if ACADO_QP_SOLVER == ACADO_QPOASES
 			outData->nIterations = getNWSR();
#endif /* ACADO_QP_SOLVER == ACADO_QPOASES */
			
			break;
		
		case 1:
			/* Initialize */
				
			if (inData->initialization == 1)
			{
				initializeNodesByForwardSimulation();
			}
			
			initialized = 1;
			
			break;
		
		case 2:
			/* Preparation step */
			
			preparationStep();
			
			break;
		
		case 3:
			/* Feedback step */
			
			outData->status = feedbackStep();
			
			outData->kktValue = getKKT();
			outData->objValue = getObjective();
				
#if ACADO_QP_SOLVER == ACADO_QPOASES
			outData->nIterations = getNWSR();
#endif /* ACADO_QP_SOLVER == ACADO_QPOASES */
						
			break;
		
		case 4:
			/* Shifting */
			
#if ACADO_QP_SOLVER == ACADO_QPDUNES
			shiftQpData();
#endif
			
			shiftStates(inData->shifting, 0, 0);
			shiftControls( 0 );
			
			break;
			
		default:
			/* Return default error code */
			outData->status = -1;
	}
	
	/* Copy the data to outside world. */
	memcpy(&(outData->data), &acadoVariables, sizeof( ACADOvariables ));
	
	/* Read the elapsed time. */
	outData->execTime = toc( &tmr );
}

void acado_initialize( void )
{
	memset(&acadoWorkspace, 0, sizeof( acadoWorkspace ));	
	initializeSolver();
	initialized = 0;
}

void acado_cleanup( void )
{
#if ACADO_QP_SOLVER == ACADO_QPDUNES
	/* Clean-up of the qpDUNES QP solver. */
	cleanupSolver();
#endif /* ACADO_QP_SOLVER == ACADO_QPDUNES */
}
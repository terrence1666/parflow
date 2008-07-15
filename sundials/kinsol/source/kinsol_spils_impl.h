/*
 * -----------------------------------------------------------------
 * $Revision: 1.1 $
 * $Date: 2006/02/02 00:36:32 $
 * -----------------------------------------------------------------
 * Programmer(s): Radu Serban and Aaron Collier @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2002, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials/kinsol/LICENSE.
 * -----------------------------------------------------------------
 * Common implementation header file for the scaled, preconditioned
 * linear solver modules.
 * -----------------------------------------------------------------
 */

#ifndef _KINSPILS_IMPL_H
#define _KINSPILS_IMPL_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#include "kinsol_spils.h"

/* Types of iterative linear solvers */

#define SPILS_SPGMR   1
#define SPILS_SPBCG   2
#define SPILS_SPTFQMR 3

/*
 * -----------------------------------------------------------------
 * keys for KINPrintInfo (do not use 1 -> conflict with PRNT_RETVAL)
 * -----------------------------------------------------------------
 */

#define PRNT_NLI   101
#define PRNT_EPS   102

/*
 * -----------------------------------------------------------------
 * Types : struct KINSpilsMemRec and struct *KINSpilsMem
 * -----------------------------------------------------------------
 * A variable declaration of type struct *KINSpilsMem denotes a
 * pointer to a data structure of type struct KINSpilsMemRec. The
 * KINSpilsMemRec structure contains fields that must be accessible
 * by KINSPILS/SPGMR solver module routines.
 * -----------------------------------------------------------------
 */

typedef struct {

  int s_type;           /* type of scaled preconditioned iterative LS          */

  /* problem specification data */

  int  s_maxl;          /* maximum allowable dimension of Krylov subspace      */     
  int  s_pretype;       /* preconditioning type: PREC_NONE, PREC_RIGHT,
			   PREC_LEFT or PREC_BOTH (used by SPGMR module and
			   defined in shared/include/iterative.h)              */
  int  s_gstype;        /* Gram-Schmidt orthogonalization procedure:
			   CLASSICAL_GS or MODIFIED_GS (used by SPGMR module
			   and defined in shared/include/iterative.h)          */
  booleantype s_new_uu; /* flag indicating if the iterate has been updated -
			   Jacobian must be updated/reevaluated (meant to be
			   used by user-supplied jtimes function)              */
  int s_maxlrst;        /* maximum number of times the SPGMR linear solver
			   can be restarted                                    */

  /* counters */

  long int s_nli;     /* number of linear iterations performed                 */
  long int s_npe;     /* number of preconditioner evaluations                  */
  long int s_nps;     /* number of times preconditioner was applied to linear
		         system                                                */
  long int s_ncfl;    /* number of linear convergence failures                 */
  long int s_nfes;    /* number of evaluations of the system function F(u) or
			 number of calls made to func routine                  */    
  long int s_njtimes; /* number of times the matrix-vector product J(u)*v
			 was computed or number of calls made to jtimes
			 routine                                               */

  /* functions */

  KINSpilsPrecSetupFn s_pset;     /* routine called to compute preconditioner
				     matrix                                    */
  KINSpilsPrecSolveFn s_psolve;   /* subroutine called to solve a
				     preconditioned linear system              */ 
  KINSpilsJacTimesVecFn s_jtimes; /* function called to compute matrix-vector
				     product J(u)*v                            */

  /* memory references (pointers) */

  void *s_P_data; /* pointer to user-allocated memory block that is passed
		     to pset and psolve                                        */
  void *s_J_data; /* pointer to user-allocated memory block that is passed
		     to jtimes (only required if using a user-supplied
		     jtimes routine)                                           */

  void *s_spils_mem;    /* pointer to generic linear solver memory block       */

  /* miscellaneous data */

  int s_last_flag; /* last flag returned                                       */

} KINSpilsMemRec, *KINSpilsMem;


/*
 * -----------------------------------------------------------------
 * Prototypes of internal functions
 * -----------------------------------------------------------------
 */

/* KINSpgmr Atimes and PSolve routines called by generic SPGMR solver */

int KINSpilsAtimes(void *kinsol_mem, N_Vector v, N_Vector z);
int KINSpilsPSolve(void *kinsol_mem, N_Vector r, N_Vector z, int lr);

/* difference quotient approximation for jacobian times vector */

int KINSpilsDQJtimes(N_Vector v, N_Vector Jv,
                     N_Vector u, booleantype *new_u, 
                     void *jac_data);

/*
 * -----------------------------------------------------------------
 * KINSPILS error messages
 * -----------------------------------------------------------------
 */

#define MSGS_KINMEM_NULL "KINSOL memory is NULL."
#define MSGS_MEM_FAIL    "A memory request failed."
#define MSGS_BAD_NVECTOR "A required vector operation is not implemented."
#define MSGS_LMEM_NULL   "Linear solver memory is NULL."
#define MSGS_NEG_MAXRS   "maxrs < 0 illegal."


/*
 * -----------------------------------------------------------------
 * KINSPILS info messages
 * -----------------------------------------------------------------
 */

#define INFO_NLI  "nli_inc = %d"

#if defined(SUNDIALS_EXTENDED_PRECISION)

#define INFO_EPS  "residual norm = %12.3Lg  eps = %12.3Lg"

#elif defined(SUNDIALS_DOUBLE_PRECISION)

#define INFO_EPS  "residual norm = %12.3lg  eps = %12.3lg"

#else

#define INFO_EPS  "residual norm = %12.3g  eps = %12.3g"

#endif



#ifdef __cplusplus
}
#endif

#endif
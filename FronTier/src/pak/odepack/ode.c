/*		       
*				ode.c:
*
*	Copyright 1999 by The University at Stony Brook, All rights reserved.
*
*	C front end for the odepack.
*/

#include <cdecs.h>

#if defined(__cplusplus)
extern "C" {    
#endif /* defined(__cplusplus) */
    LOCAL   LSODE_JAC 	jac_dummy;
#if defined(__cplusplus)
}                          
#endif /* defined(__cplusplus) */


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*ARGSUSED*/
LOCAL	void	jac_dummy(
	int	*neq,
	float	*t,
	float	*y,
	int	*ml,
	int	*mu,
	float	*pd,
	int	*nrowpd)
{
}	/*end jac_dummy*/

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

/*
*			ode_solver():
*
*	Front end for the ode solver lsode.  Solves the scalar equation
*
*		dy/dt = f(t,y)
*
*	Input:
*		f = function returning rhs, f(&neq,&t,&y,&yp)
*			Input:
*				neq = number of equations in system (= 1)
*				t = value of independent variable
*				y = value of dependent variable
*			Output:
*				yp = dy/dx
*		y[0] = initial value of dependent variable
*		t0 = initial value of independent variable
*		dt = spacing for independent variable
*		nt = number of solution points desired
*		rtol = relative error tolerance
*		atol = absolute error tolerance
*		terr = independent variable tolerance, accept answer if
*		       lsode can integrate to within terr of desired point
*		*istate = lode flag,  1 for start, 2 to continue
*	Output:
*		y[1]..y[nt-1] = array of solution values
*		*istate = lsode return status flag
*/

EXPORT	int	ode_solver(
	LSODE_FUNC *f,
	float	   *y,
	float	   t0,
	float	   dt,
	int	   nt,
	float	   rtol,
	float	   atol,
	float	   terr,
	int	   *istate,
        LSODE_JAC  *jac)
{
	static const	int	max_num_tries = 100;/*TOLERANCE*/
	int	i, j, n;
	int	itol = 2;
	int	itask = 1;
	int	iopt = 1;
     	//TMP_XY	
	//int	lrw = 196;
	int	lrw = 212;
	int	iwork[20];
	int	liw = 20;
	int	mf = 10;  
	float	t, tout;
	//TMP_XY
	//float	rwork[196];
	float	rwork[212];
	//TMP_XY
//	static	int	ydim = 11;  //number of equations
	static	int	ydim = 12;  //number of equations

        float   aatol[ydim];

        if(dt < 0.000000001)  //TEMP
            return 1;

        for (i = 0; i < ydim; ++i)
            aatol[i] = atol;

	if (jac == NULL)
    	    jac = jac_dummy;

        
	for (i = 0; i < liw; ++i)
	    iwork[i] = 0;
	for (i = 0; i < lrw; ++i)
	    rwork[i] = 0.0;
	iwork[5] = 5000;
	int k;
	t = tout = t0;
	for (i = 1; i < nt+1; ++i)
	{
            tout += dt;
//	    y[i] = y[i-1];
	    mf = 10;
            
	    for (n = 0; n < max_num_tries; ++n)
	    {
                if (*istate == -1)
		    *istate = 2;
	        FORTRAN_NAME(lsode)(f,&ydim,
		    y,&t,&tout,&itol,&rtol,aatol,&itask,istate,&iopt,
		    rwork,&lrw,iwork,&liw,jac,&mf);
	        switch (*istate)
	        {
		case -1:
		    if (fabs(tout-t) < terr)
			*istate = 2;
		    break;
	        case -2:
		    if (DEBUG)
		    {
		        (void)printf("WARNING in ode_solver(), "
		                     "excessive accuracy requested\n");
		    }
		    rtol *= rwork[13];
		    atol *= rwork[13];
		    *istate = 3;
		    break;
	        case -3:
		    (void) printf("WARNING in ode_solver(), "
		                  "illegal input detected\n");
		    return FUNCTION_FAILED;
	        case -4:
		    (void) printf("WARNING in ode_solver(), "
		                  "repeated error test failures\n");
		    return FUNCTION_FAILED;
	        case -5:
		    (void) printf("WARNING in ode_solver(), "
		                  "repeated convergence failures\n");
		    return FUNCTION_FAILED;
	        case -6:
		    (void) printf("WARNING in ode_solver(), "
		                  "error weight became zero during problem\n");
		    return FUNCTION_FAILED;
	        default:
		    break;
	        }
		if (*istate == 2)
		    break;
	    }
	    if ((n == max_num_tries) && (*istate != 2))
	    {
		(void) printf("WARNING in ode_solver(), "
		              "did not succeed in integrating up to limit\n");
	    }
	}
	return 1;
}		/*end ode_solver*/


EXPORT	int	ode_solver_VM(
	LSODE_FUNC *f,
	float	   *y,
	float	   t0,
	float	   dt,
	int	   nt,
	float	   rtol,
	float	   atol,
	float	   terr,
	int	   *istate,
        LSODE_JAC  *jac,
	float	   *coords)
{
	static const	int	max_num_tries = 100;/*TOLERANCE*/
	int	i, j, n;
	int	itol = 2;
	int	itask = 1;
	int	iopt = 1;
     	//TMP_XY	
	//int	lrw = 196;
	int	lrw = 212;
	int	iwork[20];
	int	liw = 20;
	int	mf = 10;  
	float	t, tout;
	//TMP_XY
	//float	rwork[196];
	float	rwork[212];
	//TMP_XY
//	static	int	ydim = 11;  //number of equations
	static	int	ydim = 12;  //number of equations

        float   aatol[ydim];

        if(dt < 0.000000001)  //TEMP
            return 1;

        for (i = 0; i < ydim; ++i)
            aatol[i] = atol;

	if (jac == NULL)
    	    jac = jac_dummy;

        
	for (i = 0; i < liw; ++i)
	    iwork[i] = 0;
	for (i = 0; i < lrw; ++i)
	    rwork[i] = 0.0;
	iwork[5] = 5000;
	int k;
	t = tout = t0;
	for (i = 1; i < nt+1; ++i)
	{
            tout += dt;
//	    y[i] = y[i-1];
	    mf = 10;
	    for (n = 0; n < max_num_tries; ++n)
	    {
		if (*istate == -1)
		    *istate = 2;
	        FORTRAN_NAME(lsode)(f,&ydim,
		    y,&t,&tout,&itol,&rtol,aatol,&itask,istate,&iopt,
		    rwork,&lrw,iwork,&liw,jac,&mf);
	        switch (*istate)
	        {
		case -1:
		    for ( k = 0; k < 15; k++) printf("Y[%d] = %e\n",k,y[k]);
		    printf("coords = (%f, %f, %f)\n",coords[0],coords[1], coords[2]);
		    if (fabs(tout-t) < terr)
			*istate = 2;
		    break;
	        case -2:
		    if (DEBUG)
		    {
		        (void)printf("WARNING in ode_solver(), "
		                     "excessive accuracy requested\n");
		    }
		    rtol *= rwork[13];
		    atol *= rwork[13];
		    *istate = 3;
		    break;
	        case -3:
		    (void) printf("WARNING in ode_solver(), "
		                  "illegal input detected\n");
		    return FUNCTION_FAILED;
	        case -4:
		    (void) printf("WARNING in ode_solver(), "
		                  "repeated error test failures\n");
		    return FUNCTION_FAILED;
	        case -5:
		    (void) printf("WARNING in ode_solver(), "
		                  "repeated convergence failures\n");
		    return FUNCTION_FAILED;
	        case -6:
		    (void) printf("WARNING in ode_solver(), "
		                  "error weight became zero during problem\n");
		    return FUNCTION_FAILED;
	        default:
		    break;
	        }
		if (*istate == 2)
		    break;
	    }
	    if ((n == max_num_tries) && (*istate != 2))
	    {
		(void) printf("WARNING in ode_solver(), "
		              "did not succeed in integrating up to limit\n");
	    }
	}
	return 1;
}		/*end ode_solver_VM*/

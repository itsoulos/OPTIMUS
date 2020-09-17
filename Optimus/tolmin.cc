# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include <tolmin.h>
# include <omp.h>
int ifail=0;
#pragma omp threadprivate(ifail)
long iabs(long x)
{
    return x>0?x:-x;
}

#ifdef __cplusplus
extern "C" {
#endif

/* Common Block Declarations */

# define integer long
# define double double
# define real float
struct {
    integer itnocs, ipartc, itder, ipder, ithess, iphess, itjac, ipjac;
} totcal_;
#pragma omp threadprivate(totcal_)

#define totcal_1 totcal_

struct {
    integer iuinp, iuout;
} units_;
#pragma omp threadprivate(units_)

#define units_1 units_

/* Table of constant values */

integer c__0 = 0;
#pragma omp threadprivate(c__0)

/*  --------------------------------------------------------------------- */

/* Subroutine */ int getmin_(integer *n, integer *m, integer *meq, double 
	*a, integer *ia, double *b, double *xl, double *xu, 
	double *x, double *acc, integer *iact, integer *nact, 
	double *par, integer *iprint, integer *info, double *w,
    Problem *p)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
     integer iztg, ixbig, ibres, id, ig, iu, iz;
    extern /* Subroutine */ int minflc_(integer *, integer *, integer *, 
	    double *, integer *, double *, double *, double *,
	     double *, double *, integer *, integer *, double *, 
	    integer *, integer *, double *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, double *, double *,
        Problem *);
     integer ireskt, igm, igs, ixs;


/*  This is the entry point to a package of subroutines that calculate */
/*     the least value of a differentiable function of several variables */
/*     subject to linear constraints on the values of the variables. */
/*  N is the number of variables and must be set by the user. */
/*  M is the number of linear constraints (excluding simple bounds) and */
/*     must be set by the user. */
/*  MEQ is the number of constraints that are equalities and must be set */
/*     by the user. */
/*  A(.,.) is a 2-dimensional array whose columns are the gradients of */
/*     the M constraint functions. Its entries must be set by the user */
/*     and its dimensions must be at least N by M. */
/*  IA is the actual first dimension of the array A that is supplied by */
/*     the user, so its value may not be less than N. */
/*  B(.) is a vector of constraint right hand sides that must also be set */
/*     by the user.  Specifically the constraints on the variables X(I) */
/*     I=1(1)N are */
/*          A(1,K)*X(1)+...+A(N,K)*X(N) .EQ. B(K)  K=1,...,MEQ */
/*          A(1,K)*X(1)+...+A(N,K)*X(N) .LE. B(K)  K=MEQ+1,...,M  . */
/*     Note that the data that define the equality constraints come */
/*     before the data of the inequalities. */
/*  XL(.) and XU(.) are vectors whose components must be set to lower and */
/*     upper bounds on the variables.  Choose very large negative and */
/*     positive entries if a component should be unconstrained, or set */
/*     XL(I)=XU(I) to freeze the I-th variable.  Specifically these */
/*     simple bounds are */
/*          XL(I) .LE. X(I) and X(I) .LE. XU(I)  I=1,...,N  . */
/*  X(.) is the vector of variables of the optimization calculation. Its */
/*     initial elements must be set by the user to an estimate of the */
/*     required solution.  The subroutines can usually cope with poor */
/*     estimates, and there is no need for X(.) to be feasible initially. */
/*     These variables are adjusted automatically and the values that */
/*     give the least feasible calculated value of the objective function */
/*     are available in X(.) on the return from GETMIN. */
/*  ACC is a tolerance on the first order conditions at the calculated */
/*     solution of the optimization problem.  These first order */
/*     conditions state that, if X(.) is a solution, then there is a set */
/*     of active constraints with indices IACT(K) K=1(1)NACT, say, */
/*     such that X(.) is on the boundaries of these constraints, */
/*     and the gradient of the objective function can be expressed */
/*     in the form */
/*           GRAD(F)=PAR(1)*GRAD(C(IACT(1)))+... */
/*                        ...+PAR(NACT)*GRAD(C(IACT(NACT)))  . */
/*     Here PAR(K) K=1(1)NACT are Lagrange multipliers that are */
/*     nonpositive for inequality constraints, and GRAD(C(IACT(K))) */
/*     is the gradient of the IACT(K)-th constraint function, so it is */
/*     A(.,IACT(K)) if IACT(K) .LE. M, and it is minus or plus the J-th */
/*     coordinate vector if the constraint is the lower or upper bound on */
/*     X(J) respectively. The normal return from the calculation occurs */
/*     when X(.) is feasible and the sum of squares of components of the */
/*     vector RESKT(.) is at most ACC**2, where RESKT(.) is the */
/*     N-component vector of residuals of the first order condition that */
/*     is displayed above. */
/*     Sometimes the package cannot satisfy this condition, because noise */
/*     in the function values can prevent a change to the variables, */
/*     no line search being allowed to increase the objective function. */
/*  IACT(.) is a working space array of integer variables that must be */
/*     provided by the user.  Its length must be at least (M+2*N).  Its */
/*     leading entries on the return from the subroutine are the indices */
/*     IACT(K) K=1(1)NACT that are mentioned in the previous paragraph: */
/*     in other words they are the indices of the final active */
/*     constraints. */
/*     Here the indices M+1,...,M+N and M+N+1,...,M+2*N denote the lower */
/*     and upper bounds respectively. */
/*  NACT is set automatically to the integer variable of this ilk that */
/*     has been introduced already. */
/*  PAR is a one-dimensional array that will hold the Lagrange */
/*     multipliers PAR(K) K=1(1)NACT on the return from GETMIN, these */
/*     parameters being defined in the above paragraph on ACC. */
/*     The length of PAR should be at least N. */
/*  IPRINT must be set by the user to specify the frequency of printing */
/*     during the execution of the optimization package.  There is no */
/*     printed output if IPRINT=0.  Otherwise, after ensuring */
/*     feasibility, information is given every IABS(IPRINT) iterations */
/*     and whenever a parameter called TOL is reduced. The printing */
/*     provides the values of X(.), F(.) and G(.)=GRAD(F) if IPRINT */
/*     is positive, while if IPRINT is negative this information is */
/*     augmented by the current values of IACT(K) K=1(1)NACT, */
/*     PAR(K) K=1(1)NACT and RESKT(I) I=1(1)N. The reason for returning */
/*     to the calling program is also displayed when IPRINT is nonzero. */
/*  INFO is an integer variable that should be set to zero initially, */
/*     unless the user wishes to impose an upper bound on the number of */
/*     evaluations of the objective function and its gradient, in which */
/*     case INFO should be set to the value of this bound.  On the exit */
/*     from GETMIN it will have one of the following integer values to */
/*     indicate the reason for leaving the optimization package: */
/*          INFO=1   X(.) is feasible and the condition that depends on */
/*     ACC is satisfied. */
/*          INFO=2   X(.) is feasible and rounding errors are preventing */
/*     further progress. */
/*          INFO=3   X(.) is feasible but the objective function fails to */
/*     decrease although a decrease is predicted by the current gradient */
/*     vector.  If this return occurs and KTRES(.) has large components */
/*     then the user's calculation of the gradient of the objective */
/*     function may be incorrect.  One should also question the coding of */
/*     the gradient when the final rate of convergence is slow. */
/*          INFO=4   In this case the calculation cannot begin because IA */
/*     is less than N or because the lower bound on a variable is greater */
/*     than the upper bound. */
/*          INFO=5   This value indicates that the equality constraints */
/*     are inconsistent.   These constraints include any components of */
/*     X(.) that are frozen by setting XL(I)=XU(I). */
/*          INFO=6   In this case there is an error return because the */
/*     equality constraints and the bounds on the variables are found to */
/*     be inconsistent. */
/*          INFO=7   This value indicates that there is no vector of */
/*     variables that satisfies all of the constraints.  Specifically, */
/*     when this return or an INFO=6 return occurs, the current active */
/*     constraints (whose indices are IACT(K) K=1(1)NACT) prevent any */
/*     change in X(.) that reduces the sum of constraint violations, */
/*     where only bounds are included in this sum if INFO=6. */
/*          INFO=8   In this case the limit on the number of calls of */
/*     subroutine FGCALC (see below) has been reached, and there would */
/*     have been further calculation otherwise. */
/*  W(.) is a working space array of real variables that must be provided */
/*     by the user.  Its length must be at least (M+11*N+N**2).  On exit */
/*     from the package one can find the final components of GRAD(F) and */
/*     RESKT(.) in W(1),...,W(N) and W(N+1),...,W(2*N) respectively. */
/*  Note 1.   The variables N, M, MEQ, IA, ACC and IPRINT and the */
/*     elements of the arrays A(,.,), B(.), XL(.) and XU(.) are not */
/*     altered by the optimization procedure. Their values, the value of */
/*     INFO and the initial components of X(.) must be set on entry */
/*     to GETMIN. */
/*  Note 2.   Of course the package needs the objective function and its */
/*     gradient.  Therefore the user must provide a subroutine called */
/*     FGCALC, its first two lines being */
/*          SUBROUTINE FGCALC (N,X,F,G) */
/*          DIMENSION X(*),G(*)   . */
/*     It is called automatically with N set as above and with X(.) set */
/*     to a feasible vector of variables.  It should calculate the value */
/*     of the objective function and its gradient for this X(.) and */
/*     should set them in F and G(I) I=1(1)N respectively, without */
/*     disturbing N or any of the components of X(.). */
/*  Note 3.   A paper on the method of calculation and a report on the */
/*     main features of the computer code are available from the author */
/*     M.J.D.Powell (D.A.M.T.P., University of Cambridge, Silver Street, */
/*     Cambridge CB3 9EW, England). */

/*     Partition the workspace array. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --par;
    --w;

    /* Function Body */
    ig = 1;
    ireskt = ig + *n;
    iz = ireskt + *n;
    iu = iz + *n * *n;
    ixbig = iu + *n;
    ibres = ixbig + *n;
    id = ibres + *m + *n + *n;
    iztg = id + *n;
    igm = iztg + *n;
    ixs = igm + *n;
    igs = ixs + *n;

/*     Call the optimization package. */

    minflc_(n, m, meq, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], acc, &
	    iact[1], nact, &par[1], iprint, info, &w[ig], &w[iz], &w[iu], &w[
	    ixbig], &w[ireskt], &w[ibres], &w[id], &w[iztg], &w[igm], &w[ixs],
         &w[igs],p);
    return 0;
} /* getmin_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int initzu_(integer *n, integer *m, double *xl, 
	double *xu, double *x, integer *iact, integer *meql, integer *
	info, double *z__, double *u, double *xbig, double *
	relacc)
{
    /* System generated locals */
    integer i__1;
    double d__1;

    /* Local variables */
     integer jact, i__, j;
     double tempa, tempb, ztpar;
     integer nn, iz;


/*     Set RELACC. */

    /* Parameter adjustments */
    --xbig;
    --u;
    --z__;
    --iact;
    --x;
    --xu;
    --xl;

    /* Function Body */
    ztpar = (float)100.;
    *relacc = (float)1.;
L10:
    *relacc *= (float).5;
    tempa = ztpar + *relacc * (float).5;
    tempb = ztpar + *relacc;
    if (ztpar < tempa && tempa < tempb) {
	goto L10;
    }

/*     Seek bound inconsistencies and bound equality constraints. */

    *meql = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (xl[i__] > xu[i__]) {
	    goto L50;
	}
	if (xl[i__] == xu[i__]) {
	    ++(*meql);
	}
/* L20: */
    }

/*     Initialize U, Z and XBIG. */

    jact = 0;
    nn = *n * *n;
    i__1 = nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	z__[i__] = (float)0.;
    }
    iz = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (xl[i__] == xu[i__]) {
	    x[i__] = xu[i__];
	    ++jact;
	    u[jact] = (float)1.;
	    iact[jact] = i__ + *m + *n;
	    j = jact;
	} else {
	    j = i__ + *meql - jact;
	}
	z__[iz + j] = (float)1.;
	iz += *n;
/* L40: */
	xbig[i__] = (d__1 = x[i__], fabs(d__1));
    }
    *info = 1;
L50:
    return 0;
} /* initzu_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int ktvec_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, integer *nact, double *par, double *g, 
	double *reskt, double *z__, double *u, double *bres, 
	double *relaxf, integer *meql, double *ssqkt, double *
	parw, double *resktw)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1;

    /* Local variables */
     double temp;
     integer i__, j, k, icase, kk, jm, kl, iz;
     double ssqktw;


/*     Calculate the Lagrange parameters and the residual vector. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --par;
    --g;
    --reskt;
    --z__;
    --u;
    --bres;
    --parw;
    --resktw;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	reskt[i__] = g[i__];
    }
    if (*nact > 0) {
	icase = 0;
L20:
	i__1 = *nact;
	for (kk = 1; kk <= i__1; ++kk) {
	    k = *nact + 1 - kk;
	    j = iact[k];
	    temp = (float)0.;
	    iz = k;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		temp += z__[iz] * reskt[i__];
/* L30: */
		iz += *n;
	    }
	    temp *= u[k];
	    if (icase == 0) {
		par[k] = (float)0.;
	    }
	    if (k <= *meql || par[k] + temp < (float)0.) {
		par[k] += temp;
	    } else {
		temp = -par[k];
		par[k] = (float)0.;
	    }
	    if (temp != (float)0.) {
		if (j <= *m) {
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
/* L40: */
			reskt[i__] -= temp * a[i__ + j * a_dim1];
		    }
		} else {
		    jm = j - *m;
		    if (jm <= *n) {
			reskt[jm] += temp;
		    } else {
			reskt[jm - *n] -= temp;
		    }
		}
	    }
/* L50: */
	}

/*     Calculate the sum of squares of the KT residual vector. */

	*ssqkt = (float)0.;
	if (*nact == *n) {
	    goto L130;
	}
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L60: */
/* Computing 2nd power */
	    d__1 = reskt[i__];
	    *ssqkt += d__1 * d__1;
	}

/*     Apply iterative refinement to the residual vector. */

	if (icase == 0) {
	    icase = 1;
	    i__1 = *nact;
	    for (k = 1; k <= i__1; ++k) {
/* L70: */
		parw[k] = par[k];
	    }
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L80: */
		resktw[i__] = reskt[i__];
	    }
	    ssqktw = *ssqkt;
	    goto L20;
	}

/*     Undo the iterative refinement if it does not reduce SSQKT. */

	if (ssqktw < *ssqkt) {
	    i__1 = *nact;
	    for (k = 1; k <= i__1; ++k) {
/* L90: */
		par[k] = parw[k];
	    }
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L100: */
		reskt[i__] = resktw[i__];
	    }
	    *ssqkt = ssqktw;
	}

/*     Calculate SSQKT when there are no active constraints. */

    } else {
	*ssqkt = (float)0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L110: */
/* Computing 2nd power */
	    d__1 = g[i__];
	    *ssqkt += d__1 * d__1;
	}
    }

/*     Predict the reduction in F if one corrects any positive residuals */
/*       of active inequality constraints. */

    *relaxf = (float)0.;
    if (*meql < *nact) {
	kl = *meql + 1;
	i__1 = *nact;
	for (k = kl; k <= i__1; ++k) {
	    j = iact[k];
	    if (bres[j] > (float)0.) {
		*relaxf -= par[k] * bres[j];
	    }
/* L120: */
	}
    }
L130:
    return 0;
} /* ktvec_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int lsrch_(integer *n, double *x, double *g, 
	double *d__, double *xs, double *gs, double *relacc, 
	double *stepcb, double *ddotg, double *f, double *
	step, integer *nfvals, integer *nfmax, double *gopt,
    Problem *p)
{
    /* System generated locals */
    integer i__1;
    double d__1, d__2;

    /* Local variables */
     double fhgh, temp, flow, fopt;
     integer i__;
     double fbase, dghgh, dgmid, sbase, dglow, dgopt, ratio;
    extern /* Subroutine */ int fgcalc_(integer *, double *, double *,
         double *,Problem *);
     double ddotgb;
    integer isofar;
     double dgknot, relint, stphgh;
     integer icount;
    double stpmin, stplow, stpopt, amx;


/*     Initialization. */

    /* Parameter adjustments */
    --gopt;
    --gs;
    --xs;
    --d__;
    --g;
    --x;

    /* Function Body */
    relint = (float).9;
    icount = 0;
    ratio = (float)-1.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xs[i__] = x[i__];
	gs[i__] = g[i__];
	gopt[i__] = g[i__];
	if (d__[i__] != (float)0.) {
	    temp = (d__1 = x[i__] / d__[i__], fabs(d__1));
	    if (ratio < (float)0. || temp < ratio) {
		ratio = temp;
	    }
	}
/* L10: */
    }
    amx = (float)1.;
    *step = min(amx,*stepcb);
/* Computing MAX */
    d__1 = *relacc * ratio, d__2 = *step * (float)1e-19;
    stpmin = max(d__1,d__2);
    *step = max(stpmin,*step);
    sbase = (float)0.;
    fbase = *f;
    ddotgb = *ddotg;
    stplow = (float)0.;
    flow = *f;
    dglow = *ddotg;
    stphgh = (float)0.;
    stpopt = (float)0.;
    fopt = *f;
    dgopt = fabs(*ddotg);

/*     Calculate another function and gradient value. */

L20:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	x[i__] = xs[i__] + *step * d__[i__];
    }
    isofar = totcal_1.itnocs;
    fgcalc_(n, &x[1], f, &g[1],p);
    icount += totcal_1.itnocs - isofar;
    dgmid = (float)0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L40: */
	dgmid += d__[i__] * g[i__];
    }
    if (*f <= fopt) {
	if (*f < fopt || fabs(dgmid) < dgopt) {
	    stpopt = *step;
	    fopt = *f;
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
		gopt[i__] = g[i__];
	    }
	    dgopt = fabs(dgmid);
	}
    }
    if (*nfvals + icount == *nfmax) {
	goto L70;
    }

/*      Modify the bounds on the steplength or convergence. */

    if (*f >= fbase + (*step - sbase) * (float).1 * ddotgb) {
	if (stphgh > (float)0. || *f > fbase || dgmid > *ddotg * (float).5) {
	    stphgh = *step;
	    fhgh = *f;
	    dghgh = dgmid;
	    goto L60;
	}
	sbase = *step;
	fbase = *f;
	ddotgb = dgmid;
    }
    if (dgmid >= ddotgb * (float).7) {
	goto L70;
    }
    stplow = *step;
    flow = *f;
    dglow = dgmid;
L60:
    if (stphgh > (float)0. && stplow >= relint * stphgh) {
	goto L70;
    }

/*     Calculate the next step length or end the iterations. */

    if (stphgh == (float)0.) {
	if (*step == *stepcb) {
	    goto L70;
	}
	temp = (float)10.;
	if (dgmid > *ddotg * (float).9) {
	    temp = *ddotg / (*ddotg - dgmid);
	}
/* Computing MIN */
	d__1 = temp * *step;
	*step = min(d__1,*stepcb);
	goto L20;
    } else if (icount == 1 || stplow > (float)0.) {
	dgknot = (fhgh - flow) * (float)2. / (stphgh - stplow) - (dglow + 
		dghgh) * (float).5;
	if (dgknot >= (float)0.) {
	    amx = (float).1;
/* Computing MAX */
	    d__1 = amx, d__2 = dglow * (float).5 / (dglow - dgknot);
	    ratio = max(d__1,d__2);
	} else {
	    ratio = (dghgh * (float).5 - dgknot) / (dghgh - dgknot);
	}
	*step = stplow + ratio * (stphgh - stplow);
	goto L20;
    } else {
	*step *= (float).1;
	if (*step >= stpmin) {
	    goto L20;
	}
    }

/*     Return from subroutine. */

L70:
    if (*step != stpopt) {
	*step = stpopt;
	*f = fopt;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = xs[i__] + *step * d__[i__];
/* L80: */
	    g[i__] = gopt[i__];
	}
    }
    *nfvals += icount;
    return 0;
} /* lsrch_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int minflc_(integer *n, integer *m, integer *meq, double 
	*a, integer *ia, double *b, double *xl, double *xu, 
	double *x, double *acc, integer *iact, integer *nact, 
	double *par, integer *iprint, integer *info, double *g, 
	double *z__, double *u, double *xbig, double *reskt, 
	double *bres, double *d__, double *ztg, double *gm, 
	double *xs, double *gs,
    Problem *p)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1, d__2;

    /* Builtin functions */
    //integer s_wsfe(cilist *), e_wsfe();

    /* Local variables */
     integer meql, msat, mtot;
     double f;
     integer i__, k, iterc, nfmax, iexau;
    nfmax=5001;
     double relacc;
     integer mp;
    extern /* Subroutine */ int getfes_(integer *, integer *, double *, 
	    integer *, double *, double *, double *, double *,
	     integer *, integer *, double *, integer *, double *, 
	    double *, double *, double *, double *, 
	    double *, integer *, integer *, integer *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *), adjtol_(integer *, integer *, 
	    double *, integer *, double *, double *, double *,
	     double *, integer *, integer *, double *, double *, 
	    double *, integer *), eqcons_(integer *, integer *, integer *,
	     double *, integer *, double *, double *, integer *, 
	    integer *, integer *, double *, double *, double *, 
	    double *, double *);
     integer nfvals;
    extern /* Subroutine */ int minfun_(integer *, integer *, double *, 
	    integer *, double *, double *, double *, double *,
	     double *, integer *, integer *, double *, integer *, 
	    integer *, double *, double *, double *, double *,
	     double *, double *, double *, integer *, integer *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, integer *,
        Problem *), initzu_(integer *,
	    integer *, double *, double *, double *, integer *, 
	    integer *, integer *, double *, double *, double *, 
	    double *);
     double zznorm, amx, tol;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --par;
    --g;
    --z__;
    --u;
    --xbig;
    --reskt;
    --bres;
    --d__;
    --ztg;
    --gm;
    --xs;
    --gs;

    /* Function Body */
    zznorm = (float)-1.;
    iterc = 0;
    nfvals = 0;
    nfmax = 5001;
    if (*info > 0) {
	nfmax = *info;
    }

/*     Check the bounds on N, M and MEQ. */

    *info = 4;
/* Computing MAX */
    i__1 = 1 - *n, i__2 = *meq * (*meq - *m);
    if (max(i__1,i__2) > 0) {
	if (*iprint != 0) {
	    //io___56.ciunit = units_1.iuout;
	    //s_wsfe(&io___56);
	    //e_wsfe();
	}
	goto L40;
    }

/*     Initialize RELACC, Z, U and TOL. */

    initzu_(n, m, &xl[1], &xu[1], &x[1], &iact[1], &meql, info, &z__[1], &u[1]
	    , &xbig[1], &relacc);
    amx = (float).01;
/* Computing MAX */
    d__1 = amx, d__2 = relacc * (float)10.;
    tol = max(d__1,d__2);
    if (*info == 4) {
	if (*iprint != 0) {
	    //io___61.ciunit = units_1.iuout;
	    //s_wsfe(&io___61);
	    //e_wsfe();
	}
	goto L40;
    }

/*     Add any equality constraints to the active set. */

    if (*meq > 0) {
	eqcons_(n, m, meq, &a[a_offset], ia, &b[1], &xu[1], &iact[1], &meql, 
		info, &z__[1], &u[1], &relacc, &xs[1], &gs[1]);
	if (*info == 5) {
	    if (*iprint != 0) {
		//io___62.ciunit = units_1.iuout;
		//s_wsfe(&io___62);
		//e_wsfe();
	    }
	    goto L40;
	}
    }
    *nact = meql;
    msat = meql;

/*     Add the bounds to the list of constraints. */

    mtot = *nact;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (xl[i__] < xu[i__]) {
	    mtot += 2;
	    iact[mtot - 1] = *m + i__;
	    iact[mtot] = *m + *n + i__;
	}
/* L10: */
    }

/*     Try to satisfy the bound constraints. */

    getfes_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &iact[1], 
	    nact, &par[1], info, &g[1], &z__[1], &u[1], &xbig[1], &relacc, &
	    tol, &meql, &msat, &mtot, &bres[1], &d__[1], &ztg[1], &gm[1], &
	    reskt[1], &xs[1], &gs[1]);
    if (msat < mtot) {
	if (*iprint != 0) {
	    //io___66.ciunit = units_1.iuout;
	    //s_wsfe(&io___66);
	    //e_wsfe();
	}
	*info = 6;
	goto L40;
    }

/*     Add the ordinary inequalities to the list of constraints. */

    if (*m > *meq) {
	mp = *meq + 1;
	i__1 = *m;
	for (k = mp; k <= i__1; ++k) {
	    ++mtot;
/* L20: */
	    iact[mtot] = k;
	}
    }

/*     Correct any constraint violations. */

L30:
    getfes_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &iact[1], 
	    nact, &par[1], info, &g[1], &z__[1], &u[1], &xbig[1], &relacc, &
	    tol, &meql, &msat, &mtot, &bres[1], &d__[1], &ztg[1], &gm[1], &
	    reskt[1], &xs[1], &gs[1]);
    if (msat < mtot) {
	if (*iprint != 0) {
	    //io___69.ciunit = units_1.iuout;
	    //s_wsfe(&io___69);
	    //e_wsfe();
	}
	*info = 7;
	goto L40;
    } else if (meql == *n) {
	if (*iprint != 0) {
	    //io___70.ciunit = units_1.iuout;
	    //s_wsfe(&io___70);
	    //e_wsfe();
	}
	goto L40;
    }

/*     Minimize the objective function in the case when constraints are */
/*       treated as degenerate if their residuals are less than TOL. */

    iexau = 0;
    minfun_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], acc, &iact[
	    1], nact, &par[1], iprint, info, &g[1], &z__[1], &u[1], &xbig[1], 
	    &relacc, &zznorm, &tol, &meql, &mtot, &iterc, &nfvals, &nfmax, &
	    reskt[1], &bres[1], &d__[1], &ztg[1], &gm[1], &xs[1], &gs[1], &f, 
        &iexau,p);
    if (iexau != 0) {
	return 0;
    }

/*     Reduce TOL if necessary. */

    if (tol > relacc && *nact > 0) {
	if (nfvals < nfmax) {
	    adjtol_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &
		    iact[1], nact, &xbig[1], &relacc, &tol, &meql);
	    goto L30;
	} else {
	    *info = 8;
	}
    }
/*      IF (IPRINT .NE. 0) THEN */
/*          IF (INFO .EQ. 1) WRITE (IUOUT,1070) */
/* 1070     FORMAT (/5X,'GETMIN HAS ACHIEVED THE REQUIRED ACCURACY') */
/*          IF (INFO .EQ. 2) WRITE (IUOUT,1080) */
/* 1080     FORMAT (/5X,'GETMIN CAN MAKE NO FURTHER PROGRESS BECAUSE', */
/*     1      ' OF ROUNDING ERRORS') */
/*          IF (INFO .EQ. 3) WRITE (IUOUT,1090) */
/* 1090     FORMAT (/5X,'GETMIN CAN MAKE NO FURTHER PROGRESS BECAUSE', */
/*     1      ' F WILL NOT DECREASE ANY MORE') */
/*          IF (INFO .EQ. 8) WRITE (IUOUT,1100) */
/* 1100     FORMAT (/5X,'GETMIN HAS REACHED THE GIVEN LIMIT ON THE', */
/*     1      ' NUMBER OF CALLS OF FGCALC') */
/*      END IF */
L40:
    return 0;
} /* minflc_ */


/*  --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* Subroutine */ int minfun_(integer *n, integer *m, double *a, integer *
	ia, double *b, double *xl, double *xu, double *x, 
	double *acc, integer *iact, integer *nact, double *par, 
	integer *iprint, integer *info, double *g, double *z__, 
	double *u, double *xbig, double *relacc, double *
	zznorm, double *tol, integer *meql, integer *mtot, integer *iterc,
	 integer *nfvals, integer *nfmax, double *reskt, double *bres,
	 double *d__, double *ztg, double *gm, double *xs, 
	double *gs, double *f, integer *iexau,
    Problem *p)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    double d__1, d__2, d__3;

    /* Local variables */
     double diff;
    extern /* Subroutine */ int mdis_(double *, double *, integer *, 
	    integer *, integer *, integer *, integer *);
     integer msat;
     double step;
     integer i__, k;
    double ddotg;
    extern /* Subroutine */ int ktvec_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, integer *, double *, double *, double *)
	    , lsrch_(integer *, double *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, double *, integer *, integer *, 
        double *,Problem *);
    integer iterk;
    extern /* Subroutine */ int zbfgs_(integer *, double *, integer *, 
	    double *, double *, double *, double *, 
	    double *, double *);
     double fprev;
     integer iterp;
    double ssqkt;
    extern /* Subroutine */ int fgcalc_(integer *, double *, double *,
         double *,Problem *), addcon_(integer *, integer *, double *,
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *, double *, double *);
     integer indxbd;
    double stepcb;
    integer nfvalk, isofar;
    double relaxf;
    extern /* Subroutine */ int conres_(integer *, integer *, double *, 
	    integer *, double *, double *, double *, double *,
	     integer *, integer *, double *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, integer *, integer *, integer *, integer *, 
	    double *, double *, double *, double *);
    extern integer itconv_(double *);
    double sum;


/*     Initialize the minimization calculation. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --par;
    --g;
    --z__;
    --u;
    --xbig;
    --reskt;
    --bres;
    --d__;
    --ztg;
    --gm;
    --xs;
    --gs;

    /* Function Body */
    msat = *mtot;
    iterk = *iterc;
    nfvalk = *nfvals;
    if (*nfvals == 0 || *info == 1) {
	isofar = totcal_1.itnocs;
    fgcalc_(n, &x[1], f, &g[1],p);
	*nfvals += totcal_1.itnocs - isofar;
    }
    fprev = (d__1 = *f + *f + (float)1., fabs(d__1));
    iterp = -1;
    if (*iprint != 0) {
/*          WRITE (IUOUT,1000) TOL */
/* 1000     FORMAT (/5X,'NEW VALUE OF TOL =',1PE13.5) */
	iterp = *iterc;
    }

/*     Calculate the next search direction. */

L10:
    conres_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &iact[1], 
	    nact, &par[1], &g[1], &z__[1], &u[1], &xbig[1], &bres[1], &d__[1],
	     &ztg[1], relacc, tol, &stepcb, &ddotg, meql, &msat, mtot, &
	    indxbd, &gm[1], &reskt[1], &xs[1], &gs[1]);

/*     Calculate the Kuhn Tucker residual vector. */

    ktvec_(n, m, &a[a_offset], ia, &iact[1], nact, &par[1], &g[1], &reskt[1], 
	    &z__[1], &u[1], &bres[1], &relaxf, meql, &ssqkt, &xs[1], &gs[1]);

/*     Test for convergence. */

    if (ssqkt <= *acc * *acc) {
	*info = 1;
	goto L70;
    }
    if (ddotg >= (float)0.) {
	*info = 2;
	goto L70;
    }

/*     Test for termination due to no decrease in F. */

    if (*f >= fprev) {
	if (*tol == *relacc || *nact == 0) {
	    if (diff > (float)0.) {
		goto L20;
	    }
	}
	*info = 3;
	goto L70;
    }
L20:
    diff = fprev - *f;
    fprev = *f;

/*     Test that more calls of FGCALC are allowed. */

    if (*nfvals >= *nfmax) {
	*info = 8;
	goto L70;
    }

/*     Test whether to reduce TOL and to provide printing. */

/* Computing MAX */
    d__1 = diff, d__2 = ddotg * (float)-.5;
    if (*tol > *relacc && *iterc > iterk && relaxf * (float).1 >= max(d__1,
	    d__2)) {
	goto L70;
    }
    if (iterp == *iterc) {
	iterp = *iterc + iabs(*iprint);
	goto L80;
    }

/*     Calculate the step along the search direction. */

L40:
    ++(*iterc);
    lsrch_(n, &x[1], &g[1], &d__[1], &xs[1], &gs[1], relacc, &stepcb, &ddotg, 
        f, &step, nfvals, nfmax, &bres[1],p);
    if (step == (float)0.) {
	*info = 3;
	sum = (float)0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	    sum += (d__1 = d__[i__] * gs[i__], fabs(d__1));
	}
	if (ddotg + *relacc * sum >= (float)0.) {
	    *info = 2;
	}
	goto L70;
    }

/*     Revise XBIG. */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L60: */
/* Computing MAX */
	d__2 = xbig[i__], d__3 = (d__1 = x[i__], fabs(d__1));
	xbig[i__] = max(d__2,d__3);
    }

/*     Revise the second derivative approximation. */

    zbfgs_(n, &x[1], nact, &g[1], &z__[1], &ztg[1], &xs[1], &gs[1], zznorm);

/*     Add a constraint to the active set if it restricts the step. */

    if (step == stepcb) {
	k = iact[indxbd];
	if (k > *m) {
	    k -= *m;
	    if (k <= *n) {
		x[k] = xl[k];
	    } else {
		x[k - *n] = xu[k - *n];
	    }
	}
	addcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], 
		relacc, &indxbd, &xs[1], &gs[1]);
    }
    goto L10;

/*     Printing from the subroutine. */

L70:
    if (*iprint == 0) {
	goto L90;
    }
    iterp = -1;
L80:
    //mdis_(&x[1], f, iterc, nfvals, nfmax, iprint, &c__0);

/*  Did we reach our target value ? */
    /*
    if (itconv_(f) != 0) {
	*info = 9;
	*iexau = 1;
	return 0;
    }*/
/* L1010: */
/*      IF (NFVALS .GT. NFVALK) THEN */
/*          WRITE (*,1020) (X(I),I=1,N) */
/* 1020     FORMAT ('  X =',(1P5E14.5)) */
/*          WRITE (*,1030) (G(I),I=1,N) */
/* 1030     FORMAT ('  G =',(1P5E14.5)) */
/*      ELSE */
/*          WRITE (*,1040) */
/* 1040     FORMAT (5X,'NO CHANGE TO X AND G SINCE PREVIOUS OUTPUT') */
/*      END IF */
/*      IF (IPRINT .LT. 0) THEN */
/*          IF (NACT .EQ. 0) THEN */
/*              WRITE (*,1050) */
/* 1050         FORMAT (5X,'NO ACTIVE CONSTRAINTS') */
/*          ELSE */
/*              WRITE (*,1060) (IACT(I),I=1,NACT) */
/* 1060         FORMAT (' IA =',(14I5)) */
/*              WRITE (*,1070) (PAR(I),I=1,NACT) */
/* 1070         FORMAT (' LP =',(1P5E14.5)) */
/*          END IF */
/*          IF (NACT .EQ. N) THEN */
/*              WRITE (*,1080) */
/* 1080         FORMAT (5X,'KT RESIDUAL VECTOR IS ZERO') */
/*          ELSE */
/*              WRITE (*,1090) (RESKT(I),I=1,N) */
/* 1090         FORMAT (' KT =',(1P5E14.5)) */
/*          END IF */
/*      END IF */
    if (iterp >= 0) {
	goto L40;
    }
L90:
    return 0;
} /* minfun_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int newcon_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, integer *nact, double *z__, double *u, 
	double *d__, double *relacc, integer *mdeg, double *
	zzdiag, double *gmnew, double *cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1;

    /* Local variables */
     integer iadd;
     double temp, sumd;
    integer i__, j, k, khigh;
     double cviol, cvmax;
     integer jm;
    extern /* Subroutine */ int addcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *, double *, double *);
     integer np, iz;
     double savabs, sumabs, savsum;
     integer jmv;
     double sum;


/*     Initialization. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --z__;
    --u;
    --d__;
    --zzdiag;
    --gmnew;
    --cgrad;

    /* Function Body */
    np = *nact + 1;
    khigh = *mdeg;
    iz = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	zzdiag[i__] = (float)0.;
	i__2 = *n;
	for (j = np; j <= i__2; ++j) {
/* L10: */
/* Computing 2nd power */
	    d__1 = z__[iz + j];
	    zzdiag[i__] += d__1 * d__1;
	}
/* L20: */
	iz += *n;
    }

/*     Calculate the scalar products of D with its constraints. */

L30:
    cvmax = (float)0.;
    i__1 = khigh;
    for (k = np; k <= i__1; ++k) {
	j = iact[k];
	if (j <= *m) {
	    sum = (float)0.;
	    sumabs = (float)0.;
	    sumd = (float)0.;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		temp = d__[i__] * a[i__ + j * a_dim1];
		sum += temp;
		sumabs += fabs(temp);
/* L40: */
/* Computing 2nd power */
		d__1 = a[i__ + j * a_dim1];
		sumd += zzdiag[i__] * (d__1 * d__1);
	    }
	} else {
	    jm = j - *m;
	    if (jm <= *n) {
		sum = -d__[jm];
	    } else {
		jm -= *n;
		sum = d__[jm];
	    }
	    sumabs = fabs(sum);
	    sumd = zzdiag[jm];
	}

/*     Pick out the most violated constraint, or return if the */
/*       violation is negligible. */

	if (sum > *relacc * sumabs) {
	    cviol = sum * sum / sumd;
	    if (cviol > cvmax) {
		cvmax = cviol;
		iadd = k;
		savsum = sum;
		savabs = sumabs;
	    }
	}
/* L50: */
    }
    if (cvmax <= (float)0.) {
	goto L140;
    }
    if (*nact == 0) {
	goto L120;
    }

/*     Set GMNEW to the gradient of the most violated constraint. */

    j = iact[iadd];
    if (j <= *m) {
	jmv = 0;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L60: */
	    gmnew[i__] = a[i__ + j * a_dim1];
	}
    } else {
	jmv = j - *m;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L70: */
	    gmnew[i__] = (float)0.;
	}
	if (jmv <= *n) {
	    gmnew[jmv] = (float)-1.;
	} else {
	    jmv -= *n;
	    gmnew[jmv] = (float)1.;
	}
    }

/*     Modify GMNEW for the next active constraint. */

    k = *nact;
L80:
    temp = (float)0.;
    iz = k;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp += z__[iz] * gmnew[i__];
/* L90: */
	iz += *n;
    }
    temp *= u[k];
    j = iact[k];
    if (j <= *m) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L100: */
	    gmnew[i__] -= temp * a[i__ + j * a_dim1];
	}
    } else {
	jm = j - *m;
	if (jm <= *n) {
	    gmnew[jm] += temp;
	} else {
	    gmnew[jm - *n] -= temp;
	}
    }

/*     Revise the values of SAVSUM and SAVABS. */

    sum = (float)0.;
    sumabs = (float)0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = d__[i__] * gmnew[i__];
	sum += temp;
/* L110: */
	sumabs += fabs(temp);
    }
    savsum = min(savsum,sum);
    savabs = max(savabs,sumabs);
    --k;
    if (k >= 1) {
	goto L80;
    }

/*     Add the new constraint to the active set if the constraint */
/*       violation is still significant. */

    if (jmv > 0) {
	d__[jmv] = (float)0.;
    }
    if (savsum <= *relacc * savabs) {
	goto L130;
    }
L120:
    k = *nact;
    addcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], relacc, &
	    iadd, &gmnew[1], &cgrad[1]);
    if (*nact > k) {
	goto L140;
    }

/*     Seek another constraint violation. */

    iadd = np;
L130:
    if (np < khigh) {
	k = iact[khigh];
	iact[khigh] = iact[iadd];
	iact[iadd] = k;
	--khigh;
	goto L30;
    }
L140:
    return 0;
} /* newcon_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int satact_(integer *n, integer *m, double *a, integer *
	ia, double *b, double *xl, double *xu, double *x, 
	integer *iact, integer *nact, integer *info, double *z__, 
	double *u, double *xbig, double *relacc, double *tol, 
	integer *meql)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1, d__2, d__3;

    /* Local variables */
     double temp;
    integer i__, j, k;
     double scale, tempa;
     integer idrop;
     double savex;
     integer jx, iz;
    extern /* Subroutine */ int delcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *);
    double resbig, resabs, res;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --z__;
    --u;
    --xbig;

    /* Function Body */
    if (*nact == 0) {
	goto L50;
    }
    i__1 = *nact;
    for (k = 1; k <= i__1; ++k) {

/*     Calculate the next constraint residual. */

	j = iact[k];
	if (j <= *m) {
	    res = b[j];
	    resabs = (d__1 = b[j], fabs(d__1));
	    resbig = resabs;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		tempa = a[i__ + j * a_dim1];
		temp = tempa * x[i__];
		res -= temp;
		resabs += fabs(temp);
/* L10: */
		resbig += fabs(tempa) * xbig[i__];
	    }
	} else {
	    jx = j - *m;
	    if (jx <= *n) {
		res = x[jx] - xl[jx];
		resabs = (d__1 = x[jx], fabs(d__1)) + (d__2 = xl[jx], fabs(d__2)
			);
		resbig = xbig[jx] + (d__1 = xl[jx], fabs(d__1));
		savex = xl[jx];
	    } else {
		jx -= *n;
		res = xu[jx] - x[jx];
		resabs = (d__1 = x[jx], fabs(d__1)) + (d__2 = xu[jx], fabs(d__2)
			);
		resbig = xbig[jx] + (d__1 = xu[jx], fabs(d__1));
		savex = xu[jx];
	    }
	}

/*     Shift X if necessary. */

	if (res != (float)0.) {
	    temp = res / resabs;
	    if (k <= *meql) {
		temp = -fabs(temp);
	    }
	    if (*tol == *relacc || temp + *relacc < (float)0.) {
		*info = 1;
		scale = res * u[k];
		iz = k;
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    x[i__] += scale * z__[iz];
		    iz += *n;
/* L20: */
/* Computing MAX */
		    d__2 = xbig[i__], d__3 = (d__1 = x[i__], fabs(d__1));
		    xbig[i__] = max(d__2,d__3);
		}
		if (j > *m) {
		    x[jx] = savex;
		}

/*     Else flag a constraint deletion if necessary. */

	    } else if (res / resbig > *tol) {
		iact[k] = -iact[k];
	    }
	}
/* L30: */
    }

/*     Delete any flagged constraints and then return. */

    idrop = *nact;
L40:
    if (iact[idrop] < 0) {
	iact[idrop] = -iact[idrop];
	delcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], 
		relacc, &idrop);
    }
    --idrop;
    if (idrop > *meql) {
	goto L40;
    }
L50:
    return 0;
} /* satact_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int sdegen_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, integer *nact, double *par, double *z__, 
	double *u, double *d__, double *ztg, double *gm, 
	double *relacc, double *ddotgm, integer *meql, integer *mdeg, 
	double *gmnew, double *parnew, double *cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    double d__1;

    /* Local variables */
    double temp;
     integer i__, j, k;
     double theta, ratio;
    extern /* Subroutine */ int sdirn_(integer *, integer *, double *, 
	    double *, double *, double *, double *, 
	    double *);
    integer idrop;
    double dtest;
    integer itest, jm, mp, np, ku, iz;
    extern /* Subroutine */ int delcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *), newcon_(integer *, integer *, 
	    double *, integer *, integer *, integer *, double *, 
	    double *, double *, double *, integer *, double *,
	     double *, double *);
    double amx, sum;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --par;
    --z__;
    --u;
    --d__;
    --ztg;
    --gm;
    --gmnew;
    --parnew;
    --cgrad;

    /* Function Body */
    mp = *meql + 1;
    dtest = (float)0.;

/*     Calculate the search direction and branch if it is not downhill. */

L10:
    sdirn_(n, nact, &z__[1], &d__[1], &ztg[1], &gm[1], relacc, ddotgm);
    if (*ddotgm == (float)0.) {
	goto L120;
    }

/*     Branch if there is no need to consider any degenerate constraints. */
/*     The test gives termination if two consecutive additions to the */
/*       active set fail to increase the predicted new value of F. */

    if (*nact == *mdeg) {
	goto L120;
    }
    np = *nact + 1;
    sum = (float)0.;
    i__1 = *n;
    for (j = np; j <= i__1; ++j) {
/* L20: */
/* Computing 2nd power */
	d__1 = ztg[j];
	sum += d__1 * d__1;
    }
    if (dtest > (float)0. && sum >= dtest) {
	if (itest == 1) {
	    goto L120;
	}
	itest = 1;
    } else {
	dtest = sum;
	itest = 0;
    }

/*     Add a constraint to the active set if there are any significant */
/*       violations of degenerate constraints. */

    k = *nact;
    newcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], &d__[1], 
	    relacc, mdeg, &gmnew[1], &parnew[1], &cgrad[1]);
    if (*nact == k) {
	goto L120;
    }
    par[*nact] = (float)0.;

/*     Calculate the new reduced gradient and Lagrange parameters. */

L30:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L40: */
	gmnew[i__] = gm[i__];
    }
    k = *nact;
L50:
    temp = (float)0.;
    iz = k;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp += z__[iz] * gmnew[i__];
/* L60: */
	iz += *n;
    }
    temp *= u[k];
    parnew[k] = par[k] + temp;
    amx = (float)0.;
    if (k == *nact) {
/* Computing MIN */
	d__1 = parnew[k];
	parnew[k] = min(d__1,amx);
    }
    j = iact[k];
    if (j <= *m) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L70: */
	    gmnew[i__] -= temp * a[i__ + j * a_dim1];
	}
    } else {
	jm = j - *m;
	if (jm <= *n) {
	    gmnew[jm] += temp;
	} else {
	    gmnew[jm - *n] -= temp;
	}
    }
    --k;
    if (k > *meql) {
	goto L50;
    }

/*     Set RATIO for linear interpolation between PAR and PARNEW. */

    ratio = (float)0.;
    if (mp < *nact) {
	ku = *nact - 1;
	i__1 = ku;
	for (k = mp; k <= i__1; ++k) {
	    if (parnew[k] > (float)0.) {
		ratio = parnew[k] / (parnew[k] - par[k]);
		idrop = k;
	    }
/* L80: */
	}
    }

/*     Apply the linear interpolation. */

    theta = (float)1. - ratio;
    amx = (float)0.;
    i__1 = *nact;
    for (k = mp; k <= i__1; ++k) {
/* L90: */
/* Computing MIN */
	d__1 = theta * parnew[k] + ratio * par[k];
	par[k] = min(d__1,amx);
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L100: */
	gm[i__] = theta * gmnew[i__] + ratio * gm[i__];
    }

/*     Drop a constraint if RATIO is positive. */

    if (ratio > (float)0.) {
	delcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], 
		relacc, &idrop);
	i__1 = *nact;
	for (k = idrop; k <= i__1; ++k) {
/* L110: */
	    par[k] = par[k + 1];
	}
	goto L30;
    }

/*     Return if there is no freedom for a new search direction. */

    if (*nact < *n) {
	goto L10;
    }
    *ddotgm = (float)0.;
L120:
    return 0;
} /* sdegen_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int sdirn_(integer *n, integer *nact, double *z__, 
	double *d__, double *ztg, double *gm, double *relacc, 
	double *ddotgm)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
     double temp;
     integer i__, j, np, iz;
    double sumabs, sum;

    /* Parameter adjustments */
    --gm;
    --ztg;
    --d__;
    --z__;

    /* Function Body */
    *ddotgm = (float)0.;
    if (*nact >= *n) {
	goto L60;
    }

/*     Premultiply GM by the transpose of Z. */

    np = *nact + 1;
    i__1 = *n;
    for (j = np; j <= i__1; ++j) {
	sum = (float)0.;
	sumabs = (float)0.;
	iz = j;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    temp = z__[iz] * gm[i__];
	    sum += temp;
	    sumabs += fabs(temp);
/* L10: */
	    iz += *n;
	}
	if (fabs(sum) <= *relacc * sumabs) {
	    sum = (float)0.;
	}
/* L20: */
	ztg[j] = sum;
    }

/*     Form D by premultiplying ZTG by -Z. */

    iz = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sum = (float)0.;
	sumabs = (float)0.;
	i__2 = *n;
	for (j = np; j <= i__2; ++j) {
	    temp = z__[iz + j] * ztg[j];
	    sum -= temp;
/* L30: */
	    sumabs += fabs(temp);
	}
	if (fabs(sum) <= *relacc * sumabs) {
	    sum = (float)0.;
	}
	d__[i__] = sum;
/* L40: */
	iz += *n;
    }

/*     Test that the search direction is downhill. */

    sumabs = (float)0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp = d__[i__] * gm[i__];
	*ddotgm += temp;
/* L50: */
	sumabs += fabs(temp);
    }
    if (*ddotgm + *relacc * sumabs >= (float)0.) {
	*ddotgm = (float)0.;
    }
L60:
    return 0;
} /* sdirn_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int stepbd_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, double *bres, double *d__, double *
	stepcb, double *ddotg, integer *mdeg, integer *msat, integer *
	mtot, integer *indxbd)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    double temp;
     integer i__, j, k, iflag, jm, kl;
    double sp;


/*     Set steps to constraint boundaries and find the least */
/*     positive one. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --bres;
    --d__;

    /* Function Body */
    iflag = 0;
    *stepcb = (float)0.;
    *indxbd = 0;
    k = *mdeg;
L10:
    ++k;
    if (k > *mtot) {
	goto L700;
    }

/*     Form the scalar product of D with the current constraint normal. */

L20:
    j = iact[k];
    if (j <= *m) {
	sp = (float)0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	    sp += d__[i__] * a[i__ + j * a_dim1];
	}
    } else {
	jm = j - *m;
	if (jm <= *n) {
	    sp = -d__[jm];
	} else {
	    sp = d__[jm - *n];
	}
    }

/*     The next branch is taken if label 20 was reached via label 40. */

    if (iflag == 1) {
	goto L50;
    }

/*     Set BRES(J) to indicate the status of the j-th constraint. */

    if (sp * bres[j] <= (float)0.) {
	bres[j] = (float)0.;
    } else {
	bres[j] /= sp;
	if (*stepcb == (float)0. || bres[j] < *stepcb) {
	    *stepcb = bres[j];
	    *indxbd = k;
	}
    }
    goto L10;
L700:

/*     Try to pass through the boundary of a violated constraint. */

L40:
    if (*indxbd <= *msat) {
	goto L800;
    }
    iflag = 1;
    k = *indxbd;
    goto L20;
L50:
    ++(*msat);
    iact[*indxbd] = iact[*msat];
    iact[*msat] = j;
    bres[j] = (float)0.;
    *indxbd = *msat;
    *ddotg -= sp;
    if (*ddotg < (float)0. && *msat < *mtot) {

/*     Seek the next constraint boundary along the search direction. */

	temp = (float)0.;
	kl = *mdeg + 1;
	i__1 = *mtot;
	for (k = kl; k <= i__1; ++k) {
	    j = iact[k];
	    if (bres[j] > (float)0.) {
		if (temp == (float)0. || bres[j] < temp) {
		    temp = bres[j];
		    *indxbd = k;
		}
	    }
/* L60: */
	}
	if (temp > (float)0.) {
	    *stepcb = temp;
	    goto L40;
	}
    }
L800:
    return 0;
} /* stepbd_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int zbfgs_(integer *n, double *x, integer *nact, 
	double *g, double *z__, double *ztg, double *xs, 
	double *gs, double *zznorm)
{
    /* System generated locals */
    integer i__1, i__2;
    double d__1, d__2;

    /* Builtin functions */
    double sqrt(double);

    /* Local variables */
     double temp, wcos, wsin;
     integer i__, k;
     double dd, dg;
    integer km, kp, np, iz;
    double sum;


/*     Test if there is sufficient convexity for the update. */

    /* Parameter adjustments */
    --gs;
    --xs;
    --ztg;
    --z__;
    --g;
    --x;

    /* Function Body */
    dd = (float)0.;
    dg = (float)0.;
    temp = (float)0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xs[i__] = x[i__] - xs[i__];
/* Computing 2nd power */
	d__1 = xs[i__];
	dd += d__1 * d__1;
	temp += gs[i__] * xs[i__];
	gs[i__] = g[i__] - gs[i__];
/* L10: */
	dg += gs[i__] * xs[i__];
    }
    if (dg < fabs(temp) * (float).1) {
	goto L90;
    }

/*     Transform the Z matrix. */

    k = *n;
L20:
    kp = k;
    --k;
    if (k > *nact) {
	if (ztg[kp] == (float)0.) {
	    goto L20;
	}
/* Computing 2nd power */
	d__2 = ztg[k] / ztg[kp];
	temp = (d__1 = ztg[kp], fabs(d__1)) * sqrt(d__2 * d__2 + (float)1.);
	wcos = ztg[k] / temp;
	wsin = ztg[kp] / temp;
	ztg[k] = temp;
	iz = k;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    temp = wcos * z__[iz + 1] - wsin * z__[iz];
	    z__[iz] = wcos * z__[iz] + wsin * z__[iz + 1];
	    z__[iz + 1] = temp;
/* L30: */
	    iz += *n;
	}
	goto L20;
    }

/*     Update the value of ZZNORM. */

    if (*zznorm < (float)0.) {
	*zznorm = dd / dg;
    } else {
	temp = sqrt(*zznorm * dd / dg);
	*zznorm = min(*zznorm,temp);
/* Computing MAX */
	d__1 = *zznorm, d__2 = temp * (float).1;
	*zznorm = max(d__1,d__2);
    }

/*     Complete the updating of Z. */

    np = *nact + 1;
    temp = sqrt(dg);
    iz = np;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	z__[iz] = xs[i__] / temp;
/* L40: */
	iz += *n;
    }
    if (np < *n) {
	km = np + 1;
	i__1 = *n;
	for (k = km; k <= i__1; ++k) {
	    temp = (float)0.;
	    iz = k;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		temp += gs[i__] * z__[iz];
/* L50: */
		iz += *n;
	    }
	    temp /= dg;
	    sum = (float)0.;
	    iz = k;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		z__[iz] -= temp * xs[i__];
/* Computing 2nd power */
		d__1 = z__[iz];
		sum += d__1 * d__1;
/* L60: */
		iz += *n;
	    }
	    if (sum < *zznorm) {
		temp = sqrt(*zznorm / sum);
		iz = k;
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    z__[iz] = temp * z__[iz];
/* L70: */
		    iz += *n;
		}
	    }
/* L80: */
	}
    }
L90:
    return 0;
} /* zbfgs_ */

extern int getmin_(integer *n, integer *m, integer *meq, double 
	*a, integer *ia, double *b, double *xl, double *xu, 
	double *x, double *acc, integer *iact, integer *nact, 
	double *par, integer *iprint, integer *info, double *w,
    Problem *p);

double oldmin=1e+100;
#pragma omp threadprivate(oldmin)

double tolmin(Data &x, Problem *p,int iters)
{
	double fmin;
	oldmin=1e+100;
	totcal_1.itnocs =0 ;
    long n=p->getDimension();
	long m=0;
	long meq=0;
	double *a=new double[n*n];
	long ia=n;
	double b[n];
	double xl[n];
	double xu[n];
	double *xp=new double[n];
	Data x1,x2;
    x1.resize(p->getDimension());
    x2.resize(p->getDimension());
    x1=p->getLeftMargin();
    x2=p->getRightMargin();
	for(int i=0;i<n;i++) 
	{
        xp[i]=x[i];
        xl[i]=x1[i];
        xu[i]=x2[i];
	}
	double acc=1e-19;
	long iact[3*n];
	long nact;
	double par[n];
	long iprint=0;
    long info=iters;
	double *w=new double[12*n+n*n*n];
	
    getmin_(&n,&m,&meq,a,&ia,b,xl,xu,xp,&acc,iact,&nact,par,&iprint,&info,
            w,p);

    for(int i=0;i<n;i++) x[i]=xp[i];
    fmin=p->funmin(x);
	delete[] w;
	delete[] a;
	delete[] xp;
    return fmin;
}

int fgcalc_(long *n,double *x,double *f,double *g,Problem *p)
{
	totcal_1.itnocs++;

	Data Xa;
	Data Ga;
	Ga.resize(*n);
	Xa.resize(*n);
	for(int i=0;i<*n;i++) Xa[i]=x[i];
    *f=p->funmin(Xa);
    p->granal(Xa,Ga);
	{
        if(*f<=oldmin)
		{
    //    printf("NEW MIN[%d] =%20.10lg\n",totcal_1.itnocs,*f);
		 oldmin=*f;
		}
	}
	for(int i=0;i<*n;i++) g[i]=Ga[i];
	return 0;
}

/* Subroutine */ int getfes_(integer *n, integer *m, double *a, integer *
	ia, double *b, double *xl, double *xu, double *x, 
	integer *iact, integer *nact, double *par, integer *info, 
	double *g, double *z__, double *u, double *xbig, 
	double *relacc, double *tol, integer *meql, integer *msat, 
	integer *mtot, double *bres, double *d__, double *ztg, 
	double *gm, double *gmnew, double *parnew, double *
	cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    double d__1, d__2, d__3;

    /* Local variables */
     integer i__, msatk, itest;
    extern /* Subroutine */ int addcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *, double *, double *);
     integer indxbd;
    extern /* Subroutine */ int satact_(integer *, integer *, double *, 
	    integer *, double *, double *, double *, double *,
	     integer *, integer *, integer *, double *, double *, 
	    double *, double *, double *, integer *);
     double stepcb;
    extern /* Subroutine */ int adjtol_(integer *, integer *, double *, 
	    integer *, double *, double *, double *, double *,
	     integer *, integer *, double *, double *, double *, 
	    integer *), conres_(integer *, integer *, double *, integer *,
	     double *, double *, double *, double *, integer *
	    , integer *, double *, double *, double *, double 
	    *, double *, double *, double *, double *, 
	    double *, double *, double *, double *, integer *,
	     integer *, integer *, integer *, double *, double *, 
	    double *, double *);
    double sumres, sumrsk;


/*     Make the correction to X for the active constraints. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --par;
    --g;
    --z__;
    --u;
    --xbig;
    --bres;
    --d__;
    --ztg;
    --gm;
    --gmnew;
    --parnew;
    --cgrad;

    /* Function Body */
    *info = 0;
L10:
    satact_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &iact[1], 
	    nact, info, &z__[1], &u[1], &xbig[1], relacc, tol, meql);
    if (*info > 0) {
	*msat = *nact;
    }
    if (*msat == *mtot) {
	goto L60;
    }

/*     Try to correct the infeasibility. */

L20:
    msatk = *msat;
    sumrsk = (float)0.;
L30:
    conres_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &iact[1], 
	    nact, &par[1], &g[1], &z__[1], &u[1], &xbig[1], &bres[1], &d__[1],
	     &ztg[1], relacc, tol, &stepcb, &sumres, meql, msat, mtot, &
	    indxbd, &gm[1], &gmnew[1], &parnew[1], &cgrad[1]);

/*     Include the new constraint in the active set. */

    if (stepcb > (float)0.) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] += stepcb * d__[i__];
/* L40: */
/* Computing MAX */
	    d__2 = xbig[i__], d__3 = (d__1 = x[i__], fabs(d__1));
	    xbig[i__] = max(d__2,d__3);
	}
	addcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], 
		relacc, &indxbd, &gmnew[1], &cgrad[1]);
    }

/*     Test whether to continue the search for feasibility. */

    if (*msat < *mtot) {
	if (stepcb == (float)0.) {
	    goto L50;
	}
	if (msatk < *msat) {
	    goto L20;
	}
	if (sumrsk == (float)0. || sumres < sumrsk) {
	    sumrsk = sumres;
	    itest = 0;
	}
	++itest;
	if (itest <= 2) {
	    goto L30;
	}

/*     Reduce TOL if it may be too large to allow feasibility. */

L50:
	if (*tol > *relacc) {
	    adjtol_(n, m, &a[a_offset], ia, &b[1], &xl[1], &xu[1], &x[1], &
		    iact[1], nact, &xbig[1], relacc, tol, meql);
	    goto L10;
	}
    }
L60:
    return 0;
} /* getfes_ */

/* Subroutine */ int addcon_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, integer *nact, double *z__, double *u, 
	double *relacc, integer *indxbd, double *ztc, double *
	cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1, d__2;

    /* Builtin functions */
    double sqrt(double);

    /* Local variables */
     integer icon;
     double temp;
     integer ipiv;
     double wcos, wsin, wpiv;
    integer i__, j, iznbd;
     double tempa, tempb;
     integer jp, np, iz, inewbd;
     double sumabs, sum;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --z__;
    --u;
    --ztc;
    --cgrad;

    /* Function Body */
    np = *nact + 1;
    icon = iact[*indxbd];
    iact[*indxbd] = iact[np];
    iact[np] = icon;

/*     Form ZTC when the new constraint is a bound. */

    if (icon > *m) {
	inewbd = icon - *m;
	if (inewbd <= *n) {
	    temp = (float)-1.;
	} else {
	    inewbd -= *n;
	    temp = (float)1.;
	}
	iznbd = inewbd * *n - *n;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* L10: */
	    ztc[j] = temp * z__[iznbd + j];
	}

/*     Else form ZTC for an ordinary constraint. */

    } else {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L20: */
	    cgrad[i__] = a[i__ + icon * a_dim1];
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    ztc[j] = (float)0.;
	    iz = j;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		ztc[j] += z__[iz] * cgrad[i__];
/* L30: */
		iz += *n;
	    }
	}
    }

/*     Find any Givens rotations to apply to the last columns of Z. */

    j = *n;
L40:
    jp = j;
    --j;
    if (j > *nact) {
	if (ztc[jp] == (float)0.) {
	    goto L40;
	}
	if ((d__2 = ztc[jp], fabs(d__2)) <= *relacc * (d__1 = ztc[j], fabs(d__1)
		)) {
	    temp = (d__1 = ztc[j], fabs(d__1));
	} else if ((d__2 = ztc[j], fabs(d__2)) <= *relacc * (d__1 = ztc[jp], 
		fabs(d__1))) {
	    temp = (d__1 = ztc[jp], fabs(d__1));
	} else {
/* Computing 2nd power */
	    d__2 = ztc[j] / ztc[jp];
	    temp = (d__1 = ztc[jp], fabs(d__1)) * sqrt(d__2 * d__2 + (float)1.)
		    ;
	}
	wcos = ztc[j] / temp;
	wsin = ztc[jp] / temp;
	ztc[j] = temp;

/*     Apply the rotation when the new constraint is a bound. */

	iz = j;
	if (icon > *m) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		temp = wcos * z__[iz + 1] - wsin * z__[iz];
		z__[iz] = wcos * z__[iz] + wsin * z__[iz + 1];
		z__[iz + 1] = temp;
/* L50: */
		iz += *n;
	    }
	    z__[iznbd + jp] = (float)0.;

/*     Else apply the rotation for an ordinary constraint. */

	} else {
	    wpiv = (float)0.;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		tempa = wcos * z__[iz + 1];
		tempb = wsin * z__[iz];
		temp = (d__1 = cgrad[i__], fabs(d__1)) * (fabs(tempa) + fabs(
			tempb));
		if (temp > wpiv) {
		    wpiv = temp;
		    ipiv = i__;
		}
		z__[iz] = wcos * z__[iz] + wsin * z__[iz + 1];
		z__[iz + 1] = tempa - tempb;
/* L60: */
		iz += *n;
	    }

/*     Ensure orthogonality of Z(.,JP) to CGRAD. */

	    sum = (float)0.;
	    iz = jp;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		sum += z__[iz] * cgrad[i__];
/* L70: */
		iz += *n;
	    }
	    if (sum != (float)0.) {
		iz = ipiv * *n - *n + jp;
		z__[iz] -= sum / cgrad[ipiv];
	    }
	}
	goto L40;
    }

/*     Test for linear independence in the proposed new active set. */

    if (ztc[np] == (float)0.) {
	goto L90;
    }
    if (icon <= *m) {
	sum = (float)0.;
	sumabs = (float)0.;
	iz = np;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    temp = z__[iz] * cgrad[i__];
	    sum += temp;
	    sumabs += fabs(temp);
/* L80: */
	    iz += *n;
	}
	if (fabs(sum) <= *relacc * sumabs) {
	    goto L90;
	}
    }

/*     Set the new diagonal element of U and return. */

    u[np] = (float)1. / ztc[np];
    *nact = np;
L90:
    return 0;
} /* addcon_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int adjtol_(integer *n, integer *m, double *a, integer *
	ia, double *b, double *xl, double *xu, double *x, 
	integer *iact, integer *nact, double *xbig, double *relacc, 
	double *tol, integer *meql)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1, d__2;

    /* Local variables */
    double viol;
     integer i__, j, k, kl, jm;
     double resabs, res;


/*     Set VIOL to the greatest relative constraint residual of the first */
/*       NACT constraints. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --xbig;

    /* Function Body */
    viol = (float)0.;
    if (*nact > *meql) {
	kl = *meql + 1;
	i__1 = *nact;
	for (k = kl; k <= i__1; ++k) {
	    j = iact[k];
	    if (j <= *m) {
		res = b[j];
		resabs = (d__1 = b[j], fabs(d__1));
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    res -= a[i__ + j * a_dim1] * x[i__];
/* L10: */
		    resabs += (d__1 = a[i__ + j * a_dim1] * xbig[i__], fabs(
			    d__1));
		}
	    } else {
		jm = j - *m;
		if (jm <= *n) {
		    res = x[jm] - xl[jm];
		    resabs = xbig[jm] + (d__1 = xl[jm], fabs(d__1));
		} else {
		    jm -= *n;
		    res = xu[jm] - x[jm];
		    resabs = xbig[jm] + (d__1 = xu[jm], fabs(d__1));
		}
	    }
	    if (res > (float)0.) {
/* Computing MAX */
		d__1 = viol, d__2 = res / resabs;
		viol = max(d__1,d__2);
	    }
/* L20: */
	}
    }

/*     Adjust TOL. */

    *tol = min(*tol,viol) * (float).1;
    if (*tol <= *relacc + *relacc) {
	*tol = *relacc;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	    xbig[i__] = (d__1 = x[i__], fabs(d__1));
	}
    }
    return 0;
} /* adjtol_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int conres_(integer *n, integer *m, double *a, integer *
	ia, double *b, double *xl, double *xu, double *x, 
	integer *iact, integer *nact, double *par, double *g, 
	double *z__, double *u, double *xbig, double *bres, 
	double *d__, double *ztg, double *relacc, double *tol,
	 double *stepcb, double *sumres, integer *meql, integer *msat,
	 integer *mtot, integer *indxbd, double *gm, double *gmnew, 
	double *parnew, double *cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1, d__2;

    /* Local variables */
     integer mdeg;
    extern /* Subroutine */ int getd_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, integer *, integer *, double *, 
	    double *, double *, double *);
     double temp;
    integer i__, j, k, idiff;
     double ddotg;
     integer msatk, kl, jm;
     double resabs;
    extern /* Subroutine */ int stepbd_(integer *, integer *, double *, 
	    integer *, integer *, double *, double *, double *, 
	    double *, integer *, integer *, integer *, integer *);
     double res, sum;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xl;
    --xu;
    --x;
    --iact;
    --par;
    --g;
    --z__;
    --u;
    --xbig;
    --bres;
    --d__;
    --ztg;
    --gm;
    --gmnew;
    --parnew;
    --cgrad;

    /* Function Body */
    idiff = *mtot - *msat;

/*     Calculate and partition the residuals of the inactive constraints, */
/*       and set the gradient vector when seeking feasibility. */

    if ((real) idiff > (float)0.) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	    g[i__] = (float)0.;
	}
	*sumres = (float)0.;
    }
    msatk = *msat;
    mdeg = *nact;
    *msat = *nact;
    kl = *meql + 1;
    i__1 = *mtot;
    for (k = kl; k <= i__1; ++k) {
	j = iact[k];

/*     Calculate the residual of the current constraint. */

	if (j <= *m) {
	    res = b[j];
	    resabs = (d__1 = b[j], fabs(d__1));
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		res -= x[i__] * a[i__ + j * a_dim1];
/* L20: */
		resabs += (d__1 = xbig[i__] * a[i__ + j * a_dim1], fabs(d__1));
	    }
	} else {
	    jm = j - *m;
	    if (jm <= *n) {
		res = x[jm] - xl[jm];
		resabs = (d__1 = xbig[jm], fabs(d__1)) + (d__2 = xl[jm], fabs(
			d__2));
	    } else {
		jm -= *n;
		res = xu[jm] - x[jm];
		resabs = (d__1 = xbig[jm], fabs(d__1)) + (d__2 = xu[jm], fabs(
			d__2));
	    }
	}
	bres[j] = res;

/*     Set TEMP to the relative residual. */

	temp = (float)0.;
	if (resabs != (float)0.) {
	    temp = res / resabs;
	}
	if (k > msatk && temp < (float)0.) {
	    if (temp + *relacc >= (float)0.) {
		if (j <= *m) {
		    sum = (d__1 = b[j], fabs(d__1));
		    i__2 = *n;
		    for (i__ = 1; i__ <= i__2; ++i__) {
/* L30: */
			sum += (d__1 = x[i__] * a[i__ + j * a_dim1], fabs(d__1)
				);
		    }
		} else {
		    jm = j - *m;
		    if (jm <= *n) {
			sum = (d__1 = x[jm], fabs(d__1)) + (d__2 = xl[jm], fabs(
				d__2));
		    } else {
			sum = (d__1 = x[jm - *n], fabs(d__1)) + (d__2 = xu[jm 
				- *n], fabs(d__2));
		    }
		}
		if (fabs(res) <= sum * *relacc) {
		    temp = (float)0.;
		}
	    }
	}

/*     Place the residual in the appropriate position. */

	if (k <= *nact) {
	    goto L50;
	}
	if (k <= msatk || temp >= (float)0.) {
	    ++(*msat);
	    if (*msat < k) {
		iact[k] = iact[*msat];
	    }
	    if (temp > *tol) {
		iact[*msat] = j;
	    } else {
		++mdeg;
		iact[*msat] = iact[mdeg];
		iact[mdeg] = j;
	    }

/*     Update the gradient and SUMRES if the constraint is violated when */
/*       seeking feasibility. */

	} else {
	    if (j <= *m) {
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
/* L40: */
		    g[i__] += a[i__ + j * a_dim1];
		}
	    } else {
		j -= *m;
		if (j <= *n) {
		    g[j] += (float)-1.;
		} else {
		    g[j - *n] += (float)1.;
		}
	    }
	    *sumres += fabs(res);
	}
L50:
	;
    }

/*   Seek the next search direction unless CONRES was called from GETFES */
/*   and feasibility has been achieved. */

    *stepcb = (float)0.;
    if (idiff > 0 && *msat == *mtot) {
	goto L60;
    }
    getd_(n, m, &a[a_offset], ia, &iact[1], nact, &par[1], &g[1], &z__[1], &u[
	    1], &d__[1], &ztg[1], relacc, &ddotg, meql, &mdeg, &gm[1], &gmnew[
	    1], &parnew[1], &cgrad[1]);

/*     Calculate the (bound on the) step-length due to the constraints. */

    if (ddotg < (float)0.) {
	stepbd_(n, m, &a[a_offset], ia, &iact[1], &bres[1], &d__[1], stepcb, &
		ddotg, &mdeg, msat, mtot, indxbd);
    }
    if (idiff == 0) {
	*sumres = ddotg;
    }
L60:
    return 0;
} /* conres_ */

/* Subroutine */ int delcon_(integer *n, integer *m, double *a, integer *
	ia, integer *iact, integer *nact, double *z__, double *u, 
	double *relacc, integer *idrop)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1;

    /* Builtin functions */
  //  double sqrt(double);

    /* Local variables */
     integer icon, izbd;
    double rjjp, temp;
     integer ipiv;
     double wcos, wsin, wpiv;
     integer i__, j;
     double denom, tempa;
    integer isave;
     double tempb;
     integer jp, nm, iz, ibd;
     double ujp, sum;

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --z__;
    --u;

    /* Function Body */
    nm = *nact - 1;
    if (*idrop == *nact) {
	goto L60;
    }
    isave = iact[*idrop];

/*     Cycle through the constraint exchanges that are needed. */

    i__1 = nm;
    for (j = *idrop; j <= i__1; ++j) {
	jp = j + 1;
	icon = iact[jp];
	iact[j] = icon;

/*     Calculate the (J,JP) element of R. */

	if (icon <= *m) {
	    rjjp = (float)0.;
	    iz = j;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		rjjp += z__[iz] * a[i__ + icon * a_dim1];
/* L10: */
		iz += *n;
	    }
	} else {
	    ibd = icon - *m;
	    if (ibd <= *n) {
		izbd = ibd * *n - *n;
		rjjp = -z__[izbd + j];
	    } else {
		ibd -= *n;
		izbd = ibd * *n - *n;
		rjjp = z__[izbd + j];
	    }
	}

/*     Calculate the parameters of the next rotation. */

	ujp = u[jp];
	temp = rjjp * ujp;
	denom = fabs(temp);
	if (denom * *relacc < (float)1.) {
	    denom = sqrt(denom * denom + (float)1.);
	}
	wcos = temp / denom;
	wsin = (float)1. / denom;

/*     Rotate Z when a bound constraint is promoted. */

	iz = j;
	if (icon > *m) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		temp = wcos * z__[iz + 1] - wsin * z__[iz];
		z__[iz] = wcos * z__[iz] + wsin * z__[iz + 1];
		z__[iz + 1] = temp;
/* L20: */
		iz += *n;
	    }
	    z__[izbd + jp] = (float)0.;

/*     Rotate Z when an ordinary constraint is promoted. */

	} else {
	    wpiv = (float)0.;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		tempa = wcos * z__[iz + 1];
		tempb = wsin * z__[iz];
		temp = (d__1 = a[i__ + icon * a_dim1], fabs(d__1)) * (fabs(
			tempa) + fabs(tempb));
		if (temp > wpiv) {
		    wpiv = temp;
		    ipiv = i__;
		}
		z__[iz] = wcos * z__[iz] + wsin * z__[iz + 1];
		z__[iz + 1] = tempa - tempb;
/* L30: */
		iz += *n;
	    }

/*     Ensure orthogonality to promoted constraint. */

	    sum = (float)0.;
	    iz = jp;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		sum += z__[iz] * a[i__ + icon * a_dim1];
/* L40: */
		iz += *n;
	    }
	    if (sum != (float)0.) {
		iz = ipiv * *n - *n + jp;
		z__[iz] -= sum / a[ipiv + icon * a_dim1];
	    }
	}

/*     Set the new diagonal elements of U. */

	u[jp] = -denom * u[j];
	u[j] = ujp / denom;
/* L50: */
    }

/*     Return. */

    iact[*nact] = isave;
L60:
    *nact = nm;
    return 0;
} /* delcon_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int eqcons_(integer *n, integer *m, integer *meq, double 
	*a, integer *ia, double *b, double *xu, integer *iact, 
	integer *meql, integer *info, double *z__, double *u, 
	double *relacc, double *am, double *cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    double d__1;

    /* Local variables */
     integer i__, j, k;
     double vmult;
     integer jm;
    extern /* Subroutine */ int addcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *, double *, double *);
    integer np, iz;
    double sumabs;
    integer keq;
    double rhs, sum;


/*     Try to add the next equality constraint to the active set. */

    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --b;
    --xu;
    --iact;
    --z__;
    --u;
    --am;
    --cgrad;

    /* Function Body */
    i__1 = *meq;
    for (keq = 1; keq <= i__1; ++keq) {
	if (*meql < *n) {
	    np = *meql + 1;
	    iact[np] = keq;
	    addcon_(n, m, &a[a_offset], ia, &iact[1], meql, &z__[1], &u[1], 
		    relacc, &np, &am[1], &cgrad[1]);
	    if (*meql == np) {
		goto L50;
	    }
	}

/*     If linear dependence occurs then find the multipliers of the */
/*       dependence relation and apply them to the right hand sides. */

	sum = b[keq];
	sumabs = (d__1 = b[keq], fabs(d__1));
	if (*meql > 0) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/* L10: */
		am[i__] = a[i__ + keq * a_dim1];
	    }
	    k = *meql;
L20:
	    vmult = (float)0.;
	    iz = k;
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		vmult += z__[iz] * am[i__];
/* L30: */
		iz += *n;
	    }
	    vmult *= u[k];
	    j = iact[k];
	    if (j <= *m) {
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
/* L40: */
		    am[i__] -= vmult * a[i__ + j * a_dim1];
		}
		rhs = b[j];
	    } else {
		jm = j - *m - *n;
		am[jm] -= vmult;
		rhs = xu[jm];
	    }
	    sum -= rhs * vmult;
	    sumabs += (d__1 = rhs * vmult, fabs(d__1));
	    --k;
	    if (k >= 1) {
		goto L20;
	    }
	}

/*     Error return if the constraints are inconsistent. */

	if (fabs(sum) > *relacc * sumabs) {
	    *info = 5;
	    goto L60;
	}
L50:
	;
    }
L60:
    return 0;
} /* eqcons_ */


/*  --------------------------------------------------------------------- */

/* Subroutine */ int getd_(integer *n, integer *m, double *a, integer *ia,
	 integer *iact, integer *nact, double *par, double *g, 
	double *z__, double *u, double *d__, double *ztg, 
	double *relacc, double *ddotg, integer *meql, integer *mdeg, 
	double *gm, double *gmnew, double *parnew, double *
	cgrad)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
     double temp;
    integer i__, j, k, ii, jm, iz;
    extern /* Subroutine */ int delcon_(integer *, integer *, double *, 
	    integer *, integer *, integer *, double *, double *, 
	    double *, integer *), sdegen_(integer *, integer *, 
	    double *, integer *, integer *, integer *, double *, 
	    double *, double *, double *, double *, 
	    double *, double *, double *, integer *, integer *, 
	    double *, double *, double *);
     double ddotgm, abcd;


/*     Initialize GM and cycle backwards through the active set. */

/* C dgp */
    /* Parameter adjustments */
    a_dim1 = *ia;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --iact;
    --par;
    --g;
    --z__;
    --u;
    --d__;
    --ztg;
    --gm;
    --gmnew;
    --parnew;
    --cgrad;

    /* Function Body */
    abcd = (float)1.;
L10:
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
/* L20: */
	gm[ii] = g[ii];
    }
    k = *nact;
L30:
    if (k > 0) {

/*     Set TEMP to the next multiplier, but reduce the active set if */
/*       TEMP has an unacceptable sign. */

	temp = (float)0.;
	iz = k;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    temp += z__[iz] * gm[i__];
/* L40: */
	    iz += *n;
	}
	temp *= u[k];
	if (k > *meql && temp > (float)0.) {
	    delcon_(n, m, &a[a_offset], ia, &iact[1], nact, &z__[1], &u[1], 
		    relacc, &k);
	    goto L10;
	}

/*     Update GM using the multiplier that has just been calculated. */

	j = iact[k];
	if (j <= *m) {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
		gm[i__] -= temp * a[i__ + j * a_dim1];
	    }
	} else {
	    jm = j - *m;
	    if (jm <= *n) {
		gm[jm] += temp;
	    } else {
		gm[jm - *n] -= temp;
	    }
	}
	par[k] = temp;
	--k;
	goto L30;
    }

/*     Calculate the search direction and DDOTG. */

    *ddotg = (float)0.;
    if (*nact < *n) {
	sdegen_(n, m, &a[a_offset], ia, &iact[1], nact, &par[1], &z__[1], &u[
		1], &d__[1], &ztg[1], &gm[1], relacc, &ddotgm, meql, mdeg, &
		gmnew[1], &parnew[1], &cgrad[1]);
	if (ddotgm < (float)0.) {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L60: */
		*ddotg += d__[i__] * g[i__];
	    }
	}
    }
    return 0;
} /* getd_ */


/*  --------------------------------------------------------------------- */

#ifdef __cplusplus
	}
#endif

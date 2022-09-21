# ifndef __TOLMIN__H
# define __TOLMIN__H
# include "problem.h"


#define totcal_1 totcal_
#define units_1 units_

class Tolmin
{
private:
    Problem *myProblem;
    int ifail;
    double oldMin;
    integer c__0 ;
    int getmin_(integer *n, integer *m, integer *meq, double
        *a, integer *ia, double *b, double *xl, double *xu,
        double *x, double *acc, integer *iact, integer *nact,
        double *par, integer *iprint, integer *info, double *w);
    int initzu_(integer *n, integer *m, double *xl,
        double *xu, double *x, integer *iact, integer *meql, integer *
        info, double *z__, double *u, double *xbig, double *
        relacc);
    int ktvec_(integer *n, integer *m, double *a, integer *
        ia, integer *iact, integer *nact, double *par, double *g,
        double *reskt, double *z__, double *u, double *bres,
        double *relaxf, integer *meql, double *ssqkt, double *
        parw, double *resktw);
    int lsrch_(integer *n, double *x, double *g,
        double *d__, double *xs, double *gs, double *relacc,
        double *stepcb, double *ddotg, double *f, double *
        step, integer *nfvals, integer *nfmax, double *gopt);
    int minflc_(integer *n, integer *m, integer *meq, double
        *a, integer *ia, double *b, double *xl, double *xu,
        double *x, double *acc, integer *iact, integer *nact,
        double *par, integer *iprint, integer *info, double *g,
        double *z__, double *u, double *xbig, double *reskt,
        double *bres, double *d__, double *ztg, double *gm,
        double *xs, double *gs);

    int minfun_(integer *n, integer *m, double *a, integer *
        ia, double *b, double *xl, double *xu, double *x,
        double *acc, integer *iact, integer *nact, double *par,
        integer *iprint, integer *info, double *g, double *z__,
        double *u, double *xbig, double *relacc, double *
        zznorm, double *tol, integer *meql, integer *mtot, integer *iterc,
         integer *nfvals, integer *nfmax, double *reskt, double *bres,
         double *d__, double *ztg, double *gm, double *xs,
        double *gs, double *f, integer *iexau);

    int newcon_(integer *n, integer *m, double *a, integer *
        ia, integer *iact, integer *nact, double *z__, double *u,
        double *d__, double *relacc, integer *mdeg, double *
        zzdiag, double *gmnew, double *cgrad);

    int satact_(integer *n, integer *m, double *a, integer *
        ia, double *b, double *xl, double *xu, double *x,
        integer *iact, integer *nact, integer *info, double *z__,
        double *u, double *xbig, double *relacc, double *tol,
        integer *meql);

    int sdegen_(integer *n, integer *m, double *a, integer *
       ia, integer *iact, integer *nact, double *par, double *z__,
       double *u, double *d__, double *ztg, double *gm,
       double *relacc, double *ddotgm, integer *meql, integer *mdeg,
       double *gmnew, double *parnew, double *cgrad);

    int sdirn_(integer *n, integer *nact, double *z__,
        double *d__, double *ztg, double *gm, double *relacc,
        double *ddotgm);

    int stepbd_(integer *n, integer *m, double *a, integer *
        ia, integer *iact, double *bres, double *d__, double *
        stepcb, double *ddotg, integer *mdeg, integer *msat, integer *
        mtot, integer *indxbd);

    int zbfgs_(integer *n, double *x, integer *nact,
       double *g, double *z__, double *ztg, double *xs,
       double *gs, double *zznorm);

    int getfes_(integer *n, integer *m, double *a, integer *
        ia, double *b, double *xl, double *xu, double *x,
        integer *iact, integer *nact, double *par, integer *info,
        double *g, double *z__, double *u, double *xbig,
        double *relacc, double *tol, integer *meql, integer *msat,
        integer *mtot, double *bres, double *d__, double *ztg,
        double *gm, double *gmnew, double *parnew, double *
        cgrad);

    int addcon_(integer *n, integer *m, double *a, integer *
        ia, integer *iact, integer *nact, double *z__, double *u,
        double *relacc, integer *indxbd, double *ztc, double *
        cgrad);

    int adjtol_(integer *n, integer *m, double *a, integer *
        ia, double *b, double *xl, double *xu, double *x,
        integer *iact, integer *nact, double *xbig, double *relacc,
        double *tol, integer *meql);

    int conres_(integer *n, integer *m, double *a, integer *
        ia, double *b, double *xl, double *xu, double *x,
        integer *iact, integer *nact, double *par, double *g,
        double *z__, double *u, double *xbig, double *bres,
        double *d__, double *ztg, double *relacc, double *tol,
         double *stepcb, double *sumres, integer *meql, integer *msat,
         integer *mtot, integer *indxbd, double *gm, double *gmnew,
        double *parnew, double *cgrad);

    int delcon_(integer *n, integer *m, double *a, integer *
       ia, integer *iact, integer *nact, double *z__, double *u,
       double *relacc, integer *idrop);

    int eqcons_(integer *n, integer *m, integer *meq, double
       *a, integer *ia, double *b, double *xu, integer *iact,
       integer *meql, integer *info, double *z__, double *u,
       double *relacc, double *am, double *cgrad);

    int getd_(integer *n, integer *m, double *a, integer *ia,
         integer *iact, integer *nact, double *par, double *g,
        double *z__, double *u, double *d__, double *ztg,
        double *relacc, double *ddotg, integer *meql, integer *mdeg,
        double *gm, double *gmnew, double *parnew, double *
        cgrad);

    int fgcalc_(integer *, double *, double *,double *);
    struct {
        integer itnocs, ipartc, itder, ipder, ithess, iphess, itjac, ipjac;
    } totcal_;


    struct {
        integer iuinp, iuout;
    } units_;

double *a,*xl,*xu,*xp,*b;
long *iact;
double *par,*w;
bool hasDebug = false;
public:
    Tolmin(Problem *p,Data &left,Data &right);
    Tolmin(Problem *p);
    double Solve(Data &x,bool debug,int iters=2001);
    ~Tolmin();
};


# endif

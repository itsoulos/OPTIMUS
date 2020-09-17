# ifndef __TOLMIN__H
# define __TOLMIN__H
# include "problem.h"

#ifdef __cplusplus
extern "C"{
# endif

extern double	tolmin(Data &x,Problem *p,int iters=2001);

#ifdef __cplusplus
}
#endif

# endif

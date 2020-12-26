# ifndef __TOLMIN__H
# define __TOLMIN__H

# include <neuralprogram.h>
typedef struct
{
	NeuralProgram *problem;
	int	      iters;
}MinInfo;
#ifdef __cplusplus
extern "C"{
# endif

extern double	tolmin(Data &x,MinInfo &Info);

#ifdef __cplusplus
}
#endif

# endif

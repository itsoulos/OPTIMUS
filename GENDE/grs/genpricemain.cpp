# include <stdio.h>
# include <problem.h>
# include <genprice.h>
# include <math.h>
# include <unistd.h>
# include <sys/times.h>
# include <get_options_price.h>
# include <string.h>

# define BATCH
extern "C"
{
	extern int getdimension();
	extern void getleftmargin(double *x);
	extern void getrightmargin(double *x);
	extern double funmin(double *x);
	extern void   granal(double *x,double *g);
}
int problem_dimension;

double	*tempx,*tempg;

class MyProblem :public Problem
{
	public:
		MyProblem(int N);
		virtual double funmin(Data x);
		virtual void   granal(Data x,Data &g);
};

MyProblem::MyProblem(int N)
	:Problem(N)
{
}


double	MyProblem::funmin(Data x)
{
	++fevals;
	for(int i=0;i<problem_dimension;i++) tempx[i]=x[i];
	return ::funmin(tempx);
}

void	MyProblem::granal(Data x,Data &g)
{
	++gevals;
	for(int i=0;i<problem_dimension;i++) tempx[i]=x[i];
	::granal(tempx,tempg);
	for(int i=0;i<problem_dimension;i++) g[i]=tempg[i];
}

int main(int argc, char **argv)
{
	strcpy(outputfile,"");
	parse_cmd_line(argc,argv);
	srand(0);
	srand48(random_seed);

	problem_dimension=getdimension();
	tempx=new double[problem_dimension];
	tempg=new double[problem_dimension];
	MyProblem myproblem(problem_dimension);
	double *l,*r;
	l=new double[problem_dimension];
	r=new double[problem_dimension];
	getleftmargin(l);
	getrightmargin(r);
	Data L,R;
	L.resize(problem_dimension);
	R.resize(problem_dimension);
	for(int i=0;i<problem_dimension;i++)
	{
		L[i]=l[i];
		R[i]=r[i];
	}
	myproblem.setLeftMargin(L);
	myproblem.setRightMargin(R);
	delete[] l;
	delete[] r;
	GenPrice opt(&myproblem);
	opt.Solve();
	if(strlen(outputfile))
	{
		FILE *fp=fopen(outputfile,"w");
		if(!fp) return 1;
		Data x;
		double y;
		x.resize(problem_dimension);
		opt.getMinimum(x,y);
		fprintf(fp,"FUNCTION EVALUATIONS = %d\n",myproblem.getfevals());
		fprintf(fp,"GRADIENT EVALUATIONS = %d\n",myproblem.getgevals());
		fprintf(fp,"MINIMUM = ");
		for(int i=0;i<problem_dimension;i++)
		{
			fprintf(fp," %lf ",x[i]);	
		}
		fprintf(fp," %lf \n",y);
		fclose(fp);
	}
	else
	{
		Data x;
		double y;
		x.resize(problem_dimension);
		opt.getMinimum(x,y);
		printf("FUNCTION EVALUATIONS = %d\n",myproblem.getfevals());
		printf("GRADIENT EVALUATIONS = %d\n",myproblem.getgevals());
		printf("MINIMUM = ");
		for(int i=0;i<problem_dimension;i++)
		{
			printf(" %lf ",x[i]);	
		}
		printf(" %lf \n",y);
	}
	delete[] tempx;
	delete[] tempg;
	return 0;
}

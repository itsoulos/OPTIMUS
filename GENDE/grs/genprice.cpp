#include <genprice.h>
#include <grs.h>
#include <tolmin.h>
#include <math.h>
#include <get_options_price.h>

vector<double> minima;
int changeFlag = 0;

double getMean(vector<double> &x)
{
	double s=0.0;
	for(int i=0;i<x.size();i++) s+=x[i];
	return s/x.size();
}

double getVariance(vector<double> &x)
{
	double m= getMean(x);
	double s=0.0;
	for(int i=0;i<x.size();i++)
		s+=pow(x[i]-m,2.0);
	return s/x.size();
}

GenPrice::GenPrice(Problem *p)
{
	problem = p;
	M = 25 * p->getDimension();
	//M = 25;
	xmin.resize(p->getDimension());
	xmax.resize(p->getDimension());
	sample = new Collection(p->getDimension());
	center.resize(problem->getDimension());
}

bool	GenPrice::check()
{

	double fm = (fmin<oldBesty)>1e-4;//fabs(1.0 + fabs(fmin));

	x1 = x1 + fm;
	x2 = x2 + fm * fm;
	variance = x2 / (miters+1 ) - (x1 / (miters +1)) * (x1 / (miters+1 ));
	variance = fabs(variance);
	miters++;
	if(variance<1e-8 &&miters<3) variance=x1/miters;
	if (fmin < oldBesty)
	{
		oldBesty = fmin;
		stopat = variance / 2.0;
	}
	//printf("GCRS. Iteration: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n", miters, fmin, variance, stopat);
	if (variance <= stopat && miters >= 1000)
	{

		MinInfo Info;
		Info.p = problem;
		double f = problem->funmin(xmin);
		fmin = tolmin(xmin, Info);
		return true;
	}
	return false;
}

double	GenPrice::getFailPercent() const
{
	return countFail * 100.0/countEnter;
}

void GenPrice::makeSample2()
{
	int dimension = problem->getDimension();
	Data x;
	x.resize(dimension);
	double y;
	vector<int> index;
	index.resize(dimension + 1);
	for (int i = 0; i < dimension + 1; i++)
		index[i] = -1;
	sample2->clear();
	for (int i = 0; i < dimension + 1; i++)
	{
		int r, flag;
		do
		{
			r = rand() % M;
			flag = 0;
			for (int j = 0; j < i; j++)
			{
				if (index[j] == r)
				{
					flag = 1;
					break;
				}
			}
		} while (flag);
		index[i] = r;
		sample->getSampleX(r, x);
		y = sample->getSampleY(r);
		sample2->addSample(x, y);
	}
}

Data	GenPrice::getNewPoint()
{
	bool useFirstCenter=false; //original algorithm
	bool useMinCenter=false; //proposed by Ali
	bool useSimpleCenter=false; //proposed by Charilogis
	int dimension = problem->getDimension();
	Data xk;
	xk.resize(dimension);
	Data x;
	x.resize(dimension);
	for(int i=0;i<dimension;i++) center[i]=0.0;

	if(useFirstCenter /Home/charilog/Desktop/GenPrice/)
	{
		for(int i=0;i<dimension;i++)
		{
			sample2->getSampleX(i,x);
			for (int j = 0; j < dimension; j++)
				center[j] = center[j] + 1.0 / dimension * x[j];
		}
		sample2->getSampleX(dimension, x);
		for (int i = 0; i < dimension; i++)
			xk[i] = 2.0 * center[i] - x[i];
	}
	else
	
	if(useMinCenter)
	{	
		for(int i=1;i<dimension;i++)
		{
			sample2->getSampleX(i,x);
			for (int j = 0; j < dimension; j++)
				center[j] = center[j] + 1.0 / dimension * x[j];
			for(int j=0;j<dimension;j++)
				center[j]+=xmin[j]/dimension;
		}
		sample2->getSampleX(dimension, x);
		for (int i = 0; i < dimension; i++)
			xk[i] = 2.0 * center[i] - x[i];
	}
	else
	if(useSimpleCenter)
	{

		for (int i = 0; i < dimension; i++)
		{
			sample2->getSampleX(i, x);
			//if (i != dimension)
			for (int j = 0; j < dimension; j++)
				center[j] += x[j] * (1.0 / dimension);
		}
		for (int j = 0; j < dimension; j++)
			center[j] += xmin[j] * 1.0 / dimension;
		sample2->getSampleX(dimension, x);
		for (int i = 0; i < dimension; i++)
			xk[i] = center[i] - (x[i] / dimension);
	}
	return xk;
}

void GenPrice::Solve()
{
	bool newprice=false;
	bool usegrs=false;
	changeFlag=0;
	int dimension = problem->getDimension();
	sample2 = new Collection(dimension);
	vector<int> index;
	index.resize(dimension + 1);
	Data xk;
	xk.resize(dimension);
	double fk;
	int success = 0;
	int posmax = 0;
	int posmin = 0;
	Grs *Solver = new Grs(problem);
	Solver->setGenomeCount(genome_count);
	Solver->setGenomeLength(10 * problem->getDimension());
	countFail=0;
	countEnter=0;
	/**/
	double oldymin = -1e+100;
	int run_flag = 0;
	x1=0;
	x2=0;
	miters=0;
	oldBesty=1e+100;
	iters=0;
	stopat=0;
	variance=0;
	int repeatedFailure=0;
	/*
	 * */
		MinInfo Info;
		Info.p=problem;
step0:
	iters = 1;
	Data x;
	x.resize(dimension);
	double y;
	for (int i = 0; i < M; i++)
	{
		problem->getSample(x);
		y = problem->funmin(x);
		sample->addSample(x, y);
	}
step1:
	fmax = -1e+100;
	fmin = 1e+100;
	for (int i = 0; i < M; i++)
	{
		y = sample->getSampleY(i);
		if (y < fmin)
		{
			posmin = i;
			fmin = y;
			sample->getSampleX(i, xmin);
		}
		if (y > fmax)
		{
			posmax = i;
			fmax = y;
			sample->getSampleX(i, xmax);
		}
	}
	if(newprice)
{
	if(check()) 
		{

		delete Solver;
		delete sample2;
			return;
		}
}
	if (iters >= 100000)
	{
		delete Solver;
		MinInfo Info;
		Info.p = problem;
		double f = problem->funmin(xmin);
		fmin = tolmin(xmin, Info);
		delete sample2;
		return;
	}
//		printf("iters=%d fmin=%.10lg diff=%.10lg\n",iters,fmin,fabs(fmax-fmin));
	if(fabs(fmax-fmin)<1e-6)
	{
		delete Solver;
		double f= problem->funmin(xmin);
		fmin=tolmin(xmin,Info);
		delete sample2;
		return;
	}
	
	
	
	repeatedFailure=0;
step2:
	makeSample2();
	xk=getNewPoint();
	countEnter++;
	if (!problem->isPointIn(xk))
	{
		countFail++;
		repeatedFailure++;
		changeFlag = !changeFlag;
		if(repeatedFailure>=5) xk = xmin;
		else
		goto step2;
	}
	fk = problem->funmin(xk);
	if(usegrs)
	Solver->Solve(xk,fk);
	success += (fk <= fmax);
	if (fk <= fmax)
	{
		sample->replaceSample(posmax, xk, fk);
		iters++;
		goto step1;
	}

step3:
	goto step2;
	if(!newprice ) goto step2;
	if (fk > fmax)
	{
		if (success * 1.0 / iters > 0.5)
		{
			iters++;
			goto step2;
		}
		else
		{
			for (int i = 0; i < dimension; i++)
				xk[i] = 0.5 * center[i] + 0.5 * x[i];
			if (!problem->isPointIn(xk))
				goto step2;
			fk = problem->funmin(xk);
	fk=tolmin(xk,Info,10);	
			success += (fk <= fmax);
			if (fk < fmax)
			{
				sample->replaceSample(posmax, xk, fk);
				iters++;
				goto step1;
			}
			else
			{
				iters++;
				goto step2;
			}
		}
	}
step4:
	if (fk <= fmax)
	{
		sample->replaceSample(posmax, xk, fk);
		iters++;
		goto step1;
	}
}

void GenPrice::getMinimum(Data &x, double &y)
{
	x = xmin;
	y = fmin;
}

int GenPrice::getM() const
{
	return M;
}

void GenPrice::setM(int m)
{
	M = m;
}

GenPrice::~GenPrice()
{
	delete sample;
}

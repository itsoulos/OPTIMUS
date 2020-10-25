# include <genprice.h>
# include <grs.h>
# include <math.h>
# include <cstdlib>

GenPrice::GenPrice(Problem *p)
    :Optimizer(p)
{
    addParameter("genprice_chromosome_count","200","Number of chromosomes");
    addParameter("genprice_chromosome_size","100","Size of the chromosomes");
    addParameter("genprice_max_generations","200","Maximum number of generations");
    addParameter("genprice_selection_rate","0.1","Selection rate");
    addParameter("genprice_mutation_rate","0.05","Mutation rate");

}


bool GenPrice::terminated()
{
    for(int i=0;i<M;i++)
    {
        double y=sample->getSampleY(i);
        if(y<fmin)
        {
            posmin=i;
            fmin = y;
            sample->getSampleX(i,xmin);
        }
        if(y>fmax)
        {
            posmax=i;
            fmax = y;
            sample->getSampleX(i,xmax);
        }
    }
    if(iters>=100000)
    {
        return true;
    }
    printf("iters=%d fmin=%.10lg diff=%.10lg\n",iters,fmin,fabs(fmax-fmin));
    if(fabs(fmax-fmin)<1e-6)
    {
        return true;
    }
    return false;
}

void GenPrice::step()
{
    double fk;
    Data x;
    x.resize(dimension);
    step2:
    sample2->clear();
    for(int i=0;i<dimension;i++) center[i]=0.0;
    for(int i=0;i<dimension+1;i++) index[i]=-1;
    for(int i=0;i<dimension+1;i++)
    {
        int r,flag;
        do{
            r=rand() % M;
            flag=0;
            for(int j=0;j<i;j++)
            {
                if(index[j]==r)
                {
                    flag=1;
                    break;
                }
            }
        }while(flag);
        index[i]=r;
        sample->getSampleX(r,x);
        double y=sample->getSampleY(r);
        sample2->addSample(x,y);
        if(i!=dimension)
            for(int j=0;j<dimension;j++)
                center[j]=center[j]+1.0/dimension * x[j];
    }
    sample2->getSampleX(dimension,x);
    for(int i=0;i<dimension;i++)
        xk[i]=2.0 * center[i] -x[i];
    if(!myProblem->isPointIn(xk)) goto step2;
    fk=myProblem->funmin(xk);
    Solver->Solve(xk,fk);
printf("fk is %lf \n",fk);
    if(fk<=fmax)
    {
        sample->replaceSample(posmax,xk,fk);
        iters++;
    }
}

void    GenPrice::init()
{

        dimension = myProblem->getDimension();

    M =25 * myProblem->getDimension();
    xmin.resize(myProblem->getDimension());
    xmax.resize(myProblem->getDimension());
    sample=new Collection(myProblem->getDimension());
    center.resize(myProblem->getDimension());
    fmax=-1e+100;
    fmin= 1e+100;
    iters=1;
    Data x;
    x.resize(dimension);
    double y;



    sample2=new Collection(dimension);
    index.resize(dimension + 1);
    xk.resize(dimension);
    double fk;

    Solver=new Grs(myProblem);
    int genome_count=params["genprice_chromosome_count"].toString().toInt();
    Solver->setGenomeCount(genome_count);
    int genome_length=params["genprice_chromosome_size"].toString().toInt();
    Solver->setGenomeLength(genome_length);

    Solver->setMutationRate(params["genprice_selection_rate"].toString().toDouble());
    Solver->setMutationRate(params["genprice_mutation_rate"].toString().toDouble());
    Solver->setMaxGenerations(params["genprice_max_generations"].toString().toInt());
    for(int i=0;i<M;i++)
    {
        x= myProblem->getRandomPoint();
        y=myProblem->funmin(x);
        sample->addSample(x,y);
    }

}

void GenPrice::done()
{
    fmin=localSearch(xmin);
}

void	GenPrice::getMinimum(Data &x,double &y)
{
	x = xmin;
	y = fmin;
}

int	GenPrice::getM() const
{
	return M;
}

void	GenPrice::setM(int m)
{
	M = m;
}

GenPrice::~GenPrice()
{
     delete sample2;
	delete sample;
    delete Solver;

}

extern "C" GENPRICE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new GenPrice(p);
}


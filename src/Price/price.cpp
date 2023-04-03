#include "price.h"
Price::Price(Problem *p)
    :Optimizer(p)
{
    sample = NULL;
    sample2 = NULL;
    number_of_samples=-1;
    maximum_number_of_iterations=10000;
    new_point_method="PRICE";
    stopping_rule="PRICE";
    addParameter("price_samples","-1","The initial number of samples");
    addParameter("price_iterations","10000","The maximum number of iterations");
    addParameter("price_newpoint","PRICE","Price Sampling. Available values: PRICE,MINPOINT,NEWPOINT");
    addParameter("price_stopping","PRICE","Price stopping rule. Available values: PRICE,DOUBLEBOX");
}

bool    Price::terminated()
{
    const double eps = 1e-6;
    printf("PRICE. Fmin = %10.5lf Fmax = %10.5lf Iters: %4d\n",
           fmin,fmax,iters);
    if(stopping_rule == "PRICE")
    {
        return (fmax - fmin < eps) || (iters>=maximum_number_of_iterations);
    }
    else
    {
        double v= fabs(1.0 + fmin);
        xx1+=v;
        xx2+=v * v ;
        double variance=xx2/miters - (xx1/miters) * (xx1/miters);
        if(v<oldfmin) stopat=variance/2.0;
        return (stopat<=variance/2.0 && miters>=100) || (iters>=maximum_number_of_iterations);
    }
    return false;
}

void    Price::step()
{
    double fk;
    miters++;
    makeMinMax();
    step2:
    makeSample2();
    xk=getNewPoint();
    if(!myProblem->isPointIn(xk)) goto step2;
    fk = myProblem->funmin(xk);
    if(fk<=fmax)
    {
        sample->replaceSample(posmax,xk,fk);
        iters++;
    }
}

void    Price::init()
{
    number_of_samples = params["price_samples"].toString().toInt();
    maximum_number_of_iterations = params["price_iterations"].toString().toInt();
    new_point_method = params["price_newpoint"].toString();
    stopping_rule = params["price_stopping"].toString();

    if(sample!=NULL) delete sample;
    if(sample2!=NULL) delete sample2;


    sample =new Collection(myProblem->getDimension());
    sample2 = new Collection(myProblem->getDimension());

    makeSample();
    iters = 0;
    miters = 0;
    fmin = 1e+100;
    fmax = -1e+100;
    xx1 = 0.0;
    xx2 = 0.0;
    stopat = 0.0;
    xk.resize(myProblem->getDimension());
    xmin.resize(myProblem->getDimension());
    xmax.resize(myProblem->getDimension());
}

void    Price::makeMinMax()
{
    int M = sample->getSize();
    fmax = -1e+100;
    fmin = 1e+100;
    double y ;
    posmin=0,posmax=0;
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
}

void    Price::done()
{
    double y  = localSearch(xmin);
}

void    Price::makeSample2()
{
    sample2->clear();
    int dimension = myProblem->getDimension();
    vector<int> index;
    index.resize(dimension + 1);
    Data x;
    x.resize(dimension);
    double y;
    for (int i = 0; i < dimension + 1; i++)
    {
       int r, flag;
       do
       {
         r = rand() % sample->getSize();//getTournamentElement(sample, tournament_size);
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

void    Price::makeSample()
{
    sample->clear();
     center.resize(myProblem->getDimension());
    int M = (number_of_samples==-1)?25 *myProblem->getDimension():number_of_samples;
    Data x;
    x.resize(myProblem->getDimension());
    for(int i=0;i<M;i++)
    {
        x=myProblem->getRandomPoint();
        double y = myProblem->funmin(x);
        sample->addSample(x,y);
    }

}

Data    Price::getNewPoint()
{
    bool useFirstCenter =false; //original algorithm
    bool useMinCenter = false;	 //proposed by Ali
    bool useSimpleCenter = true; //proposed by Charilogis
    if(new_point_method=="PRICE")
    {
        useFirstCenter=true;
        useMinCenter=false;
        useSimpleCenter=false;
    }
    else
    if(new_point_method=="MINPOINT")
    {
        useFirstCenter=false;
        useMinCenter=true;
        useSimpleCenter=false;
    }
    else
    {
        useFirstCenter=false;
        useMinCenter=false;
        useSimpleCenter=true;
    }
    int dimension = myProblem->getDimension();
    Data xk;
    xk.resize(dimension);
    Data x;
    x.resize(dimension);

    for (int i = 0; i < dimension; i++)
    {

        center[i] = 0.0;
    }
    //if(useFirstCenter || changeFlag)
    if (useFirstCenter)
    {
        for (int i = 0; i < dimension; i++)
        {
            sample2->getSampleX(i, x);
            for (int j = 0; j < dimension; j++)
                center[j] = center[j] + 1.0 / dimension * x[j];
        }
        sample2->getSampleX(dimension, x);
        for (int i = 0; i < dimension; i++)
            xk[i] = 2.0 * center[i] - x[i];
    }
    else if (useMinCenter)
    {
        for (int i = 1; i < dimension; i++)
        {
            sample2->getSampleX(i, x);
            for (int j = 0; j < dimension; j++)
                center[j] = center[j] + 1.0 / dimension * x[j];
            for (int j = 0; j < dimension; j++)
                center[j] += xmin[j] / dimension;
        }
        sample2->getSampleX(dimension, x);
        for (int i = 0; i < dimension; i++)
            xk[i] = 2.0 * center[i] - x[i];
    }
    else if (useSimpleCenter)
    {

        for (int i = 0; i < dimension; i++)
        {
            sample2->getSampleX(i, x);
            for (int j = 0; j < dimension; j++)
                center[j] += x[j] * (1.0 / dimension);
        }
        for (int j = 0; j < dimension; j++)
            center[j] += xmin[j] / dimension;
        sample2->getSampleX(dimension, x);
        for (int i = 0; i < dimension; i++)
            xk[i] = center[i] - (x[i] / dimension);
    }

    return xk;
}


Price::~Price()
{
    if(sample!=NULL) delete sample;
    if(sample2!=NULL) delete sample2;
}


 extern "C"  PRICE_EXPORT Optimizer *createOptimizer(Problem *p)
 {
     return new Price(p);
 }

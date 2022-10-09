#include "nncsampler.h"
# include "grspopulation.h"
NNCSampler::NNCSampler(Problem *p)
    :ProblemSampler("nncsampler",p)
{
    program=NULL;
    dataset = NULL;
}

void   NNCSampler::sampleFromProblem(int N,Matrix &xsample,Data &ysample)
{
    if(dataset!=NULL) delete dataset;
    dataset = new Dataset(myProblem,N);
    program = new NNCNeuralProgram(dataset,dataset);
   // program->setDimension(myProblem->getDimension());
    xsample.resize(N);
    ysample.resize(N);
    for(int i=0;i<N;i++)
    {
        xsample[i]=dataset->getx(i);
        ysample[i]=dataset->gety(i);
    }
}

double  NNCSampler::eval(Data &xpoint)
{
    return program->neuralparser->eval(xpoint);
}

void    NNCSampler::trainModel()
{
    GrsPopulation pop(200,200,program);
    for(int i=0;i<200;i++)
    {
        pop.nextGeneration();
     //   printf("Population fitness: %20.10lg\n",pop.getBestFitness());
    }
}

void    NNCSampler::sampleFromModel(int &N,Matrix &xsample,Data &ysample)
{
    //sample many points and take the N lowest values
    const int NMany = 10 * N;
    xsample.resize(NMany);
    ysample.resize(NMany);
    for(int i=0;i<NMany;i++)
    {
        xsample[i]=myProblem->getRandomPoint();
        ysample[i]=eval(xsample[i]);
    }
    //sort
    for(int i=0;i<NMany;i++)
    {
        for(int j=0;j<NMany-1;j++)
        {
            if(ysample[j+1]<ysample[j])
            {
                Data tx = xsample[j];
                xsample[j]=xsample[j+1];
                xsample[j+1]=tx;
                double dx = ysample[j];
                ysample[j]=ysample[j+1];
                ysample[j+1]=dx;
            }
        }
    }

    xsample.resize(N);
    ysample.resize(N);
}

NNCSampler::~NNCSampler()
{
    delete program;
    if(dataset!=NULL) delete dataset;
}

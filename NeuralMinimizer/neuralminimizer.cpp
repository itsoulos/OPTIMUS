#include "neuralminimizer.h"
NeuralMinimizer::NeuralMinimizer(Problem *p)
    :Optimizer(p)
{
    addParameter("neural_samples","20","Number of samples for Neural Minimizer");
    addParameter("neural_iterations","100","Number of maximum iterations for Neural Minimizer");
    addParameter("neural_start_samples","100","Number of start samples for Neural Minimizer");
    sampler= NULL;
}

bool NeuralMinimizer::terminated()
{

    int neural_iterations =params["neural_iterations"].toString().toInt();
    int neural_samples  = params["neural_samples"].toString().toInt();

    double fmin=fabs(1.0+fabs(besty));
       double avg_minima = 0.0;
       for(int i=0;i<minima.size();i++)
           avg_minima+=minima[i];
       avg_minima/=minima.size();
fmin=fabs(1.0+fabs(avg_minima));
    x1=x1+fmin;
    x2=x2+fmin * fmin;

    iter++;
    variance = x2/(iter+1) -(x1/(iter+1))*(x1/(iter+1));
    variance=sqrt(fabs(variance));

    if(fabs(besty-oldBesty)>1e-6)
    {
        oldBesty=besty;
            printf("New old = %20.10lg \n",oldBesty);
        stopat=variance/2.0;
    }

    printf("Iter = %d Value =%20.10lg VARIANCE=%20.10lg STOPAT=%20.10lg MINIMA=%20.10lg\n",
           iter,besty,variance,stopat,avg_minima);
    return iter>=neural_iterations*neural_samples|| (variance<=stopat && iter>=50);

}

void NeuralMinimizer::step()
{

    int neural_samples=params["neural_samples"].toString().toInt();
    sampler->trainModel();
    xsample.clear();
    ysample.clear();
    sampler->sampleFromModel(neural_samples,xsample,ysample);
    for(int i=0;i<xsample.size();i++)
    {

        double y = localSearch(xsample[i]);
        sampler->addSampleFromProblem(xsample[i],y);
        minima.push_back(y);
        if(y<besty)
        {
            besty = y;
            bestx = xsample[i];
        }
   if(terminated()) break;

    }

}

void NeuralMinimizer::init()
{
    besty = 1e+100;
    oldBesty = 1e+100;
    iter = 0;
    x1 = 0;
    x2 = 0;
    if(sampler!=NULL)
        delete sampler;
    sampler = new NNCSampler(myProblem);
    minima.clear();
    int neural_start_samples  = params["neural_start_samples"].toString().toInt();
    sampler->sampleFromProblem(neural_start_samples,xsample,ysample);

}

void NeuralMinimizer::done()
{
    besty = localSearch(bestx);
}

void NeuralMinimizer::Solve()
{
    init();
   do
    {
        step();
    }while(!terminated());
    done();
}

NeuralMinimizer::~NeuralMinimizer()
{
    delete sampler;
}

Optimizer *createOptimizer(Problem *p)
{
    return new NeuralMinimizer(p);
}

#include "neuralminimizer.h"
NeuralMinimizer::NeuralMinimizer(Problem *p)
    :Optimizer(p)
{
    addParameter("neural_model","neural","Model used. Available values: neural,rbf,nnc");
    addParameter("neural_weights","10","The weights used in the neural sampler");
    addParameter("neural_samples","100","Number of samples for Neural Minimizer");
    addParameter("neural_iterations","200","Number of maximum iterations for Neural Minimizer");
    addParameter("neural_start_samples","50","Number of start samples for Neural Minimizer");
    addParameter("neural_termination","doublebox","The used stopping rule. Available values: similarity, doublebox");
    addParameter("neural_similarityMaxValue","20","The maximum number of iterations for the similarity stopping rule");
    addParameter("neural_trainmethod","bfgs","The method used to train the neural network model. Default: bfgs");
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
   // fmin=fabs(1.0+fabs(avg_minima));
    doublebox_xx1+=fmin;
    doublebox_xx2+=fmin * fmin;

    iter++;
    doublebox_variance = doublebox_xx2/(iter+1) -(doublebox_xx1/(iter+1))*(doublebox_xx1/(iter+1));

    if(fabs(besty-doublebox_oldBesty)>1e-6)
    {
        doublebox_oldBesty=besty;
        doublebox_stopat=doublebox_variance/2.0;
    }

    QString neural_termination = params["neural_termination"].toString();
    if(neural_termination == "similarity")
    {
        if(fabs(besty-similarity_best_value)>1e-5)
        {
            similarity_best_value = besty;
            similarity_current_count = 0;
        }
        else similarity_current_count++;
        return iter>=neural_iterations || similarity_current_count>=similarity_max_count;
    }
    printf("Iter = %d Value =%20.10lg VARIANCE=%20.10lg STOPAT=%20.10lg MINIMA=%20.10lg\n",
           iter,besty,doublebox_variance,doublebox_stopat,avg_minima);
    return iter>=neural_iterations|| (doublebox_variance<=doublebox_stopat && iter>=20);

}

void NeuralMinimizer::step()
{


    int neural_samples=params["neural_samples"].toString().toInt();
    QString neural_model = params["neural_model"].toString();
    if(neural_model == "neural")
    {
    	QString method=params["neural_trainmethod"].toString();
	    ((NeuralSampler *)sampler)->trainModel(method);
    }
    else
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
    similarity_best_value = 1e+100;
    similarity_current_count = 0;
    int neural_similarityMaxValue = params["neural_similarityMaxValue"].toString().toInt();
    similarity_max_count= neural_similarityMaxValue;
    besty = 1e+100;
    doublebox_oldBesty = 1e+100;
    iter = 0;
    doublebox_xx1 = 0;
    doublebox_xx2 = 0;
    if(sampler!=NULL)
        delete sampler;
    int neural_weights  = params["neural_weights"].toString().toInt();
    QString neural_model = params["neural_model"].toString();
    if(neural_model=="rbf")
        sampler = new RbfSampler(myProblem,neural_weights);
    else
        sampler = new NeuralSampler(myProblem,neural_weights);

    minima.clear();
    int neural_start_samples  = params["neural_start_samples"].toString().toInt();
    sampler->sampleFromProblem(neural_start_samples,xsample,ysample);

}

void NeuralMinimizer::done()
{
    besty = localSearch(bestx);
    printf("Terminating with %20.10lg\n",besty);
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

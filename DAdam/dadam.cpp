#include "dadam.h"

DAdam::DAdam(Problem *p)
    :Optimizer(p)
{
}

bool    DAdam::terminated()
{
    return epoch>=max_epoch;
}

void    DAdam::step()
{
    int d = myProblem->getDimension();
    int num_of_bachces = floor(nsamples / sample_node);
    for(int j = 1 ; j<= num_of_bachces; j++)
    {

            //update step-size
            double dstep = 0.001;//options.stepsizefun(total_iter, options)*(1+min_eig_W);

            //increment total iteration numbers
            total_iter = total_iter + 1;
#pragma omp parallel for num_threads(threads)
            for(int ii=1;ii<=num_nod;ii++)
            {
                //calculate gradient
                //sam_indice_node[ii-1] = randsample(indice_node{ii},options.batch_size);
                //calculate gradient
                Data gg;
                gg.resize(d);
                myProblem->granal(w_node[ii-1],gg);

                //grad_node[ii-1]= problem.grad( w_node{ii},sam_indice_node{ii});
                //update biased first moment estimate
                for(int k=0;k<d;k++)
                {
                m_node[ii-1][k] = beta1*m_node[ii-1][k] + (1 - beta1)*gg[k];
                //update biased second raw moment estimate
                v_node[ii-1][k] = beta2*v_node[ii-1][k] + (1 - beta2)*(gg[k]*gg[k]);
                m_hat_node[ii-1][k] = m_node[ii-1][k]/(1 - pow(beta1,total_iter));
                v_hat_node[ii-1][k] = v_node[ii-1][k]/(1 - pow(beta2,total_iter));
                //compute bias-corrected fist moment estimate
                //compute bias-corrected second raw moment estimate
                //normalize second raw moment
                v_bar_node[ii-1][k] =beta3*v_bar_prev_node[ii-1][k]+(1-beta3)*max(v_bar_prev_node[ii-1][k], v_hat_node[ii-1][k]);
                v_bar_prev_node[ii-1][k] = v_bar_node[ii-1][k] ;
                d_node[ii-1][k]=m_hat_node[ii-1][k]/ (sqrt(v_hat_node[ii-1][k]) + epsilon);
                }


                //update w

                for(int k=0;k<myProblem->getDimension();k++)
                w_node[ii-1][k] = w_consen_node[ii-1][k] - dstep * d_node[ii-1][k];

                //proximal operator
                //if prox==1
                //    w_node{ii}  = problem.prox(  w_node{ii} , step);
                //end
            }
            Data net;
            net.resize(threads);
            for(int k=0;k<net.size();k++)
                net[k]=1.0/net.size();
#pragma omp parallel for num_threads(threads)
            for(int ii=1;ii<=num_nod;ii++)
            {
                // update w
                for(int k=0;k<d;k++)
                {
                w_consen_node[ii-1][k]=0.0;
                //let netsize=threads
                //w net = 1/netsize
                for(int jj=1;jj<=net.size();jj++)
                    w_consen_node[ii-1][k] = w_consen_node[ii-1][k]+ net[jj-1]*w_node[jj-1][k];
                }
            }
#pragma omp critical
            for(int a=0;a<w_node.size();a++)
            {
                w[a]=0.0;
                for(int b=0;b<d;b++)
                    w[a]=w[a]+w_node[a][b];
                w[a]/=d;
            }
            printf("FMIN = %lf \n",myProblem->funmin(w));
            //
         //   w=mean(cell2mat(w_node),2);
            //
     }
    epoch++;
}

void    DAdam::init()
{

    int d= myProblem->getDimension();

    zeros.resize(d);
    for(int j=0;j<d;j++)
        zeros[j]=0.0;

     w = myProblem->getRandomPoint();
     w_node.resize(0);
     w_consen_node.resize(0);
     d_node.resize(0);
    v_node.resize(0);
m_node.resize(0);
m_hat_node.resize(0);
v_hat_node.resize(0);
v_bar_node.resize(0);
v_bar_prev_node.resize(0);

    for( int ii=1;ii<=num_nod;ii++)
    {
        int start_index = 0;
        if(ii!=num_nod)
        {
            start_index = (ii-1) * sample_node  + 1;
            indice_node.push_back(start_index);
        }
        else
           {
            start_index = (ii-1) * sample_node  + 1;
            indice_node.push_back(start_index);
        }
        w_node.push_back(w);
        w_consen_node.push_back(w);

        d_node.push_back(zeros);

       //initialise moment estimates
        v_node.push_back(zeros);
        m_node.push_back(zeros);
        m_hat_node.push_back(zeros);
        //compute bias-corrected second raw moment estimate
        v_hat_node.push_back(zeros);
        v_bar_node.push_back(zeros);
        v_bar_prev_node.push_back(zeros);
    }
}

void    DAdam::done()
{
    //nothing for now
}

void    DAdam::Solve()
{
    Optimizer::Solve();
}

DAdam::~DAdam()
{

}
extern "C" DADAM_EXPORT Optimizer *createOptimizer(Problem *p)
{
  return new DAdam(p);
}

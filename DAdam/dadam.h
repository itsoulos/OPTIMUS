#ifndef DADAM_H
#define DADAM_H

#include "DAdam_global.h"
# include <Optimus/optimizer.h>
class PAdamInterface
{
public:
    virtual ~PAdamInterface()
    {

    }
};

class DADAM_EXPORT DAdam : public Optimizer, PAdamInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    /**
     *  Parameters**/
    double beta1=0.9;
    double beta2=0.999;
    double beta3=0.999;
    double epsilon=1e-4;
    int num_nod=8;
    int nsamples=100;
    int sample_node=10;
    int max_epoch=100;
    /** End Of parameters **/

    vector<Data> w_node,d_node,w_consen_node,m_node,v_node;
    vector<Data> m_hat_node,v_hat_node,v_bar_node,v_bar_prev_node;
    int total_iter,epoch;
    vector<int> indice_node;
    Data zeros;
    Data w;
public:
    DAdam(Problem *p);
    virtual void Solve();
    virtual ~DAdam();
};
extern "C" DADAM_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // DADAM_H

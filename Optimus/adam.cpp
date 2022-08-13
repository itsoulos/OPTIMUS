#include "adam.h"
Adam::Adam(Problem *p)
{
    myProblem=p;
    learningRate = 0.01;
    iterations = 100000;

    b1=0.9;
    b2=0.999;
}

double  Adam::Solve(Data &x)
{
	
    double f=0.0;
    Data gradient=x;

    rM = myProblem->getRightMargin();
    lM = myProblem->getLeftMargin();
    M_0.resize(myProblem->getDimension(),0.0);
    U_0.resize(myProblem->getDimension(),0.0);
    for(unsigned i = 0; i < iterations; i++){
        double a = learningRate * sqrt( 1.0 - pow(b2,i+1) ) / (1.0 - pow(b1,i+1));
        myProblem->granal(x,gradient);
        for(unsigned j = 0; j < gradient.size(); j++){
            M_0[j] = b1 * M_0[j] + (1-b1) * gradient[j];
            U_0[j] = b2 * U_0[j] + (1-b2) * gradient[j] * gradient[j];
            x[j] -= a * M_0[j] / (sqrt(U_0[j]) + 1e-1) ;
            if(x[j] > rM[j]) x[j] = rM[j];
            if(x[j] < lM[j]) x[j] = lM[j];
        }
        f=myProblem->funmin(x);
        if(i % (iterations/10) == 0)std::cout << "ADAM ITER: " << i << " ERROR: " <<f << std::endl;
    }
    return f;
    

	Data g_t = x;
	Data m_t = x;
	Data v_t = x;
	Data m_cap = x;
	Data v_cap = x;
	int t = 0;
	Data xprev = x;
	double epsilon = 1e-6;
	f = 0.0;
	for(int i=0;i<m_t.size();i++)
	{
		m_t[i]  = 0.0;
		v_t[i]  = 0.0;
	}
	while(true)
	{
		t++;
		myProblem->granal(x,g_t);
		double diff = 0.0;
		const int threads=1;
#pragma omp parallel for num_threads(threads)
		for(int i=0;i<x.size();i++)
		{
			m_t[i]=b1 * m_t[i]+(1-b1)*g_t[i];
			v_t[i]=b2 * v_t[i]+(1-b2)*g_t[i]*g_t[i];
			m_cap[i]=m_t[i]/(1-pow(b1,t));
			v_cap[i]=v_t[i]/(1-pow(b2,t));
			xprev[i] = x[i];
			x[i]=x[i]-(learningRate*m_cap[i])/(sqrt(v_cap[i])+epsilon); 

		}	
		for(int i=0;i<x.size();i++)
			diff+=pow(x[i]-xprev[i],2.0);
		diff/=x.size();
		if(diff<1e-7) break;
		f=myProblem->funmin(x);
		if(t>=iterations) break;
	}

	return f;
}

void    Adam::setB1(double b)
{
    if(b>=0 && b<=1)
        b1=b;
}

void    Adam::setB2(double b)
{
    if(b>=0 && b<=1)
        b2=b;
}

double  Adam::getB1() const
{
    return b1;
}

double  Adam::getB2() const
{
    return b2;
}

void    Adam::setLearningRate(double r)
{
    if(r>0 && r<=1)
        learningRate=r;
}

double  Adam::getLearningRate() const
{
    return learningRate;
}

void    Adam::setIterations(int it)
{
    if(it>0) iterations=it;
}

int     Adam::getIterations() const
{
    return iterations;
}

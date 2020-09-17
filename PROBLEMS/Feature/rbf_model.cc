# include <rbf_model.h>
//# define CLASSMODEL

Rbf::Rbf(Mapper *m)
	:Model(m)
{
	centers = NULL;
	variances = NULL;
	weights = NULL;
	input  = NULL;
	weight.resize(0);
}

double *Output;

double Rbf::train1()
{
	int noutput=1;
	if(weight.size() != noutput * num_weights)
	{
		weight.resize(num_weights*noutput);
		setDimension(num_weights*noutput);
		if(centers)
		{
		delete[] centers;
		delete[] variances;
		delete[] weights;
		delete[] input;
		}
		centers = new double[num_weights * pattern_dimension];
		variances = new double[num_weights * pattern_dimension];
		weights = new double[num_weights*noutput];
		input = new double[pattern_dimension*xpoint.size()];
	Output=new double[noutput * xpoint.size()];
	}
	

	Matrix xx;
	xx.resize(original_dimension);
	for(int i=0;i<xpoint.size();i++) 
	{
		int d=mapper->map(origx[i],xpoint[i]);
		if(!d) 
		{
			return 1e+8;
		}
		for(int j=0;j<pattern_dimension;j++)
		{
			
			input[i*pattern_dimension+j]=xpoint[i][j];
			if(fabs(xpoint[i][j])>=1e+10 || isnan(xpoint[i][j]) || isinf(xpoint[i][j]))
			{
				return 1e+8;
			}
		}
		Output[i]=ypoint[i];
	}

        Kmeans(rnd,input,centers,variances,
			xpoint.size(),pattern_dimension,num_weights);
	
        int icode=train_rbf(pattern_dimension,num_weights,noutput,xpoint.size(),
			centers,variances,weights,input,Output);
	double v =0.0;
	v=funmin(weight);
	if(icode==1) return 1e+8;
	return v;
}

double Rbf::train2()
{
	return train1();
}

double Rbf::output(Matrix x)
{
	int noutput=1;
#ifdef CLASS
	noutput=2;
#endif
	double v[noutput];
	double *xt=new double[x.size()];
	double penalty=0.0;
	int i;
	for( i=0;i<x.size();i++) 
	{
		xt[i]=x[i];
		penalty+=(fabs(x[i])>=10.0)*1000.0;
	}
	create_rbf(pattern_dimension,num_weights,noutput,
			centers,variances,weights,xt,v);
	delete[] xt;
#ifndef CLASS
	return v[0];
#else
	return (v[0]>v[1]?0:1);	
#endif
}

void   Rbf::getDeriv(Matrix x,Matrix &g)
{
}

Rbf::~Rbf()
{
	delete[] centers;
	delete[] variances;
	delete[] weights;
	delete[] input;

}

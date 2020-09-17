# include <model.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <math.h>

//# define SCALEFACTOR

static int isone(double x)
{
	return fabs(x-1.0)<1e-5;
}

static int iszero(double x)
{
	return fabs(x)<1e-5;
}


void	Model::setRand(GRand *r)
{
	rnd=r;
}

Model::Model(Mapper *m)
{
	num_weights = 1;
	pattern_dimension = 0;
	mapper = m;
	isvalidation=0;
}

int	Model::getOriginalDimension() const
{
	return original_dimension;
}


int	Model::getNumPatterns() const
{
	return ypoint.size();
}

void	Model::randomizeWeights()
{
	weight.resize((pattern_dimension+2)*num_weights);
	setDimension(weight.size());
	for(int i=0;i<weight.size();i++) weight[i]=rnd->d();
}

void	Model::setPatternDimension(int d)
{
	if(pattern_dimension!=d)
	{
		pattern_dimension = d;
		for(int i=0;i<xpoint.size();i++) 
			xpoint[i].resize(pattern_dimension);
	}
}

void    Model::readPatterns(vector<Matrix> &xdata,Matrix &ydata)
{
    int d=xdata[0].size();
    original_dimension =d ;
    int count=ydata.size();
    origx.resize(count);
    xpoint.resize(count);
    origy.resize(count);
    ypoint.resize(count);
    int count1=0,count2=0,two_classes_flag=1;
    for(int i=0;i<count;i++)
    {
        origx[i].resize(d);
        for(int j=0;j<d;j++)
        {
            origx[i][j]=xdata[i][j];
            #ifdef LOGAR
                origx[i][j]=log(origx[i][j]);
            #endif
        }
        ypoint[i]=ydata[i];
        if(iszero(ypoint[i])) count1++;
        else
        if(isone(ypoint[i])) count2++;
        else	two_classes_flag=0;
        origy[i]=ypoint[i];
    }
}

void 	Model::readPatterns(char *filename)
{
	FILE *fp;
	fp = fopen(filename,"r");
	if(!fp) return;
	int count,d;
	fscanf(fp,"%d",&d);
	if(d<=0) 
	{
		d=0;
		fclose(fp);
		return;
	}
	original_dimension =d ;
	fscanf(fp,"%d",&count);
	if(count<=0)
	{
		d=0;
		fclose(fp);
		return;
	}
	origx.resize(count);
	xpoint.resize(count);
	origy.resize(count);
	ypoint.resize(count);
	int count1=0,count2=0,two_classes_flag=1;
	for(int i=0;i<count;i++)
	{
		origx[i].resize(d);
		for(int j=0;j<d;j++)
		{
			fscanf(fp,"%lf",&origx[i][j]);
			#ifdef LOGAR
				origx[i][j]=log(origx[i][j]);
			#endif
		}
		fscanf(fp,"%lf",&ypoint[i]);
		if(iszero(ypoint[i])) count1++;
		else 
		if(isone(ypoint[i])) count2++;
		else	two_classes_flag=0;
		origy[i]=ypoint[i];
	}
	fclose(fp);
}

static double sig(double x) {return 1.0/(1.0+exp(-x));}
void	Model::transform(Matrix x,Matrix &x1)
{
	for(int  i=0;i<x.size();i++) 
	//x1[i]=sig(x[i]);
		//x1[i]=(x[i]-xmean[i])/(xstd[i]);
	x1[i]=x[i];
	//x1[i]=(x[i]-xmin[i])/(xmax[i]-xmin[i]);
}

void	Model::setNumOfWeights(int w)
{
	num_weights = w;
}

int	Model::getPatternDimension() const
{
	return pattern_dimension;
}

double	Model::valError()
{
	double s=0.0;
	for(int i=4*xpoint.size()/5;i<xpoint.size();i++)
	{
		double v = output(xpoint[i]);
		s+=(v-ypoint[i])*(v-ypoint[i]);
	}
	return s;
}

double	Model::funmin(Matrix x)
{
	if(weight.size()!=x.size()) weight.resize(x.size());
	for(int i=0;i<x.size();i++) weight[i] = x[i];
	//return classTrainError();


	double s=0.0;
	int end=xpoint.size();
	if(isvalidation) end=4*xpoint.size()/5;
	int correct1=0;
	int correct2=0;
	int count1=0,count2=0;
	for(int i=0;i<end;i++)
	{
		double v = output(xpoint[i]);
		double e=v-ypoint[i];
		e=e*e;
#ifdef SCALEFACTOR
		if(!isone(scale_factor))
		{
			if(scale_factor>1) 
			{
				if(isone(ypoint[i])) e=e*scale_factor;
			}
			else
			{
				if(iszero(ypoint[i])) e=e*1.0/scale_factor;
			}
		}
#endif
		if(isnan(v) || isinf(v)) return 1e+8;
		s+=e;
		if(isnan(s) || isinf(s)) return 1e+8;
	}
	return s;
}

void  Model::granal(Matrix x,Matrix &g)
{
	if(weight.size()!=x.size())
	weight.resize(x.size());
	for(int i=0;i<x.size();i++) 
	{
		weight[i] = x[i];
		g[i]=0.0;
	}
	double s=0.0;
	Matrix gtemp;
	gtemp.resize(g.size());
	int end=xpoint.size();
	if(isvalidation) end=4*xpoint.size()/5;
	for(int i=0;i<end;i++)
	{
		double	e=output(xpoint[i])-ypoint[i];
#ifdef SCALEFACTOR
		if(!isone(scale_factor))
		{
			if(scale_factor>1) 
			{
				if(isone(ypoint[i])) e=e*scale_factor;
			}
			else
			{
				if(iszero(ypoint[i])) e=e*1.0/scale_factor;
			}
		}
#endif
		getDeriv(xpoint[i],gtemp);
		for(int j=0;j<g.size();j++)
		{
			g[j]+=2.0*e*gtemp[j];
		}
	}
}

/*	Grafei to apotelemsa tou mapping 
 *	sto arxeio train. Diabazei apo to arxeio itest
 *	ta test patterns kai ta grafei sto arxeio otest.
 *	O skopos einai na xrisimopoiithoyn ta patterns 
 *	gia epexergasia apo kapoio montelo poy den 
 *	kalyptetai sto paketo.
 * */
void	Model::print(char *train,char *itest, char *otest)
{
	FILE *fp=fopen(train,"w");
	if(!fp) return;
	fprintf(fp,"%d\n%d\n",pattern_dimension,origx.size());
	for(int i=0;i<origx.size();i++)
	{
		mapper->map(origx[i],xpoint[i]);
		for(int j=0;j<pattern_dimension;j++)
			fprintf(fp,"%lf ",xpoint[i][j]);
		fprintf(fp,"%lf\n",ypoint[i]);
	}
	fclose(fp);
	FILE *fin=fopen(itest,"r");
	if(!fin) return;
	FILE *fout=fopen(otest,"w");
	int d,count;
	fscanf(fin,"%d",&d);
	fscanf(fin,"%d",&count);
	printf("read %d %d \n",d,count);
	Matrix testx;
	testx.resize(d);
	double testy;
	Matrix xx;
	xx.resize(pattern_dimension);
	fprintf(fout,"%d\n%d\n",pattern_dimension,count);
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<d;j++)
			fscanf(fin,"%lf",&testx[j]);
		fscanf(fin,"%lf",&testy);
		mapper->map(testx,xx);
		for(int j=0;j<pattern_dimension;j++)
			fprintf(fout,"%lf ",xx[j]);
		fprintf(fout,"%lf\n",testy);
	}
	fclose(fin);
	fclose(fout);
}
double maxValue(Data &x);

double	Model::testError(vector<Matrix> &xdata,Matrix &ydata)
{
    double testy;
    Matrix testx;

    int count;
    int dim;
    dim=xdata[0].size();
    count=ydata.size();
    testx.resize(pattern_dimension);
    Matrix xx;
    xx.resize(dim);
    double sum = 0.0;
    double maxy=maxValue(ydata);
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<dim;j++)
        {
            xx[j]=xdata[i][j];
#ifdef LOGAR
            xx[j]=log(xx[j]);
    #endif
        }
        mapper->map(xx,testx);
        testy=ydata[i];
        double d=output(testx);
        sum+=pow(d-testy,2.0);
    }
    return (sum);
}

double maxValue(Data &x)
{
	double max=x[0];
	for(int i=0;i<x.size();i++)
		if(x[i]>max) max=x[i];
	return max;
}

double	Model::testError(char *filename)
{
	double testy;
	Matrix testx;
	int count;
	int dim;
	FILE *fp;
	fp=fopen(filename,"r");
	if(!fp) return -1.0;
	fscanf(fp,"%d",&dim);
	if(dim<=0) 
	{
		fclose(fp);
		return -1.0;
	}	
	fscanf(fp,"%d",&count);
	if(count<=0)
	{
		fclose(fp);
		return -1.0;
	}
	testx.resize(pattern_dimension);
	Matrix xx;
	xx.resize(dim);
	double sum = 0.0;
	//double maxy=maxValue(testy);
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<dim;j++) 	
		{
			fscanf(fp,"%lf",&xx[j]);
	#ifdef LOGAR
			xx[j]=log(xx[j]);
	#endif
		}
		mapper->map(xx,testx);
		fscanf(fp,"%lf",&testy);
		double d=output(testx);
	//	if(fabs(d)>1000) d=maxy;
		sum+=pow(d-testy,2.0);
	}
	fclose(fp);
	return (sum);
}

double	Model::classTestError(vector<Matrix> &xdata,Matrix &ydata)
{
    vector<double> classes;
    Matrix testx;
    double testy;
    int count;
    int dim;
    dim=xdata[0].size();
    count=ydata.size();
    double y1;
    for(int i=0;i<count;i++)
    {
        y1=ydata[i];
        int found=-1;
        for(int j=0;j<classes.size();j++)
        {
            if(fabs(classes[j]-y1)<1e-8)
            {
                found = j;
                break;
            }
        }
        if(found==-1)
        {
            int s=classes.size();
            classes.resize(s+1);
            classes[s]=y1;
        }
    }

    testx.resize(pattern_dimension);
    Matrix xx;
    xx.resize(dim);
    double sum = 0.0;
    Matrix xx2;
    xx2.resize(dim);
    int count1=0,count2=0,est1=0,est2=0;
    originalTesty.resize(count);
    originalResult.resize(count);
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<dim;j++)
        {
            xx[j]=xdata[i][j];
            #ifdef LOGAR
            xx[j]=log(xx[j]);
            #endif
        }
        mapper->map(xx,testx);
        testy=ydata[i];
        double c=output(testx);

        int found =-1;
        double dmin=1e+10;
        for(int j=0;j<classes.size();j++)
            if(fabs(classes[j]-c)<dmin)
            {
                found=j;
                dmin=fabs(classes[j]-c);
            }

        double myclass=classes[found];
        if(fabs(testy)<1e-5) count1++; else count2++;
        originalTesty[i]=testy;
        originalResult[i]=classes[found];
        if(fabs(testy-classes[found])<1e-5)
        {
            if(fabs(classes[found])<1e-5) est1++; else est2++;
        }
        sum+=(fabs(testy-myclass)>1e-5);

    }
    return (sum*100.0)/count;

}

double	Model::classTestError(char *filename,double &precision,double &recall)
{
	vector<double> classes;
	Matrix testx;
	double testy;
	int count;
	int dim;
	FILE *Fp;
	int   tp=0,fp=0,tn=0,fn=0;
	precision = 0.0;
	recall    = 0.0;
	Fp=fopen(filename,"r");
	if(!Fp) return -1.0;
	fscanf(Fp,"%d",&dim);
	if(dim<=0) 
	{
		fclose(Fp);
		return -1.0;
	}	
	fscanf(Fp,"%d",&count);
	if(count<=0)
	{
		fclose(Fp);
		return -1.0;
	}

	double x1,y1;
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<dim;j++)
		{
			fscanf(Fp,"%lf",&x1);
		}
		fscanf(Fp,"%lf",&y1);
		int found=-1;
		for(int j=0;j<classes.size();j++)
		{
			if(fabs(classes[j]-y1)<1e-8)
			{
				found = j;
				break;
			}
		}
		if(found==-1)
		{
			int s=classes.size();
			classes.resize(s+1);
			classes[s]=y1;
		}
	}
	fclose(Fp);
	
	Fp=fopen(filename,"r");
	fscanf(Fp,"%d",&dim);
	fscanf(Fp,"%d",&count);
	testx.resize(pattern_dimension);
	Matrix xx;
	xx.resize(dim);
	double sum = 0.0;
	Matrix xx2;
	xx2.resize(dim);
	int count1=0,count2=0,est1=0,est2=0;
	originalTesty.resize(count);
	originalResult.resize(count);
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<dim;j++) 
		{
			fscanf(Fp,"%lf",&xx[j]);
			#ifdef LOGAR
			xx[j]=log(xx[j]);
			#endif
		}
		mapper->map(xx,testx);
		fscanf(Fp,"%lf",&testy);
		double c=output(testx);
		
		int found =-1;
		double dmin=1e+10;
		for(int j=0;j<classes.size();j++)
			if(fabs(classes[j]-c)<dmin)
			{
				found=j;
				dmin=fabs(classes[j]-c);
			}
		if(classes.size()==2)
		{
			if(isone(classes[found]) && isone(testy)) tp++;
			else
			if(isone(classes[found]) && iszero(testy)) fp++;
			else
			if(iszero(classes[found]) && iszero(testy)) tn++;
			else
			if(iszero(classes[found]) && isone(testy)) fn++;
			else ;
	//			printf("unspecified %lf %lf \n",classes[found],testy);
		}
		double myclass=classes[found];
		if(fabs(testy)<1e-5) count1++; else count2++;
		originalTesty[i]=testy;
		originalResult[i]=classes[found];
		if(fabs(testy-classes[found])<1e-5)
		{
			if(fabs(classes[found])<1e-5) est1++; else est2++;
		}
		sum+=(fabs(testy-myclass)>1e-5);
		
	}
	fclose(Fp);
	recall=tp*1.0/(tp*1.0+fn*1.0);
	precision=tp*1.0/(tp*1.0+fp*1.0);
	return (sum*100.0)/count;
}


double	Model::classTrainError()
{
	vector<double> classes;
	double x1,y1;
	int count=xpoint.size();
	for(int i=0;i<count;i++)
	{
		int found=-1;
		y1=ypoint[i];
		for(int j=0;j<classes.size();j++)
		{
			if(fabs(classes[j]-y1)<1e-8)
			{
				found = j;
				break;
			}
		}
		if(found==-1)
		{
			int s=classes.size();
			classes.resize(s+1);
			classes[s]=y1;
		}
	}
	
	int dim=xpoint[0].size();
	Matrix testx;
	testx.resize(pattern_dimension);
	Matrix xx;
	xx.resize(dim);
	double sum = 0.0;
	Matrix xx2;
	xx2.resize(dim);
	int count1=0,count2=0,est1=0,est2=0;
	vector<int> belong;
	vector<int> fail;
	belong.resize(classes.size());
	fail.resize(classes.size());
	for(int i=0;i<fail.size();i++)
	{
		belong[i]=0;
		fail[i]=0;
	}
	for(int i=0;i<count;i++)
	{
		xx=origx[i];
		mapper->map(xx,testx);
		double c=output(testx);
		double testy=ypoint[i];
		
		int found =-1;
		double dmin=1e+10;
		for(int j=0;j<classes.size();j++)
			if(fabs(classes[j]-c)<dmin)
			{
				found=j;
				dmin=fabs(classes[j]-c);
			}
		double myclass=classes[found];
		belong[(int)testy]++;
		if(myclass!=testy) fail[(int)testy]++;		
		sum+=(fabs(testy-myclass)>1e-5);
		
	}
	double value1=0.0;
	for(int i=0;i<fail.size();i++)
	{
		double f=fail[i]*100.0/belong[i];
		value1=value1+f*f;
		//printf("BELONG[%d]=%d FAIL[%d]=%d\n",i,belong[i],i,fail[i]);
	}
	return value1;
	return (sum)/count;
}

void	Model::getErrorMatrix(Matrix &original,Matrix &res)
{
	original.resize(originalTesty.size());
	res.resize(originalTesty.size());
	for(int i=0;i<res.size();i++)
	{
	original[i]=originalTesty[i];
	res[i]=originalResult[i];
	}
}
	

void	Model::enableValidation()
{
	isvalidation=1;
}

Model::~Model()
{
}

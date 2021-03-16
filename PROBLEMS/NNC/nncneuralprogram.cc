# include <nncneuralprogram.h>
# include <math.h>
# include <QFile>
# include <QTextStream>
# include <stdio.h>
# include <stdlib.h>

NNCNeuralProgram::NNCNeuralProgram()
    :NeuralProgram(1)
{

}

void NNCNeuralProgram::setAll(int Dimension,vector<Data> &xx,Data &yy,vector<Data> &tx,Data &ty)
{
    setDimension(Dimension);
    isvalidation=0;
    dimension=xx[0].size();
        xtemp=new double[dimension];
    int tcount=xx.size();
    train_xpoint.resize(tcount);
    train_ypoint.resize(tcount);
    xmax.resize(dimension);
    xmin.resize(dimension);
    for(int i=0;i<tcount;i++)
    {
            train_xpoint[i].resize(dimension);
            for(int j=0;j<dimension;j++)
            {
                train_xpoint[i][j]=xx[i][j];
                if(i==0 || train_xpoint[i][j]>xmax[j]) xmax[j]=train_xpoint[i][j];
                if(i==0 || train_xpoint[i][j]<xmin[j]) xmin[j]=train_xpoint[i][j];
            }
            train_ypoint[i]=yy[i];
    }

    tcount=tx.size();
    test_xpoint.resize(tcount);
    test_ypoint.resize(tcount);
    categ.resize(0);
    for(int i=0;i<tcount;i++)
    {
            test_xpoint[i].resize(dimension);
            for(int j=0;j<dimension;j++)
                test_xpoint[i][j]=tx[i][j];
            test_ypoint[i]=ty[i];
            int found=0;
            for(int j=0;j<categ.size();j++)
                if(fabs(categ[j]-test_ypoint[i])<1e-7)
                {

                    found=1;
                    break;
                }
            if(!found)
            {
                int s=categ.size();
                categ.resize(s+1);
                categ[s]=test_ypoint[i];
            }

    }
    program=new SigProgram(dimension);
    setStartSymbol(program->getStartSymbol());
    neuralparser=new NeuralParser(dimension);
}

void NNCNeuralProgram::setAll1(int Dimension, QString TrainFile, QString TestFile)
{
    setDimension(Dimension);
    isvalidation=0;
    char cfile1[1024],cfile2[1024];
    strcpy(cfile1,TrainFile.toStdString().c_str());
    strcpy(cfile2,TestFile.toStdString().c_str());
    if(TrainFile!=NULL)
    {
        FILE *fp=fopen(cfile1,"r");
        if(!fp)  exit(EXIT_FAILURE);
        int d;
        fscanf(fp,"%d",&dimension);
        xtemp=new double[dimension];
        int tcount;
        fscanf(fp,"%d",&tcount);
        if(tcount<=0) {fclose(fp); return ;}
        train_xpoint.resize(tcount);
        train_ypoint.resize(tcount);
        xmax.resize(dimension);
        xmin.resize(dimension);
        for(int i=0;i<tcount;i++)
        {
            train_xpoint[i].resize(dimension);
            for(int j=0;j<dimension;j++)
            {
                fscanf(fp,"%lf",&train_xpoint[i][j]);
                if(i==0 || train_xpoint[i][j]>xmax[j]) xmax[j]=train_xpoint[i][j];
                if(i==0 || train_xpoint[i][j]<xmin[j]) xmin[j]=train_xpoint[i][j];
            }
            fscanf(fp,"%lf",&train_ypoint[i]);
        }

        fclose(fp);
    }
    if(TestFile!="")
    {
        FILE *fp=fopen(cfile2,"r");
        if(!fp) return;
        int d;
        fscanf(fp,"%d",&d);
        if(d!=dimension) {fclose(fp); return ;}
        int tcount;
        fscanf(fp,"%d",&tcount);
        if(tcount<=0) {fclose(fp); return ;}
        test_xpoint.resize(tcount);
        test_ypoint.resize(tcount);
        categ.resize(0);
        for(int i=0;i<tcount;i++)
        {
            test_xpoint[i].resize(dimension);
            for(int j=0;j<dimension;j++)
                fscanf(fp,"%lf",&test_xpoint[i][j]);
            fscanf(fp,"%lf",&test_ypoint[i]);
            int found=0;
            for(int j=0;j<categ.size();j++)
                if(fabs(categ[j]-test_ypoint[i])<1e-7)
                {

                    found=1;
                    break;
                }
            if(!found)
            {
                int s=categ.size();
                categ.resize(s+1);
                categ[s]=test_ypoint[i];
            }

        }
        fclose(fp);
    }
    program=new SigProgram(dimension);
    setStartSymbol(program->getStartSymbol());
    neuralparser=new NeuralParser(dimension);

}

NNCNeuralProgram::NNCNeuralProgram(int Dimension,QString TrainFile,QString TestFile):
	NeuralProgram(Dimension)
{
    setAll1(Dimension,TrainFile,TestFile);
}

static void mymap(Data x,Data &x1)
{
	for(int i=0;i<x.size();i++)
		x1[i]=x[i];
}


void	NNCNeuralProgram::getDeriv(Data &g)
{
	for(int i=0;i<g.size();i++) g[i]=0.0;
	Data tempg;
	tempg.resize(g.size());
	int start=0;
	int end=train_ypoint.size();
	if(isvalidation) end=4*train_ypoint.size()/5;
	for(int i=start;i<end;i++)
	{
		double v=neuralparser->eval(train_xpoint[i])-train_ypoint[i];
		neuralparser->getDeriv(train_xpoint[i],tempg);
		for(int j=0;j<g.size();j++) g[j]+=2.0*v*tempg[j];
	}
}

double	NNCNeuralProgram::getTestError()
{
	double value=0.0;

	for(int i=0;i<test_ypoint.size();i++)
	{
		double v=neuralparser->eval(test_xpoint[i])-test_ypoint[i];
		value=value+v*v;
	}
	return value;
}

int	NNCNeuralProgram::getTrainSize() const
{
	return train_ypoint.size();
}

int	NNCNeuralProgram::getTestSize() const
{
	return test_ypoint.size();
}


double	NNCNeuralProgram::getTrainError()
{
	double value=0.0;
	Data xx;
	xx.resize(dimension);
        double *xtemp=new double[dimension];
#ifndef IMBALANCED
	int start=0;
	int end=train_ypoint.size();
	if(isvalidation)
	{		
		start=0;
		end=4*train_ypoint.size()/5;
	}
	for(int i=start;i<end;i++)
	{
		for(int j=0;j<dimension;j++) xtemp[j]=train_xpoint[i][j];
		double v=neuralparser->eval(xtemp);
        if(program->EvalError() || std::isnan(v) || std::isinf(v))
		{
			delete[] xtemp;
			return 1e+8;
		}
		v=(v-train_ypoint[i]);
		value=value+v*v;
	}
#else
	vector<int> fail;
	vector<int> belong;
	int nclass=categ.size();
	fail.resize(nclass);
	belong.resize(nclass);
	for(int i=0;i<nclass;i++)
		fail[i]=belong[i]=0;
	for(int i=0;i<train_ypoint.size();i++)
	{
		for(int j=0;j<dimension;j++) xtemp[j]=train_xpoint[i][j];
		double v=neuralparser->eval(xtemp);
        if(program->EvalError() || std::isnan(v) || std::isinf(v))
		{
			return 1e+8;
		}
		double minValue=1e+10;
		int index=-1;
		int actual_class_index=0;
		double actual_min_value=1e+10;
		for(int j=0;j<categ.size();j++)
		{
			if(j==0 || fabs(categ[j]-train_ypoint[i])<actual_min_value)
			{
				actual_min_value=fabs(categ[j]-train_ypoint[i]);
				actual_class_index=j;
			}	
			if(j==0 || fabs(categ[j]-v)<minValue)
			{
				minValue=fabs(categ[j]-v);
				index = j;
			}
		}
		if(index<0 || index>=categ.size()) printf("Category: Index %d of %d out of range \n",index,categ.size());
		double myclass=categ[index];
		
		if(actual_class_index<0 || actual_class_index>=categ.size()) printf("Index %d of %d out of range \n",actual_class_index,categ.size());
		belong[actual_class_index]++;
		if(fabs(myclass-categ[actual_class_index])>1e-5)
		{
			fail[index]++;
		}
		
		//value=value+ pow(trainy[i]-outy[i],2.0);
	}
	double value1=0.0;
	for(int i=0;i<nclass;i++)
	{
		double f=fail[i]*100.0/belong[i];
		value1=value1+f*f;
	}
	return value1;
#endif
	delete[] xtemp;
    return value;
}

double	NNCNeuralProgram::getClassTestError(vector<int> &genome)
{
	double value=0.0;
	string str;
	if(!getElements(genome,str)) return -1e+8;
	program->Parse(str);
	double *xx=new double[train_xpoint[0].size()];

	for(int i=0;i<test_ypoint.size();i++)
	{
		for(int j=0;j<test_xpoint[i].size();j++) xx[j]=test_xpoint[i][j];
		double v=program->Eval(xx);
        if(program->EvalError() || std::isnan(v) || std::isinf(v))
		{
			value+=1.0;
			continue;
		}
		
		double minValue=1e+10;
		int index=-1;
		for(int j=0;j<categ.size();j++)
		{
			if(fabs(categ[j]-v)<minValue)
			{
				minValue=fabs(categ[j]-v);
				index = j;
			}
		}
		double myclass=categ[index];
		value+=fabs(test_ypoint[i]-myclass)>1e-5;
	}

	delete[] xx;
	return value*100.0/test_ypoint.size();
}

void     NNCNeuralProgram::printOutput(QString filename)
{
    QFile fp(filename);
    if(!fp.open(QIODevice::WriteOnly |QIODevice::Text)) return;
    QTextStream st(&fp);
    double *xx=new double[train_xpoint[0].size()];
    for(int i=0;i<test_ypoint.size();i++)
    {
        for(int j=0;j<dimension;j++) st<<train_xpoint[i][j]<<" ";
        for(int j=0;j<test_xpoint[i].size();j++) xx[j]=test_xpoint[i][j];
        double v=program->Eval(xx);
        st<<test_ypoint[i]<<" "<<v<<endl;
    }
    delete[] xx;
    fp.close();
}

NNCNeuralProgram::~NNCNeuralProgram()
{
	delete[] xtemp;
}

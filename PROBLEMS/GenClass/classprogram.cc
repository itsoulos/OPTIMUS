# include <classprogram.h>
#include <QDebug>
# define NAN_CLASS	1e+10


int	problem_dimension;

ClassProgram::ClassProgram()
{

}


void    ClassProgram::setData(QVector<Data> &x,Data &y)
{

    dimension = x[0].size();
    int c=x.size();

    trainy.resize(c);
    trainx.resize(c);
    vclass.resize(0);

    problem_dimension = dimension;
    for(int i=0;i<c;i++) trainx[i].resize(dimension);
    for(int i=0;i<c;i++)
    {
        for(int j=0;j<dimension;j++)
            trainx[i][j]=x[i][j];
        trainy[i]=y[i];
        int flag=0;
        for(int j=0;j<vclass.size();j++)
        {
            if(fabs(vclass[j]-trainy[i])<1e-5)
            {
                flag=1;
                break;
            }
        }
        if(!flag)
        {
            int s=vclass.size();
            vclass.resize(s+1);
            vclass[s]=trainy[i];
        }
    }
    //sort vclass
    for(int i=0;i<vclass.size();i++)
    {
        for(int j=0;j<vclass.size()-1;j++)
        {
            if(vclass[j+1]<vclass[j])
            //if(vclass[j+1]>vclass[j])
            {
                double d=vclass[j];
                vclass[j]=vclass[j+1];
                vclass[j+1]=d;
            }
        }
    }

    mapper.resize(vclass.size());
    for(int i=0;i<vclass.size();i++)
    {
        mapper[i]=vclass[i];
        vclass[i]=i;
    }


    program = new Cprogram(dimension,vclass.size()-1);
    setStartSymbol(program->getStartSymbol());
    pstring.resize(vclass.size());
    for(int i=0;i<pstring.size();i++) pstring[i]=" ";
    pgenome.resize(0);
    nclass = vclass.size();
    outy.resize(trainy.size());
}

ClassProgram::ClassProgram(QVector<Data> &x, Data &y)
{
    setData(x,y);

}

string	ClassProgram::printF(vector<int> &genome)
{
	string ret="";
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	char str[100];
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<genome.size()/(nclass-1);j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		pstring[i]=printRandomProgram(pgenome,redo);
		if(redo>=REDO_MAX) return "";
		ret+="if(";
		ret+=pstring[i];
		ret+=") CLASS=";
		sprintf(str,"%.2lf",vclass[i]);
		ret+=str;
		ret+="\nelse \n";
	}
	sprintf(str,"%.2lf",vclass[nclass-1]);
	ret+="CLASS=";
	ret+=str;
	ret+="\n";
	return ret;
}

int	ClassProgram::findMapper(double y)
{
	for(int i=0;i<vclass.size();i++)
		if(fabs(mapper[i]-y)<1e-7) return i;
	return 0;
}

double	ClassProgram::getClassError(vector<int> &genome,QVector<Data> &x,Data &y)
{
    double X[problem_dimension];
	double value=0.0;
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	if(outy.size()!=y.size()) outy.resize(y.size());
	for(int i=0;i<outy.size();i++) outy[i]=NAN_CLASS;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		string s = printRandomProgram(pgenome,redo);
		if(redo>=REDO_MAX) return -1e+8;
		pstring[i]=s;
	}
	for(int j=0;j<nclass-1;j++)
	{
		program->Parse(pstring[j]);
		for(int i=0;i<y.size();i++)
		{
			if(fabs(outy[i]-NAN_CLASS)>1e-5) continue;
			for(int k=0;k<problem_dimension;k++) X[k]=x[i][k];
			double v=program->Eval(X);
//			if(isnan(v) || isinf(v)) return -1e+8;
			if(fabs(v-1.0)<1e-5) outy[i]=vclass[j];
		}
	}
	
	for(int i=0;i<y.size();i++)
	{
		if(fabs(outy[i]-NAN_CLASS)<1e-5) outy[i]=vclass[nclass-1];
		value=value+(fabs(findMapper(y[i])-outy[i])>1e-5);
	}
	
//	if(isnan(value) || isinf(value)) return -1e+8;
    return value*100.0/y.size();
}

int	ClassProgram::getClass() const
{
	return nclass;
}

double 	ClassProgram::fitness(vector<int> &genome)
{
    	double X[problem_dimension];
	double value=0.0;
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	if(outy.size()!=trainy.size()) outy.resize(trainy.size());
	for(int i=0;i<outy.size();i++) outy[i]=NAN_CLASS;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		string s = printRandomProgram(pgenome,redo);
		if(redo>=REDO_MAX) return -1e+8;
		pstring[i]=s;
	}
	
	for(int j=0;j<nclass-1;j++)
	{
        int retvalue=program->Parse(pstring[j]);
        if(!retvalue)
        {
            qDebug()<<"Failed string is "<<QString(pstring[j].c_str())<<endl;
        }
        if(!retvalue) return -1e+8;
		for(int i=0;i<trainy.size();i++)
		{
			if(fabs(outy[i]-NAN_CLASS)>1e-5) continue;
            for(int k=0;k<problem_dimension;k++)X[k]=trainx[i][k];
            double v=program->Eval(X);
            if(std::isnan(v) || std::isinf(v) ) return -1e+8;
			if(fabs(v-1.0)<1e-5) outy[i]=vclass[j];
		}
	}
	

	vector<int> fail;
	vector<int> belong;
	fail.resize(nclass);
	belong.resize(nclass);
	for(int i=0;i<nclass;i++)
		fail[i]=belong[i]=0;
	for(int i=0;i<trainy.size();i++)
	{
		if(fabs(outy[i]-NAN_CLASS)<1e-5) 	outy[i]=vclass[nclass-1];
		int pos=findMapper(trainy[i]);
		value=value+((fabs(findMapper(trainy[i])-outy[i]))>1e-5);	
		belong[pos]++;
		if(fabs(findMapper(trainy[i])-outy[i])>1e-5)
		{
			fail[pos]++;
		}
		//value=value+ pow(trainy[i]-outy[i],2.0);
	}
    	if(std::isnan(value) || std::isinf(value)) return -1e+8;
    return -value*100.0/trainy.size();

    /*
    for(int i=0;i<nclass;i++)
	{
		printf("CLASS[%3d (%3d)] FAIL=%5.2lf%% \n",i,belong[i],fail[i]*100.0/belong[i]);
	}
    */
	double value1=0.0;
	double value1_max=value1;
	for(int i=0;i<nclass;i++)
	{
		double f=fail[i]*100.0/belong[i];
		value1=value1+f*f;
		if(f>value1_max)
			value1_max=f;
		
	}

	return -value1;
}

ClassProgram::~ClassProgram()
{
	delete program;
}


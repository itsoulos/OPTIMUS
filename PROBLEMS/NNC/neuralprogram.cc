# include <neuralprogram.h>
# include <math.h>
static double dmax(double a,double b)
{
	return a>b?a:b;
}

NeuralProgram::NeuralProgram()
{

}

void    NeuralProgram::setDimension(int Dimension)
{
    dimension=Dimension;
    program=new SigProgram(dimension);
    setStartSymbol(program->getStartSymbol());
    neuralparser=new NeuralParser(dimension);
    multiple=0;
}

NeuralProgram::NeuralProgram(int Dimension)
{
    setDimension(Dimension);
}

int	NeuralProgram::ismultiple()
{
	return multiple!=0;
}


double	NeuralProgram::getTrainError(Data x)
{
	neuralparser->setWeights(x);
	return getTrainError();
}

double	NeuralProgram::getPartError()
{
	return 0.0;
}

double	NeuralProgram::getPartError(Data &value)
{
	return 0.0;
}
void	NeuralProgram::enableMultiple(int K)
{
	multiple=K;
	nparser.resize(multiple);
	delete neuralparser;
	neuralparser=NULL;
	for(int i=0;i<nparser.size();i++) nparser[i]=new NeuralParser(dimension);
}

double	NeuralProgram::penalty1()
{
	return 0.0;
}

double	NeuralProgram::penalty2()
{
	return 0.0;
}

double	NeuralProgram::penalty3()
{
	return 0.0;
}

double	NeuralProgram::penalty4()
{
	return 0.0;
}

double	NeuralProgram::TestError(vector<int> &genome) 
{
	string str;
	double value=0.0;
	if(!multiple)
	{
		if(!getElements(genome,str)) return -1e+8;
		neuralparser->makeVector(str);
	}
	else
	{
		vector<int> subgenome;
		subgenome.resize(genome.size()/multiple);
		for(int i=0;i<multiple;i++)
		{
			for(int j=0;j<subgenome.size();j++) subgenome[j]=genome[i*subgenome.size()+j];	
			if(!getElements(subgenome,str)) return -1e+8;
			nparser[i]->makeVector(str);
		}
	}
	value=getTestError();
	return value;
}

string	NeuralProgram::printProgram(vector<int> &genome)
{
	string ret="";
	string str;
	if(!multiple)
	{
		if(!getElements(genome,str)) return "";
		ret=str;
	}
	else
	{
		vector<int> subgenome;
		subgenome.resize(genome.size()/multiple);
		for(int i=0;i<multiple;i++)
		{
			for(int j=0;j<subgenome.size();j++) subgenome[j]=genome[i*subgenome.size()+j];	
			if(!getElements(subgenome,str)) return "";
			ret=ret+"f(x)="+str+"\n";
		}
	}
	return ret;
}

double	NeuralProgram::fitness(vector<int> &genome)
{
	string str;
	if(!multiple)
	{
		if(!getElements(genome,str)) return -1e+8;
		neuralparser->makeVector(str);
		Data x;
		neuralparser->getWeights(x);
	}
	else
	{
		vector<int> subgenome;
		subgenome.resize(genome.size()/multiple);
		for(int i=0;i<multiple;i++)
		{
			for(int j=0;j<subgenome.size();j++) subgenome[j]=genome[i*subgenome.size()+j];	
			if(!getElements(subgenome,str)) return -1e+8;
			nparser[i]->makeVector(str);
		}
	}
	double f=getTrainError();
	return -f;
}

int	NeuralProgram::getElements(vector<int> &genome,string &str1)
{
	int redo=0;
	str1=printRandomProgram(genome,redo);
	if(redo>=REDO_MAX) return 0;
	return 1;
}

double	NeuralProgram::getTrainError()
{
	return 0.0;
}

double	NeuralProgram::getTestError()
{
	return 0.0;
}

int	NeuralProgram::getWeightSize()
{
	return neuralparser->getWeightSize();
}

void	NeuralProgram::getDeriv(Data &g)
{
	Data x;
	neuralparser->getWeights(x);
	for(int i=0;i<g.size();i++)
	{
		double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
		x[i]+=eps;
		neuralparser->setWeights(x);
		double v1=getTrainError();
		x[i]-=2.0 *eps;
		neuralparser->setWeights(x);
		double v2=getTrainError();
		g[i]=(v1-v2)/(2.0 * eps);
		x[i]+=eps;
		neuralparser->setWeights(x);
	}
		
}

NeuralProgram::~NeuralProgram()
{
	delete program;
	if(neuralparser!=NULL)		delete neuralparser;
	for(int i=0;i<multiple;i++) 	delete nparser[i];
}

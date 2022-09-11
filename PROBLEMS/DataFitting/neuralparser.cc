# include <neuralparser.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>


static double dmax(double a,double b)
{
	return a>b?a:b;
}

static double sig(double x)
{
	return 1.0/(1.0+exp(-x));
}

static double sigder(double x)
{
	double s=sig(x);
	return s*(1-s);
}


NeuralParser::NeuralParser(int Dimension)
{
	dimension = Dimension;
}

double	NeuralParser::valError()
{
	return 0.0;
}

void NeuralParser::setleftmargin(Data &x)
{
	left.resize(x.size());
	left=x;
}

void NeuralParser::setrightmargin(Data &x)
{
	right.resize(x.size());
	right=x;
}

Data 	NeuralParser::getleftmargin()
{
	return left;
}


Data NeuralParser::getrightmargin()
{
	return right;
}

int	NeuralParser::getWeightSize()
{
return weight.size();
}
int	NeuralParser::getDimension() const
{
	return dimension;
}

void	NeuralParser::makeVector(string str)
{
	node.resize(0);
	int index=0;
	while(index<str.size())
	{
		//format of node
		//(out)*sig(multiplier-list+(bias))
		string s="",s1="";	
		if(index>=str.size()) break;
		while(str[index]=='(' || str[index]=='+' || str[index]==')') index++;
		if(index>=str.size()) break;
		while(str[index]!=')')
		{
			s=s+str[index];
			index++;
		}
		int nodesize=node.size();
		node.resize(nodesize+1);
		node[nodesize].out=atof(s.c_str());	
		node[nodesize].mult.resize(0);
		//take the *sig(
		index++;index++;index++;index++;index++;
		int nodeTermFlag=0;
		do
		{
			//take the ( 
			while(str[index]=='(') index++;	
			s="";
			while(str[index]!=')')
			{
				s=s+str[index];
				index++;
			}
			//if the next item is ), the we set nodeTermFlag to 1 (bias)
			//otherwise we find the x_i element
			index++;
			if(index>=str.size() || str[index]==')')
			{
				nodeTermFlag=1;
				node[nodesize].bias=atof(s.c_str());
			}
			else //it is *
			{
				//take the x
				index++;index++;
				s1="";
				while(isdigit(str[index]))
				{
					s1=s1+str[index];
					index++;
				}
				if(str[index]=='+') index++;
				int k=node[nodesize].mult.size();
				node[nodesize].mult.resize(k+1);
				node[nodesize].mult[k].m=atof(s.c_str());
				node[nodesize].mult[k].index=atoi(s1.c_str());
			}
		}while(!nodeTermFlag);
		if(index>=str.size()) break;
	}
	weight.resize(node.size()*(dimension+2));
	fixstatus.resize(weight.size());
	for(int i=0;i<weight.size();i++) fixstatus[i]=0;
	for(int i=1;i<=node.size();i++)
	{
		weight[(dimension+2)*i-(dimension+1)-1]=node[i-1].out;
		weight[(dimension+2)*i-1]=node[i-1].bias;
		for(int j=1;j<=dimension;j++)
		{
			double sum=0.0;
			for(int k=0;k<node[i-1].mult.size();k++)
			{
				if(node[i-1].mult[k].index==j) 
					sum+=node[i-1].mult[k].m;
			}
			weight[(dimension+2)*i-(dimension+1)+j-1]=sum;
			if(sum==0.0) 
			  fixstatus[(dimension+2)*i-(dimension+1)+j-1]=1;
		}
	}
}

string	NeuralParser::print()
{
	int nodes=weight.size()/(dimension+2);
	string s="";
	for(int i=1;i<=nodes;i++)
	{
		string k="";
		for(int j=1;j<=dimension;j++)
		{
			char ss[100];
			sprintf(ss,"%.5lf",weight[(dimension+2)*i-(dimension+1)+j-1]);
			char s1[100];
			sprintf(s1,"%d",j);
			k=k+"("+ss+")"+"*x["+s1+"]";
		}
		char ss[100];
		sprintf(ss,"%.5lf",weight[(dimension+2)*i-1]);
		k=k+"+("+ss+")";
		sprintf(ss,"%.5lf",weight[(dimension+2)*i-(dimension+1)-1]);
		s=s+"("+ss+")*sig("+k+")";
		if(i!=nodes) s=s+"+";
	}
	return s;
}

void	NeuralParser::getWeights(vector<double> &w)
{
	w.resize(weight.size());
	w = weight;
}

void	NeuralParser::setWeights(vector<double> w)
{
	weight = w;
}

void	NeuralParser::getFixStatus(vector<int> &status)
{
	status.resize(fixstatus.size());
	status = fixstatus;
}

double	NeuralParser::eval(vector<double> xpoint)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);
	}
	return per;
}

double	NeuralParser::eval(double  *xpoint)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);
		if(fabs(weight[(dimension+2)*i-(dimension+1)-1])>1e-5)
		{
			sigcount++;
			if(fabs(arg)>=10.0) violcount++;
		}
	}
	return per;
}

void	NeuralParser::getDeriv(vector<double> xpoint,vector<double> &g)
{
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[pos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double s2=sigder(arg);
		g[(dimension+2)*i-(dimension+1)-1]=s;
		g[(dimension+2)*i-1]=weight[(dimension+2)*i-(dimension+1)-1]*s2;
		for(int j=1;j<=dimension;j++)
		{
			int pos=(dimension+2)*i-(dimension+1)+j;
			g[pos-1]=weight[(dimension+2)*i-(dimension+1)-1]*xpoint[j-1]*s2;
		}
	}
}



double  NeuralParser::evalDeriv(vector<double> xpoint,int pos)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		per+=weight[(dimension+2)*i-(dimension+1)-1]*s*(1.0-s)*weight[(dimension+2)*i-(dimension+1)+pos-1];
	}
	return per;
}

void	NeuralParser::getXDeriv(vector<double> xpoint,int pos,vector<double> &g)
{
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		double sigder=s*(1-s);
		double sigder2=s*(1-s)*(1-2.0*s);
		double sigder3=sigder*(6*s*s-6*s+1);
		g[(dimension+2)*i-1]=w1*w2*sigder2;
		g[(dimension+2)*i-(dimension+1)-1]=w2*sigder;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			if(j!=pos)
			g[mypos-1]=w1*w2*xpoint[j-1]*sigder2;
			else
			g[mypos-1]=w1*sigder+w1*w2*xpoint[j-1]*sigder2;
		}
	}
}

double	NeuralParser::evalDeriv2(vector<double> xpoint,int pos)
{
	int nodes=weight.size()/ (dimension + 2);
	double per=0.0;
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		per+=w1*w2*w2*s*(1.0-s)*(1.0-2*s);
	}
	return per;
}

void	NeuralParser::getX2Deriv(vector<double> xpoint,int pos,vector<double> &g)
{
	double v=0.0;
	double maxv=-1e+100;
	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		double arg=0.0;
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			arg+=xpoint[j-1]*weight[mypos-1];
		}
		arg+=weight[(dimension+2)*i-1];
		double s=sig(arg);
		double sigder=s*(1-s);
		double sigder2=s*(1-s)*(1-2.0*s);
		double sigder3=sigder*((1-2*s)*(1-2*s)-2*sigder);
		double w1=weight[(dimension+2)*i-(dimension+1)-1];
		double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
		g[(dimension+2)*i-1]=w1 * w2 * w2 * sigder3;
		g[(dimension+2)*i-(dimension+1)-1]=w2*w2*s*(1-s)*(1-2*s);
		for(int j=1;j<=dimension;j++)
		{
			int mypos=(dimension+2)*i-(dimension+1)+j;
			if(j!=pos)
			g[mypos-1]=w1*w2*w2*xpoint[j-1]*sigder3;
			else
			g[mypos-1]=2*w1*w2*sigder2+w1*w2*w2*xpoint[j-1]*sigder3;
		}
	}
}

void	NeuralParser::getMargins(Data &l,Data &r)
{
	l.resize(weight.size());
	r.resize(weight.size());

	int nodes=weight.size()/ (dimension + 2);
	for(int i=1;i<=nodes;i++)
	{
		int mypos;
		for(int j=1;j<=dimension;j++)
		{
			mypos=(dimension+2)*i-(dimension+1)+j;
			l[mypos-1]=-10;
			r[mypos-1]= 10;
		}
		l[(dimension+2)*i-1]=-10;
		r[(dimension+2)*i-1]= 10;

		l[(dimension+2)*i-(dimension+1)-1]=-100;
		r[(dimension+2)*i-(dimension+1)-1]= 100;
	}
}

NeuralParser::~NeuralParser()
{
}


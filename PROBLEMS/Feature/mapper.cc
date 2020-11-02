# include <stdio.h>
# include <stdlib.h>
# include <mapper.h>
# include <math.h>
# include <iostream>
using namespace std;
extern double sig(const double *x);
extern double MMin(const double *x);
extern double MMax(const double *x);
extern double MAvg(const double *x);
extern double MStd(const double *x);
Mapper::Mapper(int d,double scale)
{
    scale_factor=scale;
	dimension = d;
	parser.resize(0);
	vars="x1";
	for(int i=1;i<dimension;i++)
	{
		vars=vars+",";
		char str[10];
		sprintf(str,"x%d",i+1);
		vars=vars+str;
	}	
}

int havex(string s)
{
	for(int i=0;i<s.length()-1;i++)
	{
		if(s[i]=='x' && isdigit(s[i+1])) return 1;
	}
	return 0;
}

void	Mapper::setExpr(vector<string> s)
{
	if(parser.size()!=s.size()) 
	{
	        parser.resize(s.size());
		for(int i=0;i<parser.size();i++) 
		{
			delete parser[i];
			parser[i] = new FunctionParser();
			parser[i]->AddFunction("sig",sig,1);
			/*
			parser[i]->AddFunction("xmin",MMin,1);
			parser[i]->AddFunction("xmax",MMax,1);
			parser[i]->AddFunction("xmean",MAvg,1);
			parser[i]->AddFunction("xstd",MStd,1);
			*
			*/
		}
	}
	countx=0;
	for(int i=0;i<s.size();i++) 
	{
		parser[i]->Parse(s[i],vars);
		countx+=havex(s[i]);
	}
}

int	Mapper::map(Matrix x,Matrix &x1)
{
	if(countx!=parser.size()) return 0;
	double *xx=new double[x.size()];
	for(int i=0;i<x.size();i++) xx[i]=x[i];
	for(int i=0;i<parser.size();i++) 
	{
		x1[i]=parser[i]->Eval(xx);
		if(isnan(x1[i]) || isinf(x1[i])) {
					delete[] xx;return 0;}
		if(parser[i]->EvalError()) {
		delete[] xx;return 0;}
        //x1[i]=fmod(x1[i],scale_factor);
		if(fabs(x1[i])>1e+2) {delete[] xx;return 0;}
	}
	delete[] xx;
	return 1;
}

Mapper::~Mapper()
{
	for(int i=0;i<parser.size();i++) delete parser[i];
}

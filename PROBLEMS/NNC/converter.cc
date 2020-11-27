# include <converter.h>
# include <math.h>
Converter::Converter(Data x,int s,int d)
{
	weight.resize(x.size());
	weight=x;
	sigcount = s;
	dimension = d;
	nodes = x.size()/(d+2);
}

void	Converter::add(int x,vector<int> &g,int &pos)
{
	if(pos>=g.size()) 
	{
		g.resize(pos+10);
	}
	g[pos++]=x;
}

void	Converter::makeNumber(double out,vector<int> &g,int &pos)
{
	if(out>=0) add(0,g,pos); else add(1,g,pos);
	char ss[100];
	sprintf(ss,"%.5lf",fabs(out));
	string leftpart="",rightpart="";
	int i;
	for(i=0;ss[i]!='.';i++) leftpart=leftpart+ss[i];
	for(i++;i<strlen(ss);i++)  rightpart=rightpart+ss[i];
	for(int i=0;i<leftpart.size();i++)
	{
		if(i!=leftpart.size()-1) add(1,g,pos); else add(0,g,pos);
		int digit=leftpart[i]-'0';
		add(digit,g,pos);
	}
	for(int i=0;i<rightpart.size();i++)
	{
		if(i!=rightpart.size()-1) add(1,g,pos); else add(0,g,pos);
		int digit=rightpart[i]-'0';
		add(digit,g,pos);
	}
}

void	Converter::makeOut(vector<int> &g,int &pos)
{
	double out=weight[(dimension+2)*nodecounter-(dimension+1)-1];
	makeNumber(out,g,pos);
}

void	Converter::makeBias(vector<int> &g,int &pos)
{
	double out=weight[(dimension+2)*nodecounter-1];
	makeNumber(out,g,pos);
}

void	Converter::makeWeight(vector<int> &g,int &pos)
{
	double out=weight[(dimension+2)*nodecounter-(dimension+1)+xcounter-1];
	makeNumber(out,g,pos);
}

void	Converter::makeX(vector<int> &g,int &pos)
{
	add(xcounter-1,g,pos);
}

void	Converter::makeNode(vector<int> &g,int &pos)
{
	xcounter=0;
	list.resize(0);
	for(int i=1;i<=dimension;i++)
	{
		double v=weight[(dimension+2)*nodecounter-(dimension+1)+i-1];
		if(fabs(v)>1e-7) list.push_back(i);
	}
	//
	for(int i=0;i<list.size();i++)
	{
		xcounter=list[i];
		if(i==list.size()-1) add(0,g,pos); else add(1,g,pos);
		makeWeight(g,pos);	
		makeX(g,pos);
	}
	makeBias(g,pos);
	nodecounter++;
}

void	Converter::convert(vector<int> &g)
{
	for(int i=0;i<g.size();i++) g[i]=0;
	int pos=0;
	nodecounter=1;
	if(sigcount==1)
	{
		add(0,g,pos);
		makeOut(g,pos);
		makeNode(g,pos);
	}
	else
	{
		for(int i=1;i<sigcount;i++)
		{
			add(1,g,pos);
			makeOut(g,pos);
			makeNode(g,pos);
		}
		add(0,g,pos);
		makeOut(g,pos);
		makeNode(g,pos);
	}
}

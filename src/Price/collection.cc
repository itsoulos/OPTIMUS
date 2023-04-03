# include <collection.h>
# include <math.h>

Collection::Collection(int d)
{
	dimension = d;
	xpoint.resize(0);
	ypoint.resize(0);
}

void	Collection::addSample(Data x,double y)
{
	int s=xpoint.size();
	xpoint.resize(s+1);
	ypoint.resize(s+1);
	xpoint[s].resize(dimension);
	for(int i=0;i<dimension;i++) xpoint[s][i]=x[i];
	ypoint[s]=y;
}

void	Collection::replaceSample(int pos,Data x,double y)
{
	if(pos>=xpoint.size() || pos<0) return;
	for(int i=0;i<dimension;i++) xpoint[pos][i]=x[i];
	ypoint[pos]=y;
}

void	Collection::replaceSample(int pos,Data x)
{
	if(pos>=xpoint.size() || pos<0) return;
	xpoint[pos]=x;
}

void	Collection::replaceSample(int pos,double y)
{
	if(pos>=xpoint.size() || pos<0) return;
	ypoint[pos]=y;
}

void	Collection::getSampleX(int pos,Data &x)
{
	if(pos>=xpoint.size() || pos<0) return;
	for(int i=0;i<dimension;i++) x[i]=xpoint[pos][i];
}

void	Collection::getSampleY(int pos,double &y)
{
	if(pos>=xpoint.size() || pos<0) return;
	y=ypoint[pos];
}

double	Collection::getSampleY(int pos)
{
	if(pos>=xpoint.size() || pos<0) return 0;
	return ypoint[pos];
}

void	Collection::sortDesc()
{
	int s=xpoint.size();
	for(int i=0;i<s;i++)
	{
		for(int j=0;j<s-1;j++)
		{
			if(ypoint[j+1]<ypoint[j])
			{
				double dtemp;
				dtemp=ypoint[j+1];
				ypoint[j+1]=ypoint[j];
				ypoint[j]=dtemp;
				for(int k=0;k<dimension;k++)
				{
					dtemp=xpoint[j+1][k];
					xpoint[j+1][k]=xpoint[j][k];
					xpoint[j][k]=dtemp;
				}
			}
		}
	}
}

void	Collection::Cut(int N)
{
	if(N<=0 ||  N>=xpoint.size()) return;
	xpoint.resize(N);
	ypoint.resize(N);
}


int	Collection::isSampleIn(Data x,double y)
{
	int s=xpoint.size();
	for(int i=0;i<s;i++)
	{
		double d=0.0;
		for(int j=0;j<dimension;j++)
			d+=(xpoint[i][j]-x[j])*(xpoint[i][j]-x[j]);
		if(sqrt(d)<1e-5) 
		{
			return i+1;
		}
	}
	return 0;
}

int	Collection::getSize() const
{
	return ypoint.size();
}

int	Collection::getDimension()
{
	return dimension;
}

void	Collection::clear()
{
	xpoint.resize(0);
	ypoint.resize(0);
}

Collection::~Collection()
{
}

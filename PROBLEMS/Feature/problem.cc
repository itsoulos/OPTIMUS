# include <problem.h>	
# include <stdio.h>
# include <stdlib.h>
# include <math.h>


static double sum2(Matrix x)
{
	double s=0.0;
	for(int i=0;i<x.size();i++) s+=x[i]*x[i];
	return s;
}

/*	02-11-03
 *	Συνάρτηση δημιουργίας. Καθορίζει
 *	την διάσταση του προβλήματος.
 * */
Problem::Problem(int d)
{
	dimension =d ;
	has_margins=MARGINS_NONE;
}

Problem::Problem()
{
	dimension =0;
	has_margins = MARGINS_NONE;
}

void	Problem::setDimension(int d)
{
	dimension = d;
}


/*	02-11-03
 *	Καθορίζει τα όρια του προβλήματος.
 * */
void	Problem::setMargin(Matrix x1,Matrix x2)
{
	lmargin = x1;
	rmargin = x2;
	has_margins=MARGINS_LEFT | MARGINS_RIGHT;
}

/*	02-11-03
 *	Καθορίζει το αριστερό όριο του προβλήματος.
 * */
void	Problem::setLeftMargin(Matrix x1)
{
	lmargin = x1;
	has_margins|= MARGINS_LEFT;
}

/*	02-11-03
 *	Καθορίζει το δεξιό άκρο του προβλήματος.
 * */
void	Problem::setRightMargin(Matrix x2)
{
	rmargin = x2;
	has_margins|= MARGINS_RIGHT;
}

/*	02-11-03
 *	Επιστρέφει την διάσταση του προβλήματος.
 * */
int	Problem::getDimension() const
{
	return dimension;
}

/*	02-11-03
 *	Επιστρέφει το αριστερό όριο του προβλήματος.
 * */
void	Problem::getLeftMargin(Matrix &x1)
{
	x1 = lmargin;
}

/*	02-11-03
 *	Επιστρέφει το δεξιό όριο του προβλήματος.
 * */
void	Problem::getRightMargin(Matrix &x2)
{
	x2 = rmargin;
}

double	Problem::funmin(Matrix x)
{
	return 0.0;
}

/*	02-11-03
 *	Επιστρέφει την αντικειμενική συνάρτηση του προβλήματος.
 * */
FUNCTION	Problem::getFunmin()
{
	return &Problem::funmin;
}

/*	02-11-03
 *	Επιστρέφει την αντικειμενική παράγωγο του προβλήματος.
 * */
GRADIENT	Problem::getGranal()
{
	return &Problem::granal;
}

/*	02-11-03
 *	Επιστρέφει 1 αν το δοθέν σημεία είναι εντός των 
 *	ορίων του προβλήματος.
 * */
int	Problem::isPointIn(Matrix x)
{
	if(!has_margins) return 1;
	for(int i=0;i<dimension;i++)
		if(x[i]<lmargin[i] || x[i]>rmargin[i]) return 0;
	return 1;
}

/*	02-11-03
 *	Παράγει ένα σημείο μέσα στα όρια του προβλήματος.
 *	Οι μέθοδοι καθολικής ελαχιστοποιήσεως πρέπει να 
 *	χρησιμοποιούν αυτήν την συνάρτηση.
 * */
void	Problem::getSample(Matrix &x)
{
	if(has_margins==(MARGINS_LEFT|MARGINS_RIGHT))
	{
		for(int i=0;i<dimension;i++) 
			x[i]=lmargin[i]+(rmargin[i]-lmargin[i])*(drand48());
	}
	else
	{
		for(int i=0;i<dimension;i++)
			x[i]=2.0*drand48()-1.0;
	}
}

void	Problem::getSample(Matrix &x1,double rc,Matrix &x)
{
	int s=x1.size();

	Matrix ksi;
	ksi.resize(s);
	double nrm=0.0;
	for(int i=0;i<s;i++)
		ksi[i]=2.0*drand48()-1.0;
	nrm=sqrt(sum2(ksi));
	for(int i=0;i<s;i++)
		do{
			
		x[i]=x1[i]+rc*drand48()*ksi[i]/nrm;
		}while(x[i]<lmargin[i] || x[i]>rmargin[i]);
}


/*	02-11-03
 *	Επιστρέφει την σημαία των margins.
 * */
int	Problem::hasMargins()
{
	return has_margins;
}


/*	02-11-03
 *	Επιστρέφει το ελάχιστο βήμα l για την line search, 
 *	έτσι ώστε το x+l*p να είναι εντός των ορίων.
 * */
double	Problem::getLsStep(Matrix x,Matrix p)
{
	double stepcb=1;
	for(int i=0;i<x.size();i++)
	{
		double amax;
		if(fabs(p[i])<1e-5) continue;
		if(p[i]>0) 
			amax=1.0/p[i]*(rmargin[i]-x[i]);
		else
		if(p[i]<0)
			amax=1.0/p[i]*(lmargin[i]-x[i]);
		if(amax<stepcb || i==0)
				stepcb=(amax);
		if(amax<0) printf("x=%.15lf p=%.15lf l=%lf r=%lf\n",
				x[i],p[i],lmargin[i],rmargin[i]);
	}
	return stepcb;
}

static double dmax(double a,double b)
{
	return a>b?a:b;
}

static double dmin(double a,double b)
{
	return a<b?a:b;
}
/*	02-11-03
 *	Φράσσει το σημείο x εντός των ορίων
 *	του προβλήματος.
 * */
void	Problem::bound(Matrix &x)
{
	if(has_margins==(MARGINS_LEFT|MARGINS_RIGHT))
	{
		for(int i=0;i<dimension;i++)
		{
			x[i]=dmax(x[i],lmargin[i]);
			x[i]=dmin(x[i],rmargin[i]);
			
		}
	}	
}

/*	02-11-03
 *	Είναι η αντικειμενική παράγωγος του προβλήματος.
 *	Αν δεν επανοριστεί υπολογίζεται με αριθμητικό τρόπο.
 * */
void	Problem::granal(Matrix x,Matrix &g)
{
	const double eps=1e-6;
	for(int i=0;i<g.size();i++) g[i]=0.0;
	double a,b;
	b=funmin(x);
	for(int i=0;i<x.size();i++)
	{
		x[i]+=eps;
		a=funmin(x);
		x[i]-=eps;
		g[i]=(a-b)/eps;
	}
}

/*	02-11-03
 *	Υπολογίζει το REDUCED RMS.
 * */
double	Problem::getGRMS(Matrix x,Matrix g)
{
	double s=0.0;
	for(int i=0;i<dimension;i++)
	{
		double v=g[i];
		if(fabs(x[i]-lmargin[i])<1e-8 && g[i]>0) v=0.0;
		if(fabs(x[i]-rmargin[i])<1e-8 && g[i]<0) v=0.0;
		s+=v*v;
	}
	return sqrt(1.0/dimension * s);
}

void	Problem::getSample(double *x)
{
	if(has_margins==(MARGINS_LEFT|MARGINS_RIGHT))
	{
		for(int i=0;i<dimension;i++) 
			x[i]=lmargin[i]+(rmargin[i]-lmargin[i])*drand48();
	}
	else
	{
		for(int i=0;i<dimension;i++)
			x[i]=2.0*drand48()-1.0;
	}
}

/*	09-11-03
 *	Αυτή η μέθοδος σχεδιάζει το γράφημα της συναρτήσεως. 
 *	Είναι εφαρμόσιμη μόνον στις πρώτες δύο διαστάσεις 
 *	του προβλήματος.
 * */
void	Problem::plot()
{
	/*
	if(dimension<2) return;
	double *xx,*yy,**zz;
	int N=100;
	xx=new double[N];
	yy=new double[N];
	zz=new double*[N];
	for(int i=0;i<N;i++) zz[i]=new double[N];
	Matrix xtemp(2);
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			xx[i]=lmargin(0)+(rmargin(0)-lmargin(0))*i*1.0/(N-1.0);
			yy[j]=lmargin(1)+(rmargin(1)-lmargin(1))*j*1.0/(N-1.0);
			xtemp(0)=xx[i];xtemp(1)=yy[j];
			zz[i][j]=funmin(xtemp);
		}
	}
	double zmin,zmax;
	plsdev("xwin");
	plinit();
	plMinMax2dGrid(zz,N,N,&zmax,&zmin);
	pladv(0);
	plcol0(1);
	plvpor(0,1,0,1);
	plwind(-1,1,-1,1);
	plw3d(1,1,0.8,lmargin(0),rmargin(0),lmargin(1),rmargin(1),zmin,zmax,33,24);
	plbox3("bnstu","xx'",0.0,0,"bnstu","yy'",0.0,0,"bcdmnstuv","zz'",0.0,0);
	plcol0(2);
//	plot3d(xx,yy,zz,N,N,DRAW_LINEXY,0);
	plsurf3d(xx,yy,zz,N,N,FACETED,0,0);
	getchar();
	plend();*/
}


void	Problem::plot(char *filename)
{
	/*
	if(dimension<2) return;
	double *xx,*yy,**zz;
	int N=100;
	xx=new double[N];
	yy=new double[N];
	zz=new double*[N];
	for(int i=0;i<N;i++) zz[i]=new double[N];
	Matrix xtemp(2);
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			xx[i]=lmargin(0)+(rmargin(0)-lmargin(0))*i*1.0/(N-1.0);
			yy[j]=lmargin(1)+(rmargin(1)-lmargin(1))*j*1.0/(N-1.0);
			xtemp(0)=xx[i];xtemp(1)=yy[j];
			zz[i][j]=funmin(xtemp);
		}
	}
	double zmin,zmax;
	plsdev("psc");
	plsfnam(filename);
	plinit();
	plMinMax2dGrid(zz,N,N,&zmax,&zmin);
	pladv(0);
	plcol0(1);
	plvpor(0,1,0,1);
	plwind(-1,1,-1,1);
	plw3d(1,1,0.8,lmargin(0),rmargin(0),lmargin(1),rmargin(1),zmin,zmax,33,24);
	plbox3("bnstu","xx'",0.0,0,"bnstu","yy'",0.0,0,"bcdmnstuv","zz'",0.0,0);
	plcol0(2);
//	plot3d(xx,yy,zz,N,N,DRAW_LINEXY,0);
	plsurf3d(xx,yy,zz,N,N,FACETED,0,0);
	plend();
	*/
}


double	Problem::getVolume() 
{
	double prd=1.0;
	for(int i=0;i<dimension;i++) prd*=(rmargin[i]-lmargin[i]);
	return prd;
}


double	Problem::getLogVolume() 
{
	double prd=0.0;
	for(int i=0;i<dimension;i++) prd+=log(rmargin[i]-lmargin[i]);
	return prd;
}

double	Problem::getGradientCriterion(Matrix x1,Matrix x2)
{
	double s=0;
	Matrix g1;
	g1.resize(x1.size());
	Matrix g2;
	g2.resize(x2.size());
	granal(x1,g1);
	granal(x2,g2);
	for(int i=0;i<x1.size();i++)
	{
		s=s+(x1[i]-x2[i])*(g1[i]-g2[i]);
	}
	return s>=0;
}

Problem::~Problem()
{
}

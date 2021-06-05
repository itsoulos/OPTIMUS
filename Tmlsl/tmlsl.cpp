#include "tmlsl.h"
Tmlsl::Tmlsl(Problem *p)
    :Optimizer(p)
{
    addParameter("tmlsl_maxiterations","200","Number of maximum iterations");
    addParameter("tmlsl_N","25","Number of TMLSL samples");
    addParameter("tmlsl_K","5","Number of graph minima");
}
void Tmlsl::Solve()
{
    Optimizer::Solve();
}

Tmlsl::~Tmlsl()
{

}

bool    Tmlsl::terminated()
{
    int multistart_maxiterations=params["tmlsl_maxiterations"].toString().toInt();
    double fmin=fabs(1.0+mbesty);
    if(iteration<=1) {x1=0;x2=0.0;}
    x1+=fmin;
    x2+=fmin * fmin;
    variance=x2/(iteration+1)-(x1/(iteration+1))*(x1/(iteration+1));
    variance=fabs(variance);
    if(mbesty<oldBesty)
    {
        oldBesty=mbesty;
        stopat=variance/2.0;
    }
    K=params["tmlsl_K"].toString().toInt();
    N=params["tmlsl_N"].toString().toInt();
    int w=minima.size();
    bool kannCriterion=(w*(iteration *N-1.0))/(iteration*N-w-2.0)<=w*1.0+0.5;
    printf("TMLSL. Minimum: %10.5lf Variance: %10.5lf StopAt: %10.5lf\n",
           mbesty,stopat,variance);
    if(fabs(stopat)<1e-8) stopat=variance/2.0;
    return (kannCriterion && iteration>=20) || (iteration>=multistart_maxiterations) ;// && (iteration>=multistart_maxiterations || (variance<=stopat && iteration>=20));
}


double getLogVolume(Problem *p)
{
    int n;
    n=p->getDimension();
    Data l=p->getLeftMargin();
    Data r=p->getRightMargin();
    double sum=0.0;
    for(int i=0;i<n;i++)
         sum+=log(r[i]-l[i]);
    return sum;
}

double  Tmlsl::getRC()
{
   double din,alrc,algam,vol=0.0;
   vol=getLogVolume(myProblem);
   int dim=myProblem->getDimension();
   if((dim/2)*2==dim)
   {
      algam=0.0;
      for(int i=2;i<=dim;i+=2) algam+=log(1.0*i);
    }
    else
    {
       algam=log(sqrt(M_PI))-(dim+1.0)/2.0*log(2.0);
       for(int i=2;i<=dim;i+=2) algam+=log(1.0*i);
     }
    din=iteration * N;//iter * (processes-1) * (Lmax/2);
    alrc=-log(sqrt(M_PI))+log(4.0*log(din)/din)/dim;
    alrc=alrc+vol/dim+algam/dim;
    return exp(alrc);
}

double sum2(Data &x,Data &y)
{
    double s=0.0;
    for(int i=0;i<(int)x.size();i++)
        s+=(x[i]-y[i])*(x[i]-y[i]);
    return s;
}


static bool isSampleIn(vector<Data> &s,Data &x)
{
    double minDist=1e+100;
    for(int i=0;i<(int)s.size();i++)
    {
        double dist=sum2(s[i],x);
        if(dist<minDist) minDist=dist;
    }
    if(minDist<1e-4) return true;
    return false;
}


void    Tmlsl::step()
{
    int pos;
    iteration++;
    K=params["tmlsl_K"].toString().toInt();
    N=params["tmlsl_N"].toString().toInt();
    double rc=getRC();
    sample.clear();
    sampley.clear();
    start.clear();
    Data x1,x2;
    x1.resize(myProblem->getDimension());
    x2.resize(myProblem->getDimension());
    Data dist;
    dist.resize(K);
    vector<int> indexDist;
    indexDist.resize(K);
    double y1,y2;

    for(int i=0;i<N;i++)
    {
            x1=myProblem->getRandomPoint();
            double old_value=mbesty;
            y1=myProblem->funmin(x1);
            mbesty=old_value;
            sample.push_back(x1);
            sampley.push_back(y1);
     }
        int w=minima.size();
        for(int i=0;i<w;i++)
        {
            x2=minima[i];
            y2=minimay[i];
            sample.push_back(x2);
            sampley.push_back(y2);
        }
        for(int i=0;i<N;i++)
        {
            int isGraphMinimum=1,isStartPoint=1;
            x1=sample[i];
            y1=sampley[i];
            for(int k=0;k<K;k++) indexDist[k]=-1;
            for(int k=0;k<K;k++)
            {
                for(int j=0;j<N+w;j++)
                {
                    if(i==j) continue;
                    x2=sample[j];
                    y2=sampley[j];
                    int found=0;
                    for(int l=0;l<k;l++) if(indexDist[l]==j) {found=1;break;}
                    if(found) continue;
                    double d=sqrt(sum2(x2,x1));
                    if(indexDist[k]==-1 || d<dist[k])
                    {
                        indexDist[k]=j;
                        dist[k]=d;
                    }
                }
                y2=sampley[indexDist[k]];
                if(y2<y1)
                {
                    isGraphMinimum=0;
                    break;
                }
            }
            if(!isGraphMinimum)
            {
                continue;
                double minDist=1e+10;
                int ind=-1;
                for(int j=0;j<(int)minima.size();j++)
                {
                    x2=minima[j];
                    double d=sqrt(sum2(x2,x1));
                    if(d<rc && d<minDist && myProblem->getGradientCriterion(x1,x2))
                    {
                        ind=j;
                        minDist=d;
                    }
                }
                continue;

            }
            for(int j=0;j<N+w;j++)
            {
                if(i==j) continue;
                x2=sample[j];
                y2=sampley[j];
                double d=sqrt(sum2(x2,x1));
                if(d<rc && y2<=y1)
                {
                    isStartPoint=0;
                    break;
                }
            }
            if(!isStartPoint)
            {
                continue;
                double minDist=1e+10;
                int ind=-1;
                for(int j=0;j<minima.size();j++)
                {
                    x2=minima[j];
                    double d=sqrt(sum2(x2,x1));
                    if(d<rc && d<minDist && myProblem->getGradientCriterion(x1,x2))
                    {
                        ind=j;
                        minDist=d;
                    }
                }
                continue;
            }
            y1=localSearch(x1);
            lsearch++;
            pos=isSampleIn(minima,x1);
            start.push_back(x1);
            if(!pos)
            {
                minima.push_back(x1);
                minimay.push_back(y1);
                int w=minima.size();
                if(w==1 || y1<mbesty) mbesty=y1;
            }
        }

}

void    Tmlsl::init()
{
    x1=0.0;
    x2=0.0;
    iteration=0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;
    mbesty=1e+100;
    minima.clear();
    sample.clear();
    start.clear();
    minimay.clear();
    lsearch = 0;
    maxRc=0.0;
    rc=0;
    drc=0.0;
    params["tmlsl_N"]=QString::number(10 * myProblem->getDimension());
}

void    Tmlsl::done()
{

}

Optimizer *createOptimizer(Problem *p)
{
    return new Tmlsl(p);
}

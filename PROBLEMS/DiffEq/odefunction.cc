# include <math.h>
# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <demodel.h>
# include <mlpmodel.h>
# include <rbfmodel.h>
# include <nncmodel.h>
using namespace std;

#pragma gcc optimize("Ofast")
extern "C"
{
extern double	dode2ff(double x,double y,double yy,double yyy,double dy,double dyy,double dyyy);
extern double	ode2ff(double x,double y,double yy,double yyy);
extern double	dode1ff(double x,double y,double yy,double dy,double dyy);
extern double	ode1ff(double x,double y,double yy);
extern double	getf1();
extern double	getf0();
extern double	getff0();
extern int      getkind();
extern double	getx0();
extern double   getx1();

double lambda = 100.0;
QString model = "mlp";
int     weights  = 10;
int     npoints = 100;
DeModel *demodel = NULL;
const int odedimension = 1;


/**
 * @brief init
 * @param data
 * Available options:
 * 1) model (could be mlp, rbf, nnc)
 * 2) lambda (the lambda factor, lambda>1  eg. lambda=100.0)
 * 3) weights (for mlp, rbf models)
 * 4) npoints (to split the interval [x0,x1]
 */

void    init(QJsonObject data)
{
    if(data.contains("model"))
        model = data["model"].toString();

    if(data.contains("lambda"))
        lambda = data["lambda"].toString().toDouble();

    if(data.contains("weights"))
        weights = data["weights"].toString().toInt();

    if(data.contains("npoints"))
        npoints = data["npoints"].toString().toInt();

    if (model == "mlp")
        demodel = new MlpModel(odedimension,weights);
    else
    if(model == "rbf")
        demodel = new RbfModel(odedimension,weights);
    else
    if(model == "nnc")
        demodel  = new NncModel(odedimension);
}

int	getdimension()
{
    if(model == "mlp")
        return  (odedimension+2)*weights;
    else
    if(model == "rbf")
        return ((RbfModel *)demodel)->getVariablesSize();
    else
    if(model == "nnc")
        return 10 * weights;
    return 1;
}

void    getmargins(vector<Interval> &x)
{
    if(model=="nnc")
    {
        for(int i=0;i<(int)x.size();i++)
            x[i]=Interval(0.0,255.0);
        return;
    }

        for(int i=0;i<(int)x.size();i++)
                x[i]=Interval(-100,100);
}


//f(x)
double	funmin(vector<double> &a)
{
    double x=0.0;
    double sum = 0.0;

   //initial conditions
    double p0=0.0;
    double pp0=0.0;
    double p1=0.0;

    int first = 1;
    if(model == "mlp")
    {
        ((MlpModel *)demodel)->setWeights(a);
    }
    else
    if(model == "rbf")
    {
        ((RbfModel *)demodel)->setVariables(a);
    }
    else
    if(model == "nnc")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
            ia[i]=(int)a[i];

        ((NncModel *)demodel)->setChromosome(ia);
    }
    Data xx;
    xx.resize(odedimension);
    int kind = getkind();
    double px,dpx,dpx2;
    for(x=getx0();x<=getx1();x+=(getx1()-getx0())/npoints)
    {
        //compute p(x), dpx
        xx[0]=x;
         px = demodel->eval(xx);
         dpx = demodel->evalDeriv(xx,0);
         dpx2 = 0.0;
        if(kind==2 || kind==3)
            dpx2 = demodel->evalSecondDeriv(xx,0);

      if(first) {p0=px;first=0;if(kind==2) pp0=dpx;}
        else
      {
          if(kind==1)
        sum = sum + pow(ode1ff(x,px,dpx),2.0);
          else
              sum=sum+pow(ode2ff(x,px,dpx,dpx2),2.0);
      }

    }
    if(kind==3)
        p1=px;
    if(kind==1)
        return sum*(1.0+lambda * pow(p0-getf0(),2.0));
    else
    if(kind==2)
        return sum*(1.0+lambda * pow(p0-getf0(),2.0)+
                lambda * pow(pp0-getff0(),2.0));
    else
    if(kind==3)
        return sum*(1.0+lambda*pow(p0-getf0(),2.0)+
                lambda * pow(p1-getf1(),2.0));
}

QJsonObject    done(vector<double> &x)
{
    funmin(x);
    double testError = 0.0;
    double classError = 0.0;
    if(demodel!=NULL)
    {
        Data xx;
        xx.resize(odedimension);
        double px,dpx,dpx2;
        int kind = getkind();

        for(double ax=getx0();ax<=getx1();ax+=(getx1()-getx0())/(2*npoints))
        {
            xx[0]=ax;
            px = demodel->eval(xx);
            dpx = demodel->evalDeriv(xx,0);
            dpx2 = 0.0;
           if(kind==2 || kind==3)
               dpx2 = demodel->evalSecondDeriv(xx,0);

           if(kind==1)
           testError = testError + pow(ode1ff(ax,px,dpx),2.0);
           else
                testError=testError+pow(ode2ff(ax,px,dpx,dpx2),2.0);
        }
        delete demodel;
    }
    QJsonObject xx;
    xx["testError"]=testError;
    xx["classError"]=classError;
    return xx;
}

static double dmax(double a,double b){return a>b?a:b;}
void	granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<getdimension();i++)
    {
        double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=funmin(x);
        x[i]-=2.0 *eps;
        double v2=funmin(x);
        g[i]=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
    }
}
}

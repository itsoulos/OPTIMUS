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
# include <gdfmodel.h>
# include <rulermodel.h>
# include <polymodel.h>
using namespace std;

#pragma gcc optimize("Ofast")
extern "C"
{
double lambda = 100.0;
QString model = "mlp";
int     weights  = 10;
int     npoints = 100;
DeModel *demodel=NULL;
const int pdedimension = 2;
extern double	getx0();
extern double	getx1();
extern double	gety0();
extern double	gety1();
extern double	f0(double y);
extern double	f1(double y);
extern double   g0(double x);
extern double   g1(double x);
extern double   pde(double x, double y, double v, double x1, double y1, double x2, double y2);
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
        demodel = new MlpModel(pdedimension,weights);
    else
    if(model == "rbf")
        demodel = new RbfModel(pdedimension,weights);
    else
    if(model == "nnc")
        demodel  = new NncModel(pdedimension);
    else
    if(model == "gdf")
        demodel = new GdfModel(pdedimension);
    else
    if(model == "ruler")
        demodel = new RulerModel(pdedimension);
    else
    if(model == "poly")
        demodel = new PolyModel(pdedimension,weights);
}


int	getdimension()
{
    if(model == "mlp")
        return  (pdedimension+2)*weights;
    else
    if(model == "rbf")
        return ((RbfModel *)demodel)->getVariablesSize();
    else
    if(model == "nnc")
        return 10 * weights;
    else
    if(model == "gdf")
        return 10 * weights;
    else
    if(model == "ruler")
        return 10 * weights;
    else
    if(model == "poly")
        return pdedimension * weights;
    return 1;
}

void    getmargins(vector<Interval> &x)
{
    if(model=="nnc" || model == "gdf" || model == "ruler")
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
    double sum = 0.0;
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
    else
    if(model=="gdf")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
            ia[i]=(int)a[i];
        ((GdfModel *)demodel)->setChromosome(ia);
    }
    else
    if(model == "ruler")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
            ia[i]=(int)a[i];
        ((RulerModel *)demodel)->setChromosome(ia);
    }
    else
    if(model == "poly")
    {
        ((PolyModel *) demodel)->setWeights(a);
    }
    Data xx;
    xx.resize(pdedimension);
    //compute value
    for(int ix=0;ix<=npoints;ix++)
    {
        for(int iy=0;iy<=npoints;iy++)
        {
            xx[0]=getx0()+ix*(getx1()-getx0())/npoints;
            xx[1]=gety0()+iy*(gety1()-gety0())/npoints;
            double val = demodel->eval(xx);
            double valx1 = demodel->evalDeriv(xx,0);
            double valy1 = demodel->evalDeriv(xx,1);
            double valx2 = demodel->evalSecondDeriv(xx,0);
            double valy2 = demodel->evalSecondDeriv(xx,1);
            double value = pde(xx[0],xx[1],val,valx1,valy1,valx2,valy2);
            value = value * value;
            if(isnan(value) || isinf(value)) return 1e+100;
            sum = sum +value;
        }
    }
    //compute penalties
    double penalty1=0.0,penalty2=0.0,penalty3=0.0,penalty4=0.0;
    for(int ix=0;ix<=npoints;ix++)
    {
        double x=getx0()+ix*(getx1()-getx0())/npoints;
        double val1 = g0(x);
        xx[0]=x;
        xx[1]=gety0();
        penalty3+=pow(demodel->eval(xx)-val1,2.0);
        xx[1]=gety1();
        double val2 = g1(x);
        penalty4+=pow(demodel->eval(xx)-val2,2.0);

    }
    for(int iy=0;iy<=npoints;iy++)
    {
        double y=gety0()+iy*(gety1()-gety0())/npoints;
        double val3 = f0(y);
        xx[0]=getx0();
        xx[1]=y;
        penalty1+=pow(demodel->eval(xx)-val3,2.0);
        xx[0]=getx1();
        double val4 = f1(y);
        penalty2+=pow(demodel->eval(xx)-val4,2.0);
    }
    return sum+lambda*(penalty1+penalty2+penalty3+penalty4);
}
QJsonObject    done(vector<double> &x)
{
    funmin(x);
    double testError = 0.0;
    double classError = 0.0;
    Data xx;
    xx.resize(pdedimension);
    //compute value
    for(int ix=0;ix<=2*npoints;ix++)
    {
        for(int iy=0;iy<=2*npoints;iy++)
        {
            xx[0]=getx0()+ix*(getx1()-getx0())/(2*npoints);
            xx[1]=gety0()+iy*(gety1()-gety0())/(2*npoints);
            double val = demodel->eval(xx);
            double valx1 = demodel->evalDeriv(xx,0);
            double valy1 = demodel->evalDeriv(xx,1);
            double valx2 = demodel->evalSecondDeriv(xx,0);
            double valy2 = demodel->evalSecondDeriv(xx,1);
            double value = pde(xx[0],xx[1],val,valx1,valy1,valx2,valy2);
            value = value * value;
            testError = testError +value;
        }
    }
     QJsonObject jxx;
     jxx["testError"]=testError;
     jxx["classError"]=classError;
     return jxx;
}

static double pdemax(double a,double b){return a>b?a:b;}
void	granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<getdimension();i++)
    {
        double eps=pow(1e-18,1.0/3.0)*pdemax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=funmin(x);
        x[i]-=2.0 *eps;
        double v2=funmin(x);
        g[i]=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
    }
}

}

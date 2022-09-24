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
vector<DeModel*> demodel;
const int sodedimension = 1;

extern double	getx0();
extern double	getx1();
extern void     systemfun(int node, double x, double *y, double *yy,double *res);
extern void     systemf0(int node,double *f0);
extern int      getnode();
extern void	systemder(int node, double x, double *y, double *yy,double dy,double dyy,double *res);

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

    demodel.resize(getnode());

    for(int i=0;i<getnode();i++)
    {
    if (model == "mlp")
        demodel[i] = new MlpModel(sodedimension,weights);
    else
    if(model == "rbf")
        demodel[i] = new RbfModel(sodedimension,weights);
    else
    if(model == "nnc")
        demodel[i]  = new NncModel(sodedimension);
    else
    if(model == "gdf")
        demodel[i] = new GdfModel(sodedimension);
    else
    if(model == "ruler")
        demodel[i] = new RulerModel(sodedimension);
    else
    if(model == "poly")
        demodel[i] = new PolyModel(sodedimension,weights);
    }
}

int	getdimension()
{
    if(model == "mlp")
        return  getnode()*(sodedimension+2)*weights;
    else
    if(model == "rbf")
        return getnode()*((RbfModel *)demodel[0])->getVariablesSize();
    else
    if(model == "nnc")
        return getnode()*10 * weights;
    else
    if(model == "gdf")
        return getnode()*10 * weights;
    else
    if(model == "ruler")
        return getnode()*10 * weights;
    else
    if(model == "poly")
        return getnode()*sodedimension * weights;
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

    if(model == "mlp")
    {
        for(int i=0;i<(int)x.size();i++)
                x[i]=Interval(-20,20);
    }
    else
    {
        for(int i=0;i<(int)x.size();i++)
                x[i]=Interval(-10,10);
       // for(int i=x.size()-1;i>=x.size()-1-weights;i--)
       //     x[i]=Interval(0.01,10);
    }

}

//f(x)
double	funmin(vector<double> &a)
{

    double x=0.0;
    double sum = 0.0;

    int first = 1;
    if(model == "mlp")
    {
        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(a.size()/getnode());

            for(int j=0;j<suba.size();j++)
                suba[j]=a[i*suba.size()+j];
            ((MlpModel *)demodel[i])->setWeights(suba);
        }
    }
    else
    if(model == "rbf")
    {
        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(a.size()/getnode());
            for(int j=0;j<suba.size();j++)
                suba[j]=a[i*suba.size()+j];
            ((RbfModel *)demodel[i])->setVariables(suba);
        }
    }
    else
    if(model == "nnc")
    {


        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(a.size()/getnode());
            for(int j=0;j<suba.size();j++)
                suba[j]=a[i*suba.size()+j];
            vector<int> ia;
            ia.resize(suba.size());
            for(int k=0;k<(int)suba.size();k++)
            {
                ia[k]=(int)suba[k];
                if(ia[k]<0 || ia[k]>255)
                    return 1e+100;
            }
            ((NncModel *)demodel[i])->setChromosome(ia);
        }
    }
    else
    if(model=="gdf")
    {

        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(a.size()/getnode());
            for(int j=0;j<suba.size();j++)
                suba[j]=a[i*suba.size()+j];
            vector<int> ia;
            ia.resize(suba.size());
            for(int k=0;k<(int)suba.size();k++)
            {
                ia[k]=(int)suba[k];
                if(ia[k]<0 || ia[k]>255)
                    return 1e+100;
            }
            ((GdfModel *)demodel[i])->setChromosome(ia);
        }
    }
    else
    if(model == "ruler")
    {

        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(a.size()/getnode());
            for(int j=0;j<suba.size();j++)
                suba[j]=a[i*suba.size()+j];
            vector<int> ia;
            ia.resize(suba.size());
            for(int k=0;k<(int)suba.size();k++)
            {
                ia[k]=(int)suba[k];
                if(ia[k]<0 || ia[k]>255)
                    return 1e+100;
            }
            ((RulerModel *)demodel[i])->setChromosome(ia);
        }
    }
    else
    if(model == "poly")
    {
        for(int i=0;i<getnode();i++)
        ((PolyModel *) demodel[i])->setWeights(a);
    }
    Data xx;
    xx.resize(sodedimension);

    double *px=new double[getnode()],*dpx=new double[getnode()],
            *res=new double[getnode()],*res0=new double[getnode()];

    for(int ik=0;ik<=npoints;ik++)
    {
        xx[0]=getx0()+ik*(getx1()-getx0())/npoints;
        for(int i=0;i<getnode();i++)
        {
            px[i]=demodel[i]->eval(xx);
            dpx[i]=demodel[i]->evalDeriv(xx,0);
            if(isnan(px[i]) || isinf(px[i]) || isnan(dpx[i]) || isinf(dpx[i]))
              {
                delete[] px;
                delete[] dpx;
                delete[] res;
                delete[] res0;
                return 1e+100;
            }
            if(fabs(px[i])>=1e+8 || fabs(dpx[i])>=1e+8)
            {
                delete[] px;
                delete[] dpx;
                delete[] res;
                delete[] res0;
                return 1e+100;
            }
             if(first) res0[i]=px[i];
        }
        systemfun(getnode(),xx[0],px,dpx,res);
        for(int i=0;i<getnode();i++)
        {
            if(isnan(res[i]) || isinf(res[i]))
            {
                delete[] px;
                delete[] dpx;
                delete[] res;
                delete[] res0;
                return 1e+100;
            }
            if(fabs(res[i])>=1e+8)
            {
                delete[] px;
                delete[] dpx;
                delete[] res;
                delete[] res0;
                return 1e+100;
            }
            sum+=res[i]*res[i];

        }
        first = 0;
    }
    systemf0(getnode(),px);
    double penalty = 0.0;
    for(int i=0;i<getnode();i++)
        penalty+=lambda * pow(res0[i]-px[i],2.0);

   sum=sum + penalty;
    delete[] px;
    delete[] dpx;
    delete[] res;
    delete[] res0;
    return sum;
}
QJsonObject    done(vector<double> &x)
{
    funmin(x);
    QJsonObject jxx;
    double testError = 0.0;
    double classError = 0.0;
    Data xx;
    xx.resize(sodedimension);
    double *px=new double[getnode()],*dpx=new double[getnode()],
            *res=new double[getnode()];

    FILE *fp=fopen("sode.plot","w");
    for(int ik=0;ik<=2*npoints;ik++)
    {
        xx[0]=getx0()+ik*(getx1()-getx0())/(2*npoints);
    fprintf(fp,"%lf ",xx[0]);
        for(int i=0;i<getnode();i++)
        {
            px[i]=demodel[i]->eval(xx);

            dpx[i]=demodel[i]->evalDeriv(xx,0);
        }




       systemfun(getnode(),xx[0],px,dpx,res);
       for(int i=0;i<getnode();i++)
       {
           fprintf(fp,"%lf ",px[i]);
           testError = testError+res[i]*res[i];
       }
         fprintf(fp,"\n");

    }
    fclose(fp);
    delete[] px;
    delete[] dpx;
    delete[] res;

    for(int i=0;i<getnode();i++)
        delete demodel[i];

    jxx["testError"]=testError;
    jxx["classError"]=classError;
    return jxx;
}
static double sodemax(double a,double b){return a>b?a:b;}
void	granal(vector<double> &x,vector<double> &g)
{

    if(model == "mlp2")
    {
        for(int i=0;i<getnode();i++)
        {
            vector<double> suba;
            suba.resize(x.size()/getnode());

            for(unsigned int j=0;j<suba.size();j++)
                suba[j]=x[i*suba.size()+j];
            ((MlpModel *)demodel[i])->setWeights(suba);
        }

        for(int i=0;i<g.size();i++) g[i]=0.0;
        Data tempg;
        Data tempg2;
        tempg.resize(g.size());
        tempg2.resize(g.size());

        Data xx;
        xx.resize(1);

        double *y=new double[getnode()];
        double *yy=new double[getnode()];
        double *res=new double[getnode()];
        double *res2=new double[getnode()];
        double *f0=new double[getnode()];

        for(int ik=0;ik<=npoints;ik++)
        {
            xx[0]=getx0()+ik*(getx1()-getx0())/npoints;
            int gcount = 0;
            for(int j=0;j<getnode();j++)
            {
                Data g1,g2;
                g1.resize(x.size()/getnode());
                g2.resize(x.size()/getnode());
                double g1val[g1.size()];
                double g2val[g2.size()];
                y[j]=demodel[j]->eval(xx);
                yy[j]=demodel[j]->evalDeriv(xx,0);
                demodel[j]->getModelDeriv(xx,g1);
                demodel[j]->getXDeriv(xx,0,g2);
                for(int k=0;k<g1.size();k++)
                {
                    g1val[k]=g1[k];
                    g2val[k]=g2[k];
                }
                systemder(j,xx[0],y,yy,g1val[j],g2val[j],res2);
            }
            systemfun(getnode(),xx[0],y,yy,res);
            for(unsigned int j=0;j<g.size();j++)
            {

                for(int k=0;k<getnode();k++)
                {

                    g[j]+=2.0*res[k]*res2[k];
                }
            }
        }

        systemf0(getnode(),f0);
        int gcount = 0;
        for(int i=0;i<getnode();i++)
        {
            xx[0]=getx0();
            double v=demodel[i]->eval(xx)-f0[i];
            Data g1;
            g1.resize(x.size()/getnode());
            demodel[i]->getModelDeriv(xx,g1);
            for(unsigned int j=0;j<g1.size();j++)
            {
                g[gcount++]+=2.0 * lambda * v * g1[j];
            }
            /*
            for(int j=0;j<g.size();j++)
            {
                double out=tempg[j];
                g[j]+=2.0*LAMBDA*v*out;
            }*/
        }
        delete[] y;
        delete[] yy;
        delete[] res;
        delete[] res2;
        delete[] f0;
        for(int i=0;i<getdimension();i++)
        {
            double eps=pow(1e-18,1.0/3.0)*sodemax(1.0,fabs(x[i]));
            x[i]+=eps;
            double v1=funmin(x);
            x[i]-=2.0 *eps;
            double v2=funmin(x);
            double gg=(v1-v2)/(2.0 * eps);
            printf("Difference in gradient[%d]=%lf \n",i,fabs(gg-g[i]));
            x[i]+=eps;
        }
    return;
    }

    for(int i=0;i<getdimension();i++)
    {
        double eps=pow(1e-18,1.0/3.0)*sodemax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=funmin(x);
        x[i]-=2.0 *eps;
        double v2=funmin(x);
        g[i]=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
    }


}
}

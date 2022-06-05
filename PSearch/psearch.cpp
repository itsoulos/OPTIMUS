#include "psearch.h"

double r8_max(double x, double y)
{
  double value;

  if (y < x)
  {
    value = x;
  }
  else
  {
    value = y;
  }
  return value;
}
double r8_epsilon()
{
  const double value = 2.220446049250313E-016;
  return value;
}


class OneDimProblem :public IntervalProblem
{
private:
    Problem *myProblem;
    Data    trialx;
    int     current_dimension;
    double a,b,c,m,e,t;
public:
    OneDimProblem(Problem *p,Data &x,int d);
    virtual double      funmin(Data &x);
    virtual void        granal(Data &x,Data &g);
    double  localSearch(double &x);
    ~OneDimProblem();
};

OneDimProblem::OneDimProblem(Problem *p,Data &x,int d)
    :IntervalProblem(1)
{
    myProblem = p;
    trialx = x;
    current_dimension = d;
    margin.resize(1);
    Data xl = myProblem->getLeftMargin();
    Data xr = myProblem->getRightMargin();
    a = xl[0];
    b = xr[0];
    margin[0]=Interval(xl[current_dimension],xr[current_dimension]);
    c = (a - b) / 2.0;
    m = 100.0;
    e = sqrt(r8_epsilon());
    t = sqrt(r8_epsilon());
}

double      OneDimProblem::funmin(Data &x)
{
    trialx[current_dimension]=x[0];
    return myProblem->funmin(trialx);
}

void        OneDimProblem::granal(Data &x,Data &g)
{
    trialx[current_dimension]=x[0];
    myProblem->granal(trialx,g);
}


double  OneDimProblem::localSearch(double &x)
{
    double a0;
    double a2;
    double a3;
    double d0;
    double d1;
    double d2;
    double h;
    int k;
    double m2;
    double macheps;
    double p;
    double q;
    double qs;
    double r;
    double s;
    double sc;
    double y;
    double y0;
    double y1;
    double y2;
    double y3;
    double yb;
    double z0;
    double z1;
    double z2;


    a0 = b;
    x = a0;
    a2 = a;
    Data xx;
    xx.resize(1);
    xx[0]=b;
    y0 = funmin(xx);
    yb = y0;
    xx[0]=a;
    y2 = funmin(xx);
    y = y2;

    if (y0 < y)
    {
      y = y0;
    }
    else
    {
      x = a;
    }

    if (m <= 0.0 || b <= a)
    {
      return y;
    }

    macheps = r8_epsilon();

    m2 = 0.5 * (1.0 + 16.0 * macheps) * m;

    if (c <= a || b <= c)
    {
      sc = 0.5 * (a + b);
    }
    else
    {
      sc = c;
    }

    // y1 = f ( sc );/////////////////////////////////////////////////////////////
    y1 = myProblem->funmin(trialx);
    k = 3;
    d0 = a2 - sc;
    h = 9.0 / 11.0;

    if (y1 < y)
    {
      x = sc;
      y = y1;
    }
    //
    //  Loop.
    //
    for (;;)
    {
      d1 = a2 - a0;
      d2 = sc - a0;
      z2 = b - a2;
      z0 = y2 - y1;
      z1 = y2 - y0;
      r = d1 * d1 * z0 - d0 * d0 * z1;
      p = r;
      qs = 2.0 * (d0 * z1 - d1 * z0);
      q = qs;

      if (k < 1000000 || y2 <= y)
      {
        for (;;)
        {
          if (q * (r * (yb - y2) + z2 * q * ((y2 - y) + t)) <
              z2 * m2 * r * (z2 * q - r))
          {
            a3 = a2 + r / q;
            // y3 = f ( a3 );/////////////////////////////////////////////////////////////
            y3 = myProblem->funmin(trialx);

            if (y3 < y)
            {
              x = a3;
              y = y3;
            }
          }
          k = ((1611 * k) % 1048576);
          q = 1.0;
          r = (b - a) * 0.00001 * (double)(k);

          if (z2 <= r)
          {
            break;
          }
        }
      }
      else
      {
        k = ((1611 * k) % 1048576);
        q = 1.0;
        r = (b - a) * 0.00001 * (double)(k);

        while (r < z2)
        {
          if (q * (r * (yb - y2) + z2 * q * ((y2 - y) + t)) <
              z2 * m2 * r * (z2 * q - r))
          {
            a3 = a2 + r / q;
            // y3 = f ( a3 );/////////////////////////////////////////////////////////////////
            xx[0]=a3;
            y3 = funmin(xx);

            if (y3 < y)
            {
              x = a3;
              y = y3;
            }
          }
          k = ((1611 * k) % 1048576);
          q = 1.0;
          r = (b - a) * 0.00001 * (double)(k);
        }
      }

      r = m2 * d0 * d1 * d2;
      s = sqrt(((y2 - y) + t) / m2);
      h = 0.5 * (1.0 + h);
      p = h * (p + 2.0 * r * s);
      q = q + 0.5 * qs;
      r = -0.5 * (d0 + (z0 + 2.01 * e) / (d0 * m2));

      if (r < s || d0 < 0.0)
      {
        r = a2 + s;
      }
      else
      {
        r = a2 + r;
      }

      if (0.0 < p * q)
      {
        a3 = a2 + p / q;
      }
      else
      {
        a3 = r;
      }

      for (;;)
      {
        a3 = r8_max(a3, r);

        if (b <= a3)
        {
          a3 = b;
          y3 = yb;
        }
        else
        {
          // y3 = f ( a3 );///////////////////////////////////////////////
            xx[0]=a3;
          y3 = funmin(xx);
        }

        if (y3 < y)
        {
          x = a3;
          y = y3;
        }

        d0 = a3 - a2;

        if (a3 <= r)
        {
          break;
        }

        p = 2.0 * (y2 - y3) / (m * d0);

        if ((1.0 + 9.0 * macheps) * d0 <= fabs(p))
        {
          break;
        }

        if (0.5 * m2 * (d0 * d0 + p * p) <= (y2 - y) + (y3 - y) + 2.0 * t)
        {
          break;
        }
        a3 = 0.5 * (a2 + a3);
        h = 0.9 * h;
      }

      if (b <= a3)
      {
        break;
      }

      a0 = sc;
      sc = a2;
      a2 = a3;
      y0 = y1;
      y1 = y2;
      y2 = y3;
    }

    return y;

}

OneDimProblem::~OneDimProblem()
{

}

PSearch::PSearch(Problem *p)
    :Optimizer(p)
{
    nsamples = 0;
    addParameter("psearch_samples", "100", "Number of pBrent samples");
}

bool    PSearch::terminated()
{
    return true;
}

void    PSearch::step()
{

    bestPerDim.resize(myProblem->getDimension());
    bestPoint.resize(myProblem->getDimension());

    for(int i=0;i<nsamples;i++)
    {
        for(int d = 0; d< myProblem->getDimension();d++)
        {
            OneDimProblem onedim(myProblem,points[i],d);
            double initialX = points[i][d];
            double y  = onedim.localSearch(initialX);

            if(i==0 || y<bestPerDim[d])
            {
                bestPerDim[d]=y;
                bestPoint[d]=initialX;
            }
        }
    }

}


void    PSearch::init()
{
    nsamples = params["psearch_samples"].toString().toInt();
    points.resize(nsamples);
    for (int i = 0; i < nsamples; i++)
      points[i] = myProblem->getRandomPoint();
}

void    PSearch::done()
{
    double yy = localSearch(bestPoint);
    printf("Final outcome: %10.5lf\n",yy);
}

void    PSearch::Solve()
{
    Optimizer::Solve();
}

PSearch::~PSearch()
{

}

extern "C" PSEARCH_EXPORT Optimizer *createOptimizer(Problem *p)
{
  return new PSearch(p);
}

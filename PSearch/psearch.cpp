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

class OneDimProblem : public IntervalProblem
{
private:
  Problem *myProblem;
  Data trialx;
  int current_dimension;
  double a, b, c, m, e, t;

public:
  OneDimProblem(Problem *p, Data &x, int d);
  virtual double funmin(Data &x);
  virtual void granal(Data &x, Data &g);

  double brentSearch(double &x);
  double newtonSearch(double &x);
  double secantSearch(double &x);
  double goldenSearch(double xd1, double xd2);
  double bisectionSearch(double xd1, double xd2);
  ~OneDimProblem();
};

OneDimProblem::OneDimProblem(Problem *p, Data &x, int d)
    : IntervalProblem(1)
{
  myProblem = p;
  trialx = x;
  current_dimension = d;
  margin.resize(1);
  Data xl = myProblem->getLeftMargin();
  Data xr = myProblem->getRightMargin();
  a = xl[0];
  b = xr[0];
  margin[0] = Interval(xl[current_dimension], xr[current_dimension]);
  c = (a - b) / 2.0;
  m = 100.0;
  e = sqrt(r8_epsilon());
  t = sqrt(r8_epsilon());
}

double OneDimProblem::funmin(Data &x)
{
  trialx[current_dimension] = x[0];
  return myProblem->funmin(trialx);
}

void OneDimProblem::granal(Data &x, Data &g)
{
  trialx[current_dimension] = x[0];
  g.resize(1);
  myProblem->granal(trialx, g);
}

double OneDimProblem::bisectionSearch(double xd1, double xd2)
{
/* a τροπος
  Data xx,gg;
  xx.resize(1);
  double midX, check = 1e+100;
  int iter = 0;
  while (check > 1e-8)
  {
    midX = (b - a) / 2.0;
    xx.at(0) = midX;

    granal(xx, gg);

    if (gg[0] > 0)
      b = midX;
    else
      a = midX;
    check = fabs(b - a);
    iter++;
    printf("a=%f b=%f midX=%f check=%f\n", a, b, midX, check);
  }
  //β τροπος χωρίς παράγωγο
    Data xx,x1,x2;
    xx.resize(1);
    x1.resize(1);
    x2.resize(1);
    double aa = a,bb = b;
    double midX, check = 1e+100;
    int iter = 0;
    while (check > 1e-8)
    {
      midX = (bb - aa) / 2.0;
      xx.at(0) = midX;
      x1.at(0) = aa;
      x2.at(0) = bb;

      if (this->funmin(xx) * this->funmin(x1) < 0)
        bb = midX;
      else
        aa = midX;
      check = fabs(bb - aa);
      iter++;
      printf("a=%f b=%f midX=%f check=%f\n", a, b, midX, check);
    }
*/

    Data xx,x1,x2;
    xx.resize(1);
    x1.resize(1);
    x2.resize(1);
    double check = 1e+100;
    int iter = 0;
    x1.at(0) = xd1;
    x2.at(0) = xd2;
    xx.at(0) = x1.at(0);
    if (this->funmin(x1) * this->funmin(x2)>=0)
    {
        printf("No solution\n");
        return 0;
    }
    while (check > 1e-8)
    {
      xx.at(0) = (x2.at(0) - x1.at(0)) / 2.0;
      if (funmin(xx)==0)
          break;
      else if (this->funmin(xx) * this->funmin(x1) < 0)
        x2.at(0) = xx.at(0);
      else
        x1.at(0) = xx.at(0);
      iter++;
      printf("a=%f b=%f midX=%f check=%f\n", x1.at(0), x2.at(0), xx.at(0), check);
      check = fabs(x2.at(0) - x1.at(0));
    }
  return funmin(xx);
}

double OneDimProblem::goldenSearch(double xd1, double xd2)
{

  Data x1, x2;
  x1.resize(1);
  x2.resize(1);
  int iter = 0;
  double newL, newH, check = xd2 - xd1;
  while (check > 1e-8)
  {
    newL = xd1 + (xd2 - xd1) * ((3 - sqrt(5)) / 2.0);
    newH = xd2 - (xd2 - xd1) * ((3 - sqrt(5)) / 2.0);
    x1.at(0) = newL;
    x2.at(0) = newH;
    if (funmin(x1) < funmin(x2))
    {
      xd2 = newH;
    }
    else
    {
      xd1 = newL;
    }
    check = fabs(xd2 - xd1);
    iter++;
  }
  return funmin(x1);
}
double OneDimProblem::secantSearch(double &x)
{
  Data xx, x1, x2, gg, g1, g2;
  xx.resize(1);
  x1.resize(1);
  x2.resize(1);
  int iter = 0;
  double eps = 0.000000000000001;
  double newX, check = 1e+100;

  while (check > 1e-8)
  {
    xx.at(0) = x;
    granal(xx, gg);
    x1[0] = xx[0] + eps;
    granal(x1, g1);
    x2[0] = xx[0] - eps;
    granal(x2, g2);
    double r = (g1.at(0) - g2.at(0)) / (2.0 * eps);
    newX = x - gg.at(0) / r;
    check = fabs(newX - x);
    printf("x=%f newX=%f r=%f check=%f\n", x, newX, r, check);
    x = newX;
    iter++;
  }

  return funmin(xx);
}

double OneDimProblem::newtonSearch(double &x)
{
  Data xx;
  xx.resize(1);
  Data gg;
  Data ggg;
  int iter = 0;
  double newX, check = 1e+100;
  xx[0] = x;
  while (check > 1e-4)
  {
    granal(xx, gg);
    granal(gg, ggg); // error in second derivative
    newX = xx.at(0) - gg.at(0) / ggg.at(0);
    printf("x=%f y=%f p1=%f p2%f check=%f\n", xx[0], newX, gg.at(0), ggg.at(0), check);
    iter++;
    check = fabs(fabs(newX) - fabs(xx.at(0)));
    xx[0] = newX;
  }
  return funmin(xx);
}
double OneDimProblem::brentSearch(double &x)
{
  int end = 100, iter = 0;
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
  xx[0] = b;
  y0 = funmin(xx);
  yb = y0;
  xx[0] = a;
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

  // y1 = f ( sc );
  xx[0] = sc;
  y1 = funmin(xx);
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
    iter++;
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
          // y3 = f ( a3 );
          xx[0] = a3;
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
          // y3 = f ( a3 );
          xx[0] = a3;
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
        // y3 = f ( a3 );
        xx[0] = a3;
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
    if (iter > end)
      break;
  }

  return y;
}

OneDimProblem::~OneDimProblem()
{
}

PSearch::PSearch(Problem *p)
    : Optimizer(p)
{
  prin = std::chrono::system_clock::now();
  nsamples = 0;
  addParameter("psearch_samples", "100", "Number of pBrent samples");
  addParameter("localSearch_method", "5", "Number of method");
}

bool PSearch::terminated()
{
  return true;
}

void PSearch::step()
{

  bestPerDim.resize(myProblem->getDimension());
  bestPoint.resize(myProblem->getDimension());
  //#pragma omp parallel for
  for (int i = 0; i < nsamples; i++)
  {
    //#pragma omp parallel for
    for (int d = 0; d < myProblem->getDimension(); d++)
    {
      //#pragma omp parallel
      //            {
      double initialX;
      double y;
      initialX = points[i][d];
      OneDimProblem onedim(myProblem, points[i], d);
      switch (method)
      {
      case 1:
      {
        y = onedim.brentSearch(initialX); // brent
        break;
      }
      case 2:
      {
        y = onedim.newtonSearch(initialX); // newton
        break;
      }
      case 3:
      {
        y = onedim.secantSearch(initialX); // secant
        break;
      }
      case 4:
      {
        Data x2;
        x2.resize(myProblem->getDimension());
        x2 = myProblem->getRandomPoint();
        y = onedim.goldenSearch(initialX,x2[d]); //golden
        break;
      }
      case 5:
      {
        Data x2;
        x2.resize(myProblem->getDimension());
        x2 = myProblem->getRandomPoint();
        y = onedim.bisectionSearch(initialX,x2[d]); //bisection
        break;
      }
      }
      //#pragma omp critical
      if (i == 0 || y < bestPerDim[d])
      {
        bestPerDim[d] = y;
        bestPoint[d] = initialX;
      }
      //            }
    }
  }
}

void PSearch::init()
{
  nsamples = params["psearch_samples"].toString().toInt();
  method = params["localSearch_method"].toString().toInt();
  // printf("mathod=%d\n", method);
  points.resize(nsamples);
  for (int i = 0; i < nsamples; i++)
    points[i] = myProblem->getRandomPoint();
}

void PSearch::done()
{
  double yy = localSearch(bestPoint);
  meta = std::chrono::system_clock::now();

  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(meta - prin);
  auto ms = milliseconds.count();

  std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
  printf("Final outcome: %10.5lf\n", yy);
}

void PSearch::Solve()
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

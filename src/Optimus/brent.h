#include <vector>
# include <cstddef>
namespace brent {

class func_base{
public:
  virtual double operator() (double) = 0;
};

class monicPoly : public func_base {
public:
  std::vector<double> coeff;
  virtual double operator() (double x);
// constructors:
  monicPoly(const size_t degree)
   : coeff(degree) {}
  monicPoly(const std::vector<double>& v)
   : coeff(v) {}
  monicPoly(const double* c, size_t degree)
   : coeff(std::vector<double>(c, c+degree)) {}
};

class Poly : public func_base {
public:
  std::vector<double> coeff;    // a vector of size nterms i.e. 1+degree
  virtual double operator() (double x);
// constructors:
  Poly(const size_t degree)
   : coeff(1+degree) {}
  Poly(const std::vector<double>& v)
   : coeff(v) {}
  Poly(const double* c, size_t degree)
   : coeff(std::vector<double>(c, 1+c+degree)) {}
};
//****************************************************************************80
//
//  Purpose:
//
//    GLOMIN seeks a global minimum of a function F(X) in an interval [A,B].
//
//  Discussion:
//
//    This function assumes that F(X) is twice continuously differentiable
//    over [A,B] and that F''(X) <= M for all X in [A,B].
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 July 2011
//
//  Author:
//
//    Original FORTRAN77 version by Richard Brent.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the interval.
//    It must be the case that A < B.
//
//    Input, double C, an initial guess for the global
//    minimizer.  If no good guess is known, C = A or B is acceptable.
//
//    Input, double M, the bound on the second derivative.
//
//    Input, double E, a positive tolerance, a bound for the
//    absolute error in the evaluation of F(X) for any X in [A,B].
//
//    Input, double T, a positive error tolerance.
//
//    Input, func_base& F, a user-supplied c++ functor whose
//    global minimum is being sought.  The input and output
//    of F() are of type double.
//
//    Output, double &X, the estimated value of the abscissa
//    for which F attains its global minimum value in [A,B].
//
//    Output, double GLOMIN, the value F(X).
//
double glomin ( double a, double b, double c, double m, double e, double t,  func_base& f, double &x );
//****************************************************************************80
//
//  Purpose:
//
//    LOCAL_MIN seeks a local minimum of a function F(X) in an interval [A,B].
//
//  Discussion:
//
//    The method used is a combination of golden section search and
//    successive parabolic interpolation.  Convergence is never much slower
//    than that for a Fibonacci search.  If F has a continuous second
//    derivative which is positive at the minimum (which is not at A or
//    B), then convergence is superlinear, and usually of the order of
//    about 1.324....
//
//    The values EPS and T define a tolerance TOL = EPS * abs ( X ) + T.
//    F is never evaluated at two points closer than TOL.
//
//    If F is a unimodal function and the computed values of F are always
//    unimodal when separated by at least SQEPS * abs ( X ) + (T/3), then
//    LOCAL_MIN approximates the abscissa of the global minimum of F on the
//    interval [A,B] with an error less than 3*SQEPS*abs(LOCAL_MIN)+T.
//
//    If F is not unimodal, then LOCAL_MIN may approximate a local, but
//    perhaps non-global, minimum to the same accuracy.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 July 2011
//
//  Author:
//
//    Original FORTRAN77 version by Richard Brent.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the interval.
//
//    Input, double T, a positive absolute error tolerance.
//
//    Input, func_base& F, a user-supplied c++ functor whose
//    local minimum is being sought.  The input and output
//    of F() are of type double.
//
//    Output, double &X, the estimated value of an abscissa
//    for which F attains a local minimum value in [A,B].
//
//    Output, double LOCAL_MIN, the value F(X).
//
double local_min ( double a, double b, double t, func_base& f,  double &x );
//****************************************************************************80
//
//  Purpose:
//
//    LOCAL_MIN_RC seeks a minimizer of a scalar function of a scalar variable.
//
//  Discussion:
//
//    This routine seeks an approximation to the point where a function
//    F attains a minimum on the interval (A,B).
//
//    The method used is a combination of golden section search and
//    successive parabolic interpolation.  Convergence is never much
//    slower than that for a Fibonacci search.  If F has a continuous
//    second derivative which is positive at the minimum (which is not
//    at A or B), then convergence is superlinear, and usually of the
//    order of about 1.324...
//
//    The routine is a revised version of the Brent local minimization
//    algorithm, using reverse communication.
//
//    It is worth stating explicitly that this routine will NOT be
//    able to detect a minimizer that occurs at either initial endpoint
//    A or B.  If this is a concern to the user, then the user must
//    either ensure that the initial interval is larger, or to check
//    the function value at the returned minimizer against the values
//    at either endpoint.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 July 2011
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//    David Kahaner, Cleve Moler, Steven Nash,
//    Numerical Methods and Software,
//    Prentice Hall, 1989,
//    ISBN: 0-13-627258-4,
//    LC: TA345.K34.
//
//  Parameters
//
//    Input/output, double &A, &B.  On input, the left and right
//    endpoints of the initial interval.  On output, the lower and upper
//    bounds for an interval containing the minimizer.  It is required
//    that A < B.
//
//    Input/output, int &STATUS, used to communicate between
//    the user and the routine.  The user only sets STATUS to zero on the first
//    call, to indicate that this is a startup call.  The routine returns STATUS
//    positive to request that the function be evaluated at ARG, or returns
//    STATUS as 0, to indicate that the iteration is complete and that
//    ARG is the estimated minimizer.
//
//    Input, double VALUE, the function value at ARG, as requested
//    by the routine on the previous call.
//
//    Output, double LOCAL_MIN_RC, the currently considered point.
//    On return with STATUS positive, the user is requested to evaluate the
//    function at this point, and return the value in VALUE.  On return with
//    STATUS zero, this is the routine's estimate for the function minimizer.
//
//  Local parameters:
//
//    C is the squared inverse of the golden ratio.
//
//    EPS is the square root of the relative machine precision.
//
double local_min_rc ( double &a, double &b, int &status, double value );
//****************************************************************************80
//
//  Purpose:
//
//    R8_EPSILON returns the R8 roundoff unit.
//
//  Discussion:
//
//    The roundoff unit is a number R which is a power of 2 with the
//    property that, to the precision of the computer's arithmetic,
//      1 < 1 + R
//    but
//      1 = ( 1 + R / 2 )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 September 2012
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, double R8_EPSILON, the R8 round-off unit.
//
double r8_epsilon ( );
//****************************************************************************80
//
//  Purpose:
//
//    R8_MAX returns the maximum of two R8's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, Y, the quantities to compare.
//
//    Output, double R8_MAX, the maximum of X and Y.
//
double r8_max ( double x, double y );
//****************************************************************************80
//
//  Purpose:
//
//    R8_SIGN returns the sign of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 October 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the number whose sign is desired.
//
//    Output, double R8_SIGN, the sign of X.
//
double r8_sign ( double x );
//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
void timestamp ( );
//****************************************************************************80
//
//  Purpose:
//
//    ZERO seeks the root of a function F(X) in an interval [A,B].
//
//  Discussion:
//
//    The interval [A,B] must be a change of sign interval for F.
//    That is, F(A) and F(B) must be of opposite signs.  Then
//    assuming that F is continuous implies the existence of at least
//    one value C between A and B for which F(C) = 0.
//
//    The location of the zero is determined to within an accuracy
//    of 6 * MACHEPS * abs ( C ) + 2 * T.
//
//    Thanks to Thomas Secretin for pointing out a transcription error in the
//    setting of the value of P, 11 February 2013.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 February 2013
//
//  Author:
//
//    Original FORTRAN77 version by Richard Brent.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the change of sign interval.
//
//    Input, double T, a positive error tolerance.
//
//    Input, func_base& F, the name of a user-supplied c++ functor
//    whose zero is being sought.  The input and output
//    of F() are of type double.
//
//    Output, double ZERO, the estimated value of a zero of
//    the function F.
//
double zero ( double a, double b, double t, func_base& f );
//****************************************************************************80
//
//  Purpose:
//
//    ZERO_RC seeks the root of a function F(X) using reverse communication.
//
//  Discussion:
//
//    The interval [A,B] must be a change of sign interval for F.
//    That is, F(A) and F(B) must be of opposite signs.  Then
//    assuming that F is continuous implies the existence of at least
//    one value C between A and B for which F(C) = 0.
//
//    The location of the zero is determined to within an accuracy
//    of 6 * MACHEPS * abs ( C ) + 2 * T.
//
//    The routine is a revised version of the Brent zero finder
//    algorithm, using reverse communication.
//
//    Thanks to Thomas Secretin for pointing out a transcription error in the
//    setting of the value of P, 11 February 2013.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 February 2013
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Richard Brent,
//    Algorithms for Minimization Without Derivatives,
//    Dover, 2002,
//    ISBN: 0-486-41998-3,
//    LC: QA402.5.B74.
//
//  Parameters:
//
//    Input, double A, B, the endpoints of the change of sign interval.
//
//    Input, double T, a positive error tolerance.
//
//    Output, double &ARG, the currently considered point.  The user
//    does not need to initialize this value.  On return with STATUS positive,
//    the user is requested to evaluate the function at ARG, and return
//    the value in VALUE.  On return with STATUS zero, ARG is the routine's
//    estimate for the function's zero.
//
//    Input/output, int &STATUS, used to communicate between
//    the user and the routine.  The user only sets STATUS to zero on the first
//    call, to indicate that this is a startup call.  The routine returns STATUS
//    positive to request that the function be evaluated at ARG, or returns
//    STATUS as 0, to indicate that the iteration is complete and that
//    ARG is the estimated zero
//
//    Input, double VALUE, the function value at ARG, as requested
//    by the routine on the previous call.
//
void zero_rc ( double a, double b, double t, double &arg, int &status,  double value );

// === simple wrapper functions
// === for convenience and/or compatibility
double glomin ( double a, double b, double c, double m, double e, double t,  double f ( double x ), double &x );
double local_min ( double a, double b, double t, double f ( double x ),  double &x );
double zero ( double a, double b, double t, double f ( double x ) );

}

#include "interval.h"
# include <QString>
Interval::Interval(double a,double b)
{
    if(b<a)
    {
        double t=b;
        b=a;
        a=t;
    }
    _left=a;
    _right=b;
}

Interval::Interval(const Interval &other)
{
    _left=other.leftValue();
    _right=other.rightValue();
}

Interval Interval::left() const
{
    double mid=(_left+_right)/2.0;
    return Interval(_left,_left+mid);
}

Interval Interval::right() const
{
    double mid=(_left+_right)/2.0;
    return Interval(_left+mid,_right);
}

QString Interval::toString() const
{
    return "["+QString::number(_left)+","+QString::number(_right)+"]";
}

double  Interval::width() const
{
    return _right-_left;
}

std::ostream& operator<<(std::ostream& out, const Interval &f1)
{
    out << f1.toString().toStdString();
    return out;
}

double Interval::leftValue() const
{
    return _left;
}

double Interval::rightValue() const
{
    return _right;
}

Interval& Interval::operator=(const Interval &other)
{
    _left=other.leftValue();
    _right=other.rightValue();
    return *this;
}

Interval::~Interval()
{

}

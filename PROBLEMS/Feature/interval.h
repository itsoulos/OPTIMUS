#ifndef INTERVAL_H
#define INTERVAL_H
# include <QString>
# include <math.h>
class Interval
{
private:
    double _left,_right;
public:
    Interval(double a=0.0,double b=0.0);
    Interval(const Interval &other);
    Interval left() const;
    Interval right() const;
    QString toString() const;
    Interval&operator=(const Interval &other);
    double leftValue() const;
    double rightValue() const;
    double width() const;
    friend std::ostream& operator<<(std::ostream& out, const Interval &f1);
    ~Interval();
};

#endif // INTERVAL_H

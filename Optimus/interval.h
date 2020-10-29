#ifndef INTERVAL_H
#define INTERVAL_H
# include <QString>
# include <math.h>
/** The base class for interval arithmetics**/
class Interval
{
private:
    double _left,_right;
public:
    /** Create a new interval using @param a and @param b as
     * the bounds of the interval **/
    Interval(double a=0.0,double b=0.0);

    /** Assign other interval to the current**/
    Interval(const Interval &other);

    /** @return the left bound of the interval in interval format **/
    Interval left() const;

    /** @return the right bound of the interval in interval format**/
    Interval right() const;

    /** @return the string representation of the interval**/
    QString toString() const;


    /** Assign overloaded operator**/
    Interval&operator=(const Interval &other);

    /** @return the left value of the interval **/
    double leftValue() const;

    /** @return the right value of the interval **/
    double rightValue() const;

    /** @return the width of the interval **/
    double width() const;

    /** Split the interval in the left bound **/
    void    divideLeft();

    /** Split the interval in the right bound **/
    void    divideRight();


    /** Overloaded operator << for intervals **/
    friend std::ostream& operator<<(std::ostream& out, const Interval &f1);

    /** Overloaded operator + for intervals **/
    friend Interval operator+(const Interval& a, const Interval& b);

    /** Overloaded operator * for intervals **/
    friend Interval operator*(double value,const Interval &other);

    /** Overloaded operator * for intervals **/
    friend Interval operator*(const Interval &other,double value);

    /** The destructor of the class */
    ~Interval();
};

#endif // INTERVAL_H

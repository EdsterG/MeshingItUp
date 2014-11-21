/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "point.h"
#include <cmath>

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

// constructors
Point::Point() { data[0]=0; data[1]=0; data[2]=0; }
Point::Point(double x, double y, double z) { data[0]=x; data[1]=y; data[2]=z; }

// sets
void Point::setX(double x){ data[0] = x; }
void Point::setY(double y){ data[1] = y; }
void Point::setZ(double z){ data[2] = z; }

// gets
double Point::getX(){ return data[0]; }
double Point::getY(){ return data[1]; }
double Point::getZ(){ return data[2]; }

// bracket operator
const double& Point::operator [] (size_t i) const { return data[i]; }

// create new point: point addition/subtraction
Point Point::operator + (const Point& v2) const { return Point(data[0] + v2.data[0], data[1] + v2.data[1], data[2] + v2.data[2]); }
Point Point::operator - (const Point& v2) const { return Point(data[0] - v2.data[0], data[1] - v2.data[1], data[2] - v2.data[2]); }

// create new point: operations with a constant
Point Point::operator * (double c) const { return Point(data[0] * c, data[1] * c, data[2] * c); }
Point Point::operator / (double c) const { return Point(data[0] / c, data[1] / c, data[2] / c); }

// create new point: unary negation
Point Point::operator - () const { return Point(-data[0], -data[1], -data[2]); }

// override point: point addition/subtraction
Point& Point::operator += (const Point& v2) { data[0] += v2.data[0]; data[1] += v2.data[1]; data[2] += v2.data[2]; return *this; }
Point& Point::operator -= (const Point& v2) { data[0] -= v2.data[0]; data[1] -= v2.data[1]; data[2] -= v2.data[2]; return *this; }

// override point: operations with a constant
Point& Point::operator *= (double c) { data[0] *= c; data[1] *= c; data[2] *= c; return *this; }

// cross and dot product
Point Point::cross(const Point& p2) const { return Point(data[1] * p2.data[2] - data[2] * p2.data[1], data[2] * p2.data[0] - data[0] * p2.data[2], data[0]  * p2.data[1] - data[1] * p2.data[0]); }
double Point::norm() const { return std::sqrt( data[0]*data[0] + data[1]*data[1] + data[2]*data[2] ); }
double Point::dot(const Point& p2) const { return data[0]* p2.data[0] + data[1]* p2.data[1] + data[2]* p2.data[2]; }

// io stuff
void Point::stream_to(std::ostream& os) const {os<<"<"<<data[0]<<","<<data[1]<<","<<data[2]<<">";}

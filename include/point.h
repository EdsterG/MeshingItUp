/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#ifndef POINT_H
#define POINT_H

#include "point.h"
#include <iostream>

// stl :: String Theory Labs (Taking over a universe near you)
namespace stl
{

class Point {
 private:
  double data[3];

 public:
  Point();
  Point(double x, double y, double z );

  // sets
  void setX(double x);
  void setY(double y);
  void setZ(double z);

  // gets
  double getX();
  double getY();
  double getZ();

  // bracket operator
  const double& operator [] (size_t i) const;

  // create new point: point addition/subtraction
  Point operator + (const Point& v2) const;
  Point operator - (const Point& v2) const;

  // create new point: operations with a constant
  Point operator * (double c) const;
  Point operator / (double c) const;

  // create new point: unary negation
  Point operator - () const;

  // override point: point addition/subtraction
  Point& operator += (const Point& v2);
  Point& operator -= (const Point& v2);

  // override point: operations with a constant
  Point& operator *= (double c);

  // cross product
  Point cross(const Point& p2) const;
  double norm() const;
  double dot(const Point& p2) const;

  // io stuff
  void stream_to(std::ostream& os) const;
};

static inline std::ostream& operator << (std::ostream& os, const Point& p) {p.stream_to(os); return os;}

}

#endif

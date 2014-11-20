/* 
Authors: 
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include <iostream>

// stl :: String Theory Labs (Taking over a universe near you) 
namespace stl
{

class Vertex {
 private:
  Point p;
  Point n;
  double u,v;

 public:
  Vertex(const Point& p, const Point& n);
  Vertex(const Point& p, const Point& n, double u, double v);
  const Point& pos();
  const Point& normal();
  const double getU();
  const double getV();

  // io stuff
  void stream_to(std::ostream& os) const;
};

static inline std::ostream& operator << (std::ostream& os, const Vertex& v) {v.stream_to(os); return os;}

}

#endif

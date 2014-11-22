/* 
Authors: 
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include <iostream>

// struct Vertex
// {
//     float position[3];
//     float normal[3];
//     float texCoord[2];
//     float tangent[4];
//     float bitangent[3];
// };

// stl :: String Theory Labs (Taking over a universe near you) 
namespace stl
{

class Vertex {
 private:
  Point p;
  bool normalSet;
  Point n;
  double u,v;

 public:
  Vertex(const Point& p);
  Vertex(const Point& p, const Point& n);
  Vertex(const Point& p, const Point& n, double u, double v);
  const Point& pos();
  const Point& normal();
  const bool hasNormal();
  const double getU();
  const double getV();

  // io stuff
  void stream_to(std::ostream& os) const;
};

static inline std::ostream& operator << (std::ostream& os, const Vertex& v) {v.stream_to(os); return os;}

}

#endif

#ifndef BEZPATCH_H
#define BEZPATCH_H

#include "point.h"
#include "vertex.h"
#include <vector>
#include <iostream>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

// point / derivative
typedef struct {
  Point p;
  Point d;
} PointDeriv ;

class BezPatch {
 private:
  Point controls[4][4];
  double param;

  std::vector<std::vector<double> > faces;
  std::vector<Vertex> verticies;

  void uniformSampling();
  void adaptiveSampling();
  void adaptiveSplit(int v1, int v2, int v3);
  void adaptiveSplit(int* indices);
  bool splitEdge(Vertex& v1, Vertex& v2, Vertex& onCurve);

 public:
  BezPatch(const Point (&controls)[4][4], double param, bool adaptive);

  void draw();

  public:

    BezPatch();

    PointDeriv bezcurveinterp(Point (&curve)[4], double u);
    Vertex bezpatchinterp(Point (&patch)[4][4], double u, double v);
};

}

#endif

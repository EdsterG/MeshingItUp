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
  Point patch[4][4];
  double param;

  std::vector<std::vector<int> > faces;
  std::vector<Vertex> vertices;

  int index;

  void uniformSampling();
  void adaptiveSampling();
  void adaptiveSplit(int index1, int index2, int index3);
  bool splitEdge(Vertex& v1, Vertex& v2, Vertex& v_on_curve);

 public:
  BezPatch(Point (&patch)[4][4], double param, bool adaptive);
  PointDeriv bezCurveInterp(Point (&curve)[4], double u);
  Vertex bezPatchInterp(double u, double v);
  Point secondDeriv(Point (&p)[4], double u);
  void setIndex(int index);
  int getIndex();
  void draw();

};

}

#endif

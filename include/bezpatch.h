#ifndef BEZPATCH_H
#define BEZPATCH_H

#include "point.h"
#include <iostream>
#include "vertex.h"
#include "bezpatch.h"

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

    // PointDeriv PN;

  public:

    BezPatch();

    PointDeriv bezcurveinterp(Point (&curve)[4], double u);
    Vertex bezpatchinterp(Point (&patch)[4][4], double u, double v);
};

}
#endif

#ifndef BEZPATCH_H
#define BEZPATCH_H

#include "point.h"
#include <iostream>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

class BezPatch {
 private:

  Point control[4][4];

  // point / derivative
  struct PD{
    Point point;
    Point derivative;
  };

  // point / normal
  struct PN{
    Point point;
    Point normal;
  };

  PD bezcurveinterp(Point (&curve)[4], double u);
  PN bezpatchinterp(Point (&patch)[4][4], double u, double v);

 public:
  BezPatch();

};

}

#endif

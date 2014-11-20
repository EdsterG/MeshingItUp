/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "bezpatch.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;



PDN BezPatch::bezcurveinterp(Point (&curve)[4], double u){
  Point A, B, C, D, E;
  PD   

  // split each of the three segments
  // to form two new ones AB and BC
  A = curve[0] * (1.0-u) + curve[1] * u;
  B = curve[1] * (1.0-u) + curve[2] * u;
  C = curve[2] * (1.0-u) + curve[3] * u;

  // split AB and BC to form a new segment DE
  D = A * (1.0-u) + B * u;
  E = B * (1.0-u) + C * u;

  // this is the point on the curve
  P = D * (1.0-u) + E * u

  // compute derivative
  dPdu = 3 * (E - D)

  return PD




};
PDN BezPatch::bezpatchinterp(Point& patch[4][4], double u, double v);

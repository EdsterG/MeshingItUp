/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "bezpatch.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;


PointDeriv BezPatch::bezcurveinterp(Point (&curve)[4], double u){
  Point A, B, C, D, E;
  PointDeriv PD;

  // split each of the three segments
  // to form two new ones AB and BC
  A = curve[0] * (1.0-u) + curve[1] * u;
  B = curve[1] * (1.0-u) + curve[2] * u;
  C = curve[2] * (1.0-u) + curve[3] * u;

  // split AB and BC to form a new segment DE
  D = A * (1.0-u) + B * u;
  E = B * (1.0-u) + C * u;

  // this is the point on the curve
  PD.p = D * (1.0-u) + E * u;

  // compute derivative
  PD.d = (E - D) * 3;

  return PD;

};

Vertex BezPatch::bezpatchinterp(Point (&patch)[4][4], double u, double v){

  Point n;
  PointDeriv uPD, vPD;
  Point vcurve[4];
  Point ucurve[4];

  // build control points
  for(int i = 0; i < 4; i++){
    // build control points for a Bezier curve in v
    vcurve[0] = bezcurveinterp(patch[0], u).p;

    // build control points for a Bezier curve in v
    Point tempCurve[4];
    tempCurve[0] = patch[0][i];
    tempCurve[1] = patch[1][i];
    tempCurve[2] = patch[2][i];
    tempCurve[3] = patch[3][i];
    ucurve[i] = bezcurveinterp(tempCurve, v).p;
  }
  // evaluate surface and derivative for u and v
  vPD = bezcurveinterp(vcurve, v);
  uPD = bezcurveinterp(ucurve, u);
  // take cross product of partials to find normal
  n = (uPD.d).cross(vPD.d);
  n = n.norm();

  return Vertex(uPD.p, n);
};

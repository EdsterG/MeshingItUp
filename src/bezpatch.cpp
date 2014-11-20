/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "bezpatch.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

BezPatch::BezPatch(Point (&patch_)[4][4], double param_, bool adaptive) : patch(patch_), param(param_) {
if (adaptive)
  adaptiveSampling();
else
  uniformSampling();
}

void BezPatch::uniformSampling() {
  for (double u = 0; u <= 1; u+=param) {
    for (double v = 0; v <= 1; v+=param) {
      // verticies.push_back(bezpatchinterp(patch,u,v));
    }
  }

  int num_steps = (int)(1/param);
  int indices[4];
  for (int i = 0; i < num_steps-1; i+=1) {
    for (int j = 0; j < num_steps-1; j+=1) {
      indices[1] = i*num_steps+j;
      indices[2] = indices[1]+1;
      indices[3] = (i+1)*num_steps+j;
      indices[4] = indices[3]+1;

      faces.push_back(std::vector<double>(indices,indices+2));
      faces.push_back(std::vector<double>(indices+1,indices+3));
    }
  }

}
void BezPatch::adaptiveSampling() {
  // verticies.push_back(bezpatchinterp(patch,0,0));
  // verticies.push_back(bezpatchinterp(patch,0,1));
  // verticies.push_back(bezpatchinterp(patch,1,0));
  // verticies.push_back(bezpatchinterp(patch,1,1));

  int indicies1[3] = {0,1,2};
  adaptiveSplit(indicies1);
  int indicies2[3] = {1,2,3};
  adaptiveSplit(indicies2);
  // adaptiveSplit(0,1,2);
  // adaptiveSplit(1,2,3);
}

bool BezPatch::splitEdge(Vertex& v1, Vertex& v2, Vertex& onCurve) {
  return (((v1.pos()+v2.pos())/2.0-onCurve.pos()).norm()>param);
}

void BezPatch::adaptiveSplit(int* indices) {
  Vertex v1 = verticies[indices[0]];
  Vertex v2 = verticies[indices[1]];
  Vertex v3 = verticies[indices[2]];
  // Vertex p1 = bezPatchInterp(patch, (v1.getU() + v2.getU())/2, (v1.getV() + v2.getV())/2);
  // Vertex p2 = bezPatchInterp(patch, (v2.getU() + v3.getU())/2, (v2.getV() + v3.getV())/2);
  // Vertex p3 = bezPatchInterp(patch, (v3.getU() + v1.getU())/2, (v3.getV() + v1.getV())/2);

  // bool e1 = splitEdge(v1,v2,p1);
  // bool e2 = splitEdge(v2,v3,p2);
  // bool e3 = splitEdge(v3,v1,p3);

  // if (e1+e2+e3 == 0) {
  //   //face.push_back(std::vector<double>(indices,indices+2))
  // }
  // else if (e1+e2+e3 == 1){
  //     int indicies1[3] = {0,1,2};
  //     adaptiveSplit(indicies1);
  //     int indicies2[3] = {1,2,3};
  //     adaptiveSplit(indicies2);
  // }
  // else if (e1+e2+e3 == 2){
  // } else {
  //   int indicies1[3] = {0,1,2};
  //   int indicies2[3] = {0,1,2};
  //   int indicies3[3] = {0,1,2};
  //   int indicies4[3] = {0,1,2};
  // }
}

void BezPatch::adaptiveSplit(int v1, int v2, int v3) {
  // Vertex p1 = bezPatchInterp(patch, (v1.getU() + v2.getU())/2, (v1.getV() + v2.getV())/2);
  // Vertex p2 = bezPatchInterp(patch, (v2.getU() + v3.getU())/2, (v2.getV() + v3.getV())/2);
  // Vertex p3 = bezPatchInterp(patch, (v3.getU() + v1.getU())/2, (v3.getV() + v1.getV())/2);

  // bool e1 = splitEdge(v1,v2,p1);
  // bool e2 = splitEdge(v2,v3,p2);
  // bool e3 = splitEdge(v3,v1,p3);

  // if (!e1 && !e2 && !e3) {
  //   //face.push_back(std::vector<double>(indices,indices+2))
  // }
  // else if (!e1 && !e2 && !e3) {

  // }
}

void BezPatch::draw() {

}

PointDeriv BezPatch::bezCurveInterp(Point (&curve)[4], double u){
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

Vertex BezPatch::bezPatchInterp(Point (&patch)[4][4], double u, double v){

  Point n;
  PointDeriv uPD, vPD;
  Point vcurve[4];
  Point ucurve[4];

  // build control points
  for(int i = 0; i < 4; i++){
    // build control points for a Bezier curve in v
    vcurve[0] = bezCurveInterp(patch[0], u).p;

    // build control points for a Bezier curve in v
    Point tempCurve[4];
    tempCurve[0] = patch[0][i];
    tempCurve[1] = patch[1][i];
    tempCurve[2] = patch[2][i];
    tempCurve[3] = patch[3][i];
    ucurve[i] = bezCurveInterp(tempCurve, v).p;
  }
  // evaluate surface and derivative for u and v
  vPD = bezCurveInterp(vcurve, v);
  uPD = bezCurveInterp(ucurve, u);
  // take cross product of partials to find normal
  n = (uPD.d).cross(vPD.d);
  n = n.norm();

  return Vertex(uPD.p, n);
};

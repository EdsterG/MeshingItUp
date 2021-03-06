/*
Authors:
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "bezpatch.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

BezPatch::BezPatch(Point (&patch_)[4][4], double param_, bool adaptive) : param(param_){
for (int i=0; i<4; i++) {
  for (int j=0; j<4; j++) {
    patch[i][j]=patch_[i][j];
  }
}

if (adaptive)
  adaptiveSampling();
else
  uniformSampling();
}

void BezPatch::uniformSampling() {
  int num_steps = (int)(1/param)+1;

  for (int u = 0; u*param <= 1; u++) {
    for (int v = 0; v*param <= 1; v++) {
      vertices.push_back(bezPatchInterp(u*param,v*param));
    }
  }

  int indices[4];
  for (int i = 0; i < num_steps-1; i++) {
    for (int j = 0; j < num_steps-1; j++) {
      indices[0] = i*num_steps+j;
      indices[1] = indices[0]+1;
      indices[2] = (i+1)*num_steps+j;
      indices[3] = indices[2]+1;

      faces.push_back(std::vector<int>(indices,indices+3));
      faces.push_back(std::vector<int>(indices+1,indices+4));
    }
  }
}

void BezPatch::adaptiveSampling() {
  vertices.push_back(bezPatchInterp(0,0));
  vertices.push_back(bezPatchInterp(0,1));
  vertices.push_back(bezPatchInterp(1,0));
  vertices.push_back(bezPatchInterp(1,1));

  adaptiveSplit(2,0,1);
  adaptiveSplit(1,3,2);
}

bool BezPatch::splitEdge(Vertex& v1, Vertex& v2, Vertex& v_on_curve) {
  return (((v1.pos()+v2.pos())/2.0-v_on_curve.pos()).norm()>param);
}

void BezPatch::adaptiveSplit(int index1, int index2, int index3) {
  Vertex v1 = vertices[index1];
  Vertex v2 = vertices[index2];
  Vertex v3 = vertices[index3];
  Vertex p1 = bezPatchInterp((v1.getU() + v2.getU())/2, (v1.getV() + v2.getV())/2);
  Vertex p2 = bezPatchInterp((v2.getU() + v3.getU())/2, (v2.getV() + v3.getV())/2);
  Vertex p3 = bezPatchInterp((v3.getU() + v1.getU())/2, (v3.getV() + v1.getV())/2);

  bool e1 = splitEdge(v1,v2,p1);
  bool e2 = splitEdge(v2,v3,p2);
  bool e3 = splitEdge(v3,v1,p3);

  if (e1+e2+e3 == 0) {
    int indices[3] = {index1,index2,index3};
    faces.push_back(std::vector<int>(indices,indices+3));
  }
  else if (e1+e2+e3 == 1){
    if (e1) {
      vertices.push_back(p1);
      int p1_index = vertices.size()-1;
      adaptiveSplit(index1,p1_index,index3);
      adaptiveSplit(index3,p1_index,index2);
    }
    else if (e2) {
      vertices.push_back(p2);
      int p2_index = vertices.size()-1;
      adaptiveSplit(index2,p2_index,index1);
      adaptiveSplit(index1,p2_index,index3);
    }
    else {
      vertices.push_back(p3);
      int p3_index = vertices.size()-1;
      adaptiveSplit(index3,p3_index,index2);
      adaptiveSplit(index2,p3_index,index1);
    }
  }
  else if (e1+e2+e3 == 2){
    if (e1+e2 == 2) {
      vertices.push_back(p1);
      vertices.push_back(p2);
      int p2_index = vertices.size()-1;
      int p1_index = p2_index-1;
      adaptiveSplit(index1,p1_index,index3);
      adaptiveSplit(p1_index,p2_index,index3);
      adaptiveSplit(p1_index,index2,p2_index);
    }
    else if (e2+e3 == 2) {
      vertices.push_back(p2);
      vertices.push_back(p3);
      int p3_index = vertices.size()-1;
      int p2_index = p3_index-1;
      adaptiveSplit(index2,p2_index,index1);
      adaptiveSplit(p2_index,p3_index,index1);
      adaptiveSplit(p2_index,index3,p3_index);
    }
    else {
      vertices.push_back(p1);
      vertices.push_back(p3);
      int p3_index = vertices.size()-1;
      int p1_index = p3_index-1;
      adaptiveSplit(index3,p3_index,index2);
      adaptiveSplit(p3_index,p1_index,index2);
      adaptiveSplit(p3_index,index1,p1_index);
    }
  } else {
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    int p3_index = vertices.size()-1;
    int p2_index = p3_index-1;
    int p1_index = p3_index-2;

    adaptiveSplit(index1,p1_index,p3_index);
    adaptiveSplit(p1_index,index2,p2_index);
    adaptiveSplit(p3_index,p2_index,index3);
    adaptiveSplit(p1_index,p2_index,p3_index);
  }
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

Vertex BezPatch::bezPatchInterp(double u, double v){
  Point n;
  PointDeriv uPD, vPD;
  Point vcurve[4];
  Point ucurve[4];

  // build control points
  for(int i = 0; i < 4; i++){
    // build control points for a Bezier curve in v
    vcurve[i] = bezCurveInterp(patch[i], u).p;

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
  n = n/n.norm();

  return Vertex(uPD.p, n, u, v);
};

Point BezPatch::secondDeriv(Point (&p)[4], double u) {
  return ((p[2]-p[1]*2+p[0])*(1-u) + (p[3]-p[2]*2+p[1])*u)*6;
}

void BezPatch::setIndex(int index_) { index=index_;}
int BezPatch::getIndex() {return index;}

std::vector<Vertex>& BezPatch::getVertices() {return vertices;}
std::vector<std::vector<int> >& BezPatch::getFaces() {return faces;}

void BezPatch::draw() {
  for (int i=0; i < faces.size(); i++) {
    glBegin(GL_TRIANGLES);
    for (int k=0; k < faces[i].size(); k++){
      int index = faces[i][k];
      Vertex v = vertices[index];
      if (v.hasNormal()) glNormal3f(v.normal()[0], v.normal()[1], v.normal()[2]);
      glVertex3f(v.pos()[0], v.pos()[1], v.pos()[2]);
    }
    glEnd();
  }
}

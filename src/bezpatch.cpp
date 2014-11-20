#include "bezpatch.h"

using namespace stl;

BezPatch::BezPatch(const Point (&controls_)[4][4], double param_, bool adaptive) : controls(controls_), param(param_) {
if (adaptive)
  adaptiveSampling();
else
  uniformSampling();
}

void BezPatch::uniformSampling() {
  for (double u = 0; u <= 1; u+=param) {
    for (double v = 0; v <= 1; v+=param) {
      // verticies.push_back(bezpatchinterp(controls,u,v));
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
  // verticies.push_back(bezpatchinterp(controls,0,0));
  // verticies.push_back(bezpatchinterp(controls,0,1));
  // verticies.push_back(bezpatchinterp(controls,1,0));
  // verticies.push_back(bezpatchinterp(controls,1,1));

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
  // Vertex p1 = bezPatchInterp(controls, (v1.getU() + v2.getU())/2, (v1.getV() + v2.getV())/2);
  // Vertex p2 = bezPatchInterp(controls, (v2.getU() + v3.getU())/2, (v2.getV() + v3.getV())/2);
  // Vertex p3 = bezPatchInterp(controls, (v3.getU() + v1.getU())/2, (v3.getV() + v1.getV())/2);

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
  // Vertex p1 = bezPatchInterp(controls, (v1.getU() + v2.getU())/2, (v1.getV() + v2.getV())/2);
  // Vertex p2 = bezPatchInterp(controls, (v2.getU() + v3.getU())/2, (v2.getV() + v3.getV())/2);
  // Vertex p3 = bezPatchInterp(controls, (v3.getU() + v1.getU())/2, (v3.getV() + v1.getV())/2);

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
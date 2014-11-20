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
      //verticies.push_back(bezpatchinterp(controls,u,v));
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

}

void BezPatch::draw() {

}
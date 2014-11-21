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

#include "objmodel.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

ObjModel::ObjModel(std::vector<Point>* v, std::vector<std::vector<int> >* f1, std::vector<Point>* n, std::vector<std::vector<int> >* f2) : vertices(v), facesToVerts(f1), normals(n), facesToNormals(f2) {}

void ObjModel::setIndex(int index_) { index=index_;}
int ObjModel::getIndex() {return index;}

void ObjModel::draw() {
  for (int i=0; i < facesToVerts->size(); i++) {
    glBegin(GL_TRIANGLES);
    for (int k=0; k < facesToVerts->at(i).size(); k++){
      int index = facesToNormals->at(i)[k];
      glNormal3f((*normals)[index][0], (*normals)[index][1], (*normals)[index][2]);
      index = facesToVerts->at(i)[k];
      glVertex3f((*vertices)[index][0], (*vertices)[index][1], (*vertices)[index][2]);
    }
    glEnd();
  }
}

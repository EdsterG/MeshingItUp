#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "vertex.h"
#include <vector>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

class ObjModel {
 private:
  std::vector<Point>* vertices;
  std::vector<std::vector<int> >* facesToVerts;
  std::vector<Point>* normals;
  std::vector<std::vector<int> >* facesToNormals;

  int index;

 public:
  ObjModel(std::vector<Point>* v, std::vector<std::vector<int> >* f1, std::vector<Point>* n, std::vector<std::vector<int> >* f2);
  void setIndex(int index);
  int getIndex();
  void draw();

};

}

#endif

#ifndef PARSER_H
#define PARSER_H

#include <iostream>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

// enum class Shading {
//   FLAT = 0;
//   SMOOTH = 1;
// };

// enum class Mode {
//   FILLED = 0;
//   WIREFRAME = 1;
//   HIDDEN_LINE = 2;
// };

// enum class Parameters {
//   GAUSSIAN_CURVATURE = 0;
// };

// enum class Tesselation {
//   UNIFORM=0;
//   ADAPTIVE=0;
// };

// struct test {
//   std::string fileName;
//   double paramater;
//   bool adaptive_tess;
// };

class Parser {
 public:
  void parse(int* argc, char** argv);
  void loadBez(std::string fileName);
  // void loadObj();
  // void loadMat();
};

}

#endif

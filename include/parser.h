#ifndef PARSER_H
#define PARSER_H

#include <iostream>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

class Parser {
 public:
  void parse(int* argc, char** argv);
  void loadBez(std::string fileName, double param, bool adaptive);
  void loadOBJ(std::string fileName);
  // void loadMat();
};

}

#endif

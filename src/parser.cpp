#include "parser.h"

#include <fstream>
#include <sstream>
#include <vector>
#include "point.h"
#include "bezpatch.h"

using namespace stl;

// struct Vertex
// {
//     float position[3];
//     float texCoord[2];
//     float normal[3];
//     float tangent[4];
//     float bitangent[3];
// };

void Parser::parse(int* argc, char** argv) {
  bool adaptive = false;
  std::string fileName = "";
  double param = 0.1;

  std::string arg;
  for (int i=1; i < *argc; i++) {
    arg = argv[i];

    if (i==1) {
      fileName = arg;
    }
    else if (i==2) {
      param = std::atof(argv[i]);
    }
    else if (i==3 && arg.compare("-a")==0) {
      adaptive = true;
    }
  }

  loadBez(fileName, param, adaptive);


}

void Parser::loadBez(std::string filename, double param, bool adaptive) {
  int numPatches;
  std::vector<BezPatch> patches;

  std::ifstream scnFile(filename.c_str());
  if (scnFile.is_open()) {
    std::string line;
    int patchRow = 0;
    while (std::getline(scnFile,line)) {
      char delim = ' ';
      std::replace(line.begin(),line.end(),'\t',delim); //Replace tabs with delimiter
      std::stringstream lineStream(line);

      std::string item;
      std::vector< std::string > items;
      while(std::getline(lineStream, item, delim)) {
        if (item.size() < 1){
          continue;
        }
        else if (item[0] == '#'){
          break;
        }
        else {
          items.push_back(item);
        }
      }

      if (items.size() < 1) {
        continue;
      }
      else if (items.size() == 1) {
        numPatches = std::atoi(items[0].c_str());
      }
      else if (items.size() == 12) {
        Point tempPoint;
        Point tempPatch[4][4];
        std::cout << "Storing: " << line << std::endl;
        for (int pointNum = 0; pointNum < 4; pointNum++){
          for(int i = 0; i < 3; i++){
            tempPoint[i] = std::atof(items[i+pointNum].c_str());
          }
          tempPatch[patchRow][pointNum] = tempPoint;
          patches.push_back(BezPatch(tempPatch, param, adaptive));
        }
        patchRow++;
        if (patchRow==4) {
          std::cout << "Patch created!" << std::endl;
          patchRow = 0;
        }
      }
      else {
        std::cerr << "Unknown line: " << line << std::endl;
        continue;
      }
    }
    scnFile.close();

    // if (numPatches!=patches.size()) {
    //   std::cerr << "Incorrect number of patches." << std::endl;
    // }
    //return patches;

  }
  else std::cout << "Unable to open file" << std::endl;
}

// void Parser::loadOBJ(std::string filename, std::vector<Primitive*>& faces) {
//   std::string name = "obj";
//   std::vector<Point> verticies;
//   std::vector<Normal> normals;
//   //std::vector<Texture> uvs;

//   std::ifstream scnFile(filename.c_str());
//   if (scnFile.is_open()) {
//     std::string line;
//     while (std::getline(scnFile,line)) {
//       char delim = ' ';
//       std::replace(line.begin(),line.end(),'\t',delim); //Replace tabs with delimiter
//       std::stringstream lineStream(line);

//       std::string item;
//       std::vector< std::string > items;
//       while(std::getline(lineStream, item, delim)) {
//         if (item.size() < 1){
//           continue;
//         }
//         else if (item[0] == '#'){
//           break;
//         }
//         else {
//           items.push_back(item);
//         }
//       }

//       if (items.size() < 1) {
//         continue;
//       }
//       else if (items[0].compare("o") == 0) {
//         name = items[1];
//       }
//       else if (items[0].compare("g") == 0) {
//         std::cout << "Group name: " << items[1] << std::endl;
//       }
//       else if (items[0].compare("v") == 0) {
//         //Parse vector
//         double x = std::atof(items[1].c_str());
//         double y = std::atof(items[2].c_str());
//         double z;
//         if (items.size()>3) {
//           z = std::atof(items[3].c_str());
//         }
//         else {
//           z = 0.0;
//         }
//         verticies.push_back(Point(x,y,z));
//         //std::cout << "Point: " << x << y << z << std::endl;
//       }
//       else if (items[0].compare("vt") == 0) {
//         double u = std::atof(items[1].c_str());
//         double v = std::atof(items[2].c_str());
//         std::cout << "Texture: " << u << v << std::endl;
//       }
//       else if (items[0].compare("vn") == 0) {
//         double x = std::atof(items[1].c_str());
//         double y = std::atof(items[2].c_str());
//         double z = std::atof(items[3].c_str());
//         normals.push_back(Normal(x,y,z));
//         //std::cout << "Normal: " << x << y << z << std::endl;
//       }
//       else if (items[0].compare("f") == 0) {
//         char delim = '/';
//         std::string index;
//         int vertexInd[3];
//         int countV = 0;
//         int textureInd[3];
//         int countT = 0;
//         int normalInd[3];
//         int countN = 0;
//         for (size_t i=1; i<items.size(); i++){
//           std::stringstream faceStream(items[i]);
//           if (std::getline(faceStream, index, delim)){
//             vertexInd[countV++] = std::atoi(index.c_str())-1;
//           }
//           if (std::getline(faceStream, index, delim) && index.size()>0){
//             textureInd[countT++] = std::atoi(index.c_str())-1;
//           }
//           if (std::getline(faceStream, index, delim) && index.size()>0){
//             normalInd[countN++] = std::atoi(index.c_str())-1;
//           }
//         }

//         Triangle* face;
//         if (countV == 3) {
//           if (countN == 3) {
//             face = new Triangle(verticies[vertexInd[0]],verticies[vertexInd[1]],verticies[vertexInd[2]],normals[normalInd[0]],normals[normalInd[1]],normals[normalInd[2]]);
//           } else {
//             face = new Triangle(verticies[vertexInd[0]],verticies[vertexInd[1]],verticies[vertexInd[2]]);
//           }
//         }
//         faces.push_back(new GeoPrimitive(face,materials.back(),""));
//       }
//       else if (items[0].compare("mtllib") == 0) {
//         std::cout << "MTL file: " << items[1] << std::endl;
//       }
//       else if (items[0].compare("usemtl") == 0) {
//         std::cout << "Material: " << items[1] << std::endl;
//       }
//       else {
//         std::cerr << "Unknown type: " << items[0] << std::endl;
//         continue;
//       }
//     }
//     scnFile.close();
//   }
//   else std::cout << "Unable to open file" << std::endl;
// }

// void Parser::loadMTL(std::string filename) {
//   //TODO
// }

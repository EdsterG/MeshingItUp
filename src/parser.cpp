#include "parser.h"

#include <fstream>
#include <sstream>
#include <vector>
#include "assert.h"
#include "point.h"
#include "bezpatch.h"
#include "objmodel.h"

using namespace stl;

extern std::vector<std::vector<BezPatch> > bezObjs;
extern std::vector<ObjModel> objModels;
extern std::vector<std::vector<double> > obj_centers;
extern std::vector<std::vector<double> > trans;
extern std::vector<std::vector<double> > rot;

void Parser::parse(int* argc, char** argv) {
  bool adaptive = false;
  std::string fileName = "";
  std::string ext = "";
  double param = 0.1;

  std::string arg;
  for (int i=1; i < *argc; i++) {
    arg = argv[i];

    if (i==1) {
      fileName = arg;

      int dotIndex = fileName.find_last_of(".");
      ext = fileName.substr(dotIndex+1,fileName.size());
      
    }
    else if (i==2) {
      param = std::atof(argv[i]);
    }
    else if (i==3 && arg.compare("-a")==0) {
      adaptive = true;
    }
  }

  if (ext.compare("bez")==0) {
    loadBez(fileName,param,adaptive);
  }
  else if (ext.compare("obj")==0) {
    loadOBJ(fileName);
  }
  

}

void Parser::loadBez(std::string filename, double param, bool adaptive) {
  int numPatches;
  std::vector<BezPatch> patches;
  Point tempPatch[4][4];

  std::ifstream scnFile(filename.c_str());
  if (scnFile.is_open()) {
    std::string line;
    int patchRow = 0;
    while (std::getline(scnFile,line)) {
      char delim = ' ';
      std::replace(line.begin(),line.end(),'\t',delim); //Replace tabs with delimiter
      std::replace(line.begin(),line.end(),'\r',delim); //Replace carriage_return with delim
      std::stringstream lineStream(line);

      std::string item;
      std::vector< std::string > items;
      items.clear();
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
        // std::cout << "Storing: " << line << std::endl;
        for (int pointNum = 0; pointNum < 4; pointNum++){
          double x = std::atof(items[pointNum*3].c_str());
          double y = std::atof(items[pointNum*3+1].c_str());
          double z = std::atof(items[pointNum*3+2].c_str());
          tempPatch[patchRow][pointNum] = Point(x,y,z);
        }
        patchRow++;
        if (patchRow==4) {
          patches.push_back(BezPatch(tempPatch, param, adaptive));
          // std::cout << "Patch created!" << std::endl;
          patchRow = 0;
        }
      }
      else {
        std::cerr << "Unknown line: " << line << std::endl;
        continue;
      }
    }
    scnFile.close();

    if (numPatches!=patches.size()) {
      std::cerr << "Incorrect number of patches." << std::endl;
    }

    // double center[3] = {(maxX-minX)/2,(maxY-minY)/2,(maxZ-minZ)/2};
    // obj_centers.push_back(std::vector<double>(center,center+3));
    double translation[3] = {0,0,-7};
    trans.push_back(std::vector<double>(translation,translation+3));
    double rotation[3] = {0,0,0};
    rot.push_back(std::vector<double>(rotation,rotation+3));
    bezObjs.push_back(patches);
  }
  else std::cout << "Unable to open file" << std::endl;
}

void Parser::loadOBJ(std::string filename) {
  std::string name = "obj";
  std::vector<Point>* points = new std::vector<Point>(); // Store the OBJ points
  std::vector<Point>* vertNormals = new std::vector<Point>(); // Store the OBJ normals
  //std::vector<Texture> uvs; // Store the OBJ texture coordinates

  std::vector<std::vector<int> >* facesToPoints = new std::vector<std::vector<int> >(); // Maps face to points
  std::vector<std::vector<int> >* facesToNormals = new std::vector<std::vector<int> >(); // Maps face to normals

  // The following two lines are used to generate normals when the OBJ file is missing normals
  std::vector<Point>* faceNormals = new std::vector<Point>();
  std::vector<std::vector<int> >* pointsToFaceNormals = new std::vector<std::vector<int> >();

  std::ifstream scnFile(filename.c_str());
  if (scnFile.is_open()) {
    std::string line;
    while (std::getline(scnFile,line)) {
      char delim = ' ';
      std::replace(line.begin(),line.end(),'\t',delim); //Replace tabs with delimiter
      std::replace(line.begin(),line.end(),'\r',delim); //Replace carriage_return with delim
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
      else if (items[0].compare("o") == 0) {
        name = items[1];
      }
      else if (items[0].compare("g") == 0) {
        //std::cout << "Group name: " << items[1] << std::endl;
      }
      else if (items[0].compare("v") == 0) {
        //Parse vector
        double x = std::atof(items[1].c_str());
        double y = std::atof(items[2].c_str());
        double z;
        if (items.size()>3) {
          z = std::atof(items[3].c_str());
        }
        else {
          z = 0.0;
        }
        points->push_back(Point(x,y,z));
        pointsToFaceNormals->push_back(std::vector<int>()); // Initialize with empty vector
        //std::cout << "Point: " << x << y << z << std::endl;
      }
      else if (items[0].compare("vt") == 0) {
        double u = std::atof(items[1].c_str());
        double v = std::atof(items[2].c_str());
        //std::cout << "Texture: " << u << v << std::endl;
      }
      else if (items[0].compare("vn") == 0) {
        double x = std::atof(items[1].c_str());
        double y = std::atof(items[2].c_str());
        double z = std::atof(items[3].c_str());
        Point temp = Point(x,y,z);
        vertNormals->push_back(temp/temp.norm());
        //std::cout << "Normal: " << x << y << z << std::endl;
      }
      else if (items[0].compare("f") == 0) {
        char delim = '/';
        std::string index;
        int vertexInd[3];
        int countV = 0;
        int textureInd[3];
        int countT = 0;
        int normalInd[3];
        int countN = 0;
        for (size_t i=1; i<items.size(); i++){
          std::stringstream faceStream(items[i]);
          if (std::getline(faceStream, index, delim)){
            vertexInd[countV++] = std::atoi(index.c_str())-1;
          }
          if (std::getline(faceStream, index, delim) && index.size()>0){
            textureInd[countT++] = std::atoi(index.c_str())-1;
          }
          if (std::getline(faceStream, index, delim) && index.size()>0){
            normalInd[countN++] = std::atoi(index.c_str())-1;
          }
        }

        if (countN==3) {
          facesToNormals->push_back(std::vector<int>(normalInd,normalInd+3));
        }

        if (countV==3){
          facesToPoints->push_back(std::vector<int>(vertexInd,vertexInd+3));

          // Generate face normals incase the OBJ file is missing normals
          if (vertNormals->size()==0) {
            Point e1 = points->at(vertexInd[1])-points->at(vertexInd[0]);
            Point e2 = points->at(vertexInd[2])-points->at(vertexInd[0]);
            Point faceNormal = e1.cross(e2);
            faceNormals->push_back(faceNormal);
            for (int k=0; k<3; k++) {
              pointsToFaceNormals->at(vertexInd[k]).push_back(faceNormals->size()-1);
            }
          }
        }

      }
      else if (items[0].compare("mtllib") == 0) {
        //std::cout << "MTL file: " << items[1] << std::endl;
      }
      else if (items[0].compare("usemtl") == 0) {
        //std::cout << "Material: " << items[1] << std::endl;
      }
      else {
        std::cerr << "Unknown type: " << items[0] << std::endl;
        continue;
      }
    }
    scnFile.close();
  }
  else std::cout << "Unable to open file" << std::endl;

  // double center[3] = {(maxX-minX)/2,(maxY-minY)/2,(maxZ-minZ)/2};
  // obj_centers.push_back(std::vector<double>(center,center+3));
  double translation[3] = {0,0,-4};
  trans.push_back(std::vector<double>(translation,translation+3));
  double rotation[3] = {0,0,0};
  rot.push_back(std::vector<double>(rotation,rotation+3));

  if (vertNormals->size()==0){
    for (int i=0; i<pointsToFaceNormals->size(); i++){
      Point normal = Point();
      for (int j=0; j<pointsToFaceNormals->at(i).size(); j++){
        normal+=faceNormals->at(pointsToFaceNormals->at(i)[j]);
      }
      normal = normal/normal.norm();
      vertNormals->push_back(normal);
    }
    facesToNormals = facesToPoints;
  }
  assert(("facesToPoints and facesToNormals should contain the SAME number of face!",facesToPoints->size()==facesToNormals->size()));
  objModels.push_back(ObjModel(points,facesToPoints,vertNormals,facesToNormals));
}

// void Parser::loadMTL(std::string filename) {
//   //TODO
// }

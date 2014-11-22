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

int obj_num = 0;

void Parser::parse(int* argc, char** argv) {
  std::string input_filename = "";
  std::string ext = "";
  std::string output_filename = "";
  bool adaptive = false;
  double param = 0.1;

  std::string arg;
  for (int i=1; i < *argc; i++) {
    arg = argv[i];

    if (i==1) {
      input_filename = arg;

      int dotIndex = input_filename.find_last_of(".");
      ext = input_filename.substr(dotIndex+1,input_filename.size());
      
    }
    else if (i==2 && std::atof(argv[i])!=0) {
      param = std::atof(argv[i]);
    }
    else if (arg.compare("-a")==0) {
      adaptive = true;
    }
    else if (arg.compare("-o")==0) {
      output_filename = argv[++i];
    }
  }

  if (ext.compare("bez")==0) {
    loadBez(input_filename,param,adaptive);
    if (output_filename.size()>1){
      saveObj(output_filename);
      exit(0);
    }

    //Initial offset to fit model on screen
    double center[3] = {0,0,0};
    obj_centers.push_back(std::vector<double>(center,center+3));
    double translation[3] = {0,0,-6};
    trans.push_back(std::vector<double>(translation,translation+3));
    double rotation[3] = {0,0,0};
    rot.push_back(std::vector<double>(rotation,rotation+3));
  }
  else if (ext.compare("obj")==0) {
    loadOBJ(input_filename);

    //Initial offset to fit model on screen
    double center[3] = {0,0,0};
    obj_centers.push_back(std::vector<double>(center,center+3));
    double translation[3] = {0,0,-4};
    trans.push_back(std::vector<double>(translation,translation+3));
    double rotation[3] = {0,0,0};
    rot.push_back(std::vector<double>(rotation,rotation+3));
  }
  else if (ext.compare("scene")==0) {
    loadScene(input_filename);
  }
}

void Parser::loadScene(std::string filename) {
  std::string model_filename = "";
  std::string model_ext = "";
  double translate[3] = {0,0,0};
  double center[3] = {0,0,0};


  std::ifstream scnFile(filename.c_str());
  if (scnFile.is_open()) {
    std::string line;
    while (std::getline(scnFile,line)) {
      char delim = ' ';
      std::replace(line.begin(),line.end(),'\t',delim); //Replace tabs with delimiter
      std::replace(line.begin(),line.end(),'\r',delim); //Replace carriage_return with delim
      std::stringstream paramStream(line);

      std::string item;
      std::vector< std::string > items;
      while(std::getline(paramStream, item, delim)) {
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
      else if (items[0].compare("model") == 0) {
        model_filename = items[1];

        int dotIndex = model_filename.find_last_of(".");
        model_ext = filename.substr(dotIndex+1,filename.size());
        if (model_ext.compare("bez")==0) {
          //loadBez(filename,param,adaptive);
        }
        else if (model_ext.compare("obj")==0) {
          //loadOBJ(filename);
        }
      }
      else if (items[0].compare("center") == 0) {
        center[0] = atof(items[1].c_str());
        center[1] = atof(items[2].c_str());
        center[2] = atof(items[3].c_str());
      }
      else if (items[0].compare("xft") == 0) {
        translate[0] += atof(items[1].c_str());
        translate[1] += atof(items[2].c_str());
        translate[2] += atof(items[3].c_str());
      }
      else {
        std::cerr << "Unknown type: " << items[0] << std::endl;
        continue;
      }
      std::cout << items[0] << std::endl;
    }
    scnFile.close();
  }
  else std::cout << "Unable to open file" << std::endl;

  // if (outputName.size()<1) {
  //   outputName = "out.png";
  // }
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

          patches.back().setIndex(obj_num); // Object indexing is a lazy hack in place of a model class interface
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

    bezObjs.push_back(patches);
    obj_num++;
  }
  else std::cout << "Unable to open file" << std::endl;
}

void Parser::saveObj(std::string filename) {
  //Saves the first object in bezObjs
  std::vector<Point> points;
  std::vector<Point> normals;
  std::vector<std::vector<int> > faces;
  for (int i=0; i<bezObjs[0].size(); i++) {
    int offset = points.size();
    int tempFace[3];
    
    std::vector<std::vector<int> > tempFaces = bezObjs[0][i].getFaces();
    for (int j=0; j<tempFaces.size(); j++) {
      for (int k=0; k<3; k++) {
        tempFace[k] = tempFaces[j][k] + offset;
      }
      faces.push_back(std::vector<int>(tempFace,tempFace+3));
    }

    std::vector<Vertex> tempVertices = bezObjs[0][i].getVertices();
    for (int j=0; j<tempVertices.size(); j++) {
      points.push_back(tempVertices[j].pos());
      normals.push_back(tempVertices[j].normal());
    }
  }

  std::ofstream objFile;
  objFile.open (filename.c_str());
  for (int i=0; i<points.size(); i++){
    objFile << "v " << points[i][0] << " " << points[i][1] << " " << points[i][2] << "\n";
  }
  for (int i=0; i<normals.size(); i++){
    objFile << "vn " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << "\n";
  }
  for (int i=0; i<faces.size(); i++){
    objFile << "f";
    for (int j=0; j<3; j++){
      objFile << " " << faces[i][j] << "//" << faces[i][j];
    }
    objFile << "\n";
  }
  objFile.close();

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

  objModels.back().setIndex(obj_num++); // Object indexing is a lazy hack in place of a model class interface
}

// void Parser::loadMTL(std::string filename) {
//   //TODO
// }

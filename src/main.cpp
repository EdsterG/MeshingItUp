#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cassert>

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

#include <time.h>
#include <math.h>

#include <vector>
#include "viewport.h"
#include "parser.h"
#include "bezpatch.h"
#include "objmodel.h"
#include "point.h"
#include <Eigen/Core>
#include <Eigen/LU>

#define PI 3.14159265  // Should be used from mathlib
//#define SPACEBAR ' '
#define SPACEBAR 32
#define SHIFTKEY 31

// #define INIT_WINDOW_WIDTH 800.0
// #define INIT_WINDOW_HEIGHT 800.0

using namespace stl;

enum Shading {
  FLAT,
  SMOOTH
};
Shading shading = SMOOTH;

enum Mode {
  FILLED,
  WIREFRAME,
  HIDDEN
};
Mode mode = FILLED;

//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
std::vector<std::vector<BezPatch> > bezObjs;
std::vector<ObjModel> objModels;
std::vector<std::vector<double> > obj_centers;
std::vector<std::vector<double> > trans;
std::vector<std::vector<double> > rot;
int current_obj = 0;

int prev_x = 0;
int prev_y = 0;
int curr_x = 0;
int curr_y = 0;
int arcball_on = false;

Point get_arcball_vector(int x, int y) {
  Point point = Point((double)x/viewport.getW()*2 - 1.0,
        (double)y/viewport.getH()*2 - 1.0,
        0);
  point.setY(-point[1]);
  float norm_squared = point.dot(point);
  if (norm_squared <= 1)
    point.setY(sqrt(1 - norm_squared));  // Pythagore
  else
    point = point/point.norm();  // nearest point
  return point;
}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.resize(w,h);

  glViewport (0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  if (h == 0) { h=1;}
  float ratio = (float)w /(float)h;
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 200.0f);
}


//****************************************************
// Simple init function
//****************************************************
void initScene(){

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//   GLuint depthTexture;
//   glGenTextures(1, &depthTexture);
//   glBindTexture(GL_TEXTURE_2D, depthTexture);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glClearColor (0.0, 0.0, 0.0, 0.0);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

}

//****************************************************
// functions that do the actual drawing of stuff
//***************************************************
void drawObjects() {
  int obj_index;
  for (int k=0; k<bezObjs.size(); k++){
    obj_index = bezObjs[k][0].getIndex();
    glLoadIdentity(); // make sure transformation is "zero'd"
    glTranslatef(trans[obj_index][0], trans[obj_index][1], trans[obj_index][2]);
    glRotatef(rot[obj_index][0],1.0,0.0,0.0);
    glRotatef(rot[obj_index][1],0.0,1.0,0.0);
    glRotatef(rot[obj_index][2],0.0,0.0,1.0);
    glTranslatef(-obj_centers[obj_index][0], -obj_centers[obj_index][1], -obj_centers[obj_index][2]); 
    for (int i=0; i<bezObjs[k].size(); i++) {
      bezObjs[k][i].draw();
    }
  }
  for (int k=0; k<objModels.size(); k++) {
    obj_index = objModels[k].getIndex();
    glLoadIdentity(); // make sure transformation is "zero'd"
    glTranslatef(trans[obj_index][0], trans[obj_index][1], trans[obj_index][2]);
    glRotatef(rot[obj_index][0],1.0,0.0,0.0);
    glRotatef(rot[obj_index][1],0.0,1.0,0.0);
    glRotatef(rot[obj_index][2],0.0,0.0,1.0);
    objModels[k].draw();
  }
}

void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations

  // Code to draw objects
  if (shading==FLAT) glShadeModel(GL_FLAT);
  else glShadeModel(GL_SMOOTH);

  if (mode==FILLED) {
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  else {
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  glColor3f(1.0,1.0,1.0);
  drawObjects();

  if (mode==HIDDEN) {
    // http://www.glprogramming.com/red/chapter14.html#name16
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glColor3f(0.0f, 0.0f, 0.0f); // Background color
    drawObjects();
    glDisable(GL_POLYGON_OFFSET_FILL);
  }

  glutPostRedisplay();

  //-----------------------------------------------------------------------

  /* onIdle() */
  if (curr_x != prev_x || curr_y != prev_y) {
    Point va = get_arcball_vector(prev_x, prev_y);
    Point vb = get_arcball_vector( curr_x,  curr_y);
    float angle = acos(fmin(1.0f, va.dot(vb)));
    Point axis_in_camera_coord = va.cross(vb);
    // Eigen::Matrix3d camera2object = (std::transform[MODE_CAMERA] * Eigen::Matrix3d(mesh.object2world)).inverse;
    // Point axis_in_object_coord = camera2object * axis_in_camera_coord;
  // mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
  // prev_x = curr_x;
  // prev_y = curr_y;
}
  //glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// function to process keyboard input
//***************************************************
void myKeyboard(unsigned char key, int x, int y) {
  if (key == 'q' || key == SPACEBAR) {
    exit(0);
  }
  if (key == 's') {
    //toggle between flat and smooth shading
    if (shading==SMOOTH) shading=FLAT;
    else shading=SMOOTH;
  }
  if (key == 'w') {
    //toggle between filled and wireframe mode
    if (mode!=WIREFRAME) mode=WIREFRAME;
    else mode=FILLED;
  }
  if (key == 'h') {
    //toggle between filled and hidden-line mode
    if (mode!=HIDDEN) mode=HIDDEN;
    else mode=WIREFRAME;
  }
  if (key == '+') {
    trans[current_obj][2]+=0.1;
  }
  if (key == '-') {
    trans[current_obj][2]-=0.1;
  }

  glutPostRedisplay();
}

void myArrowKeys(int key, int x, int y) {
  int modifier = glutGetModifiers();
  if (modifier&GLUT_ACTIVE_SHIFT != 0) {
    //Translate
    switch(key) {
      case GLUT_KEY_UP:
        trans[current_obj][0]+=0.1;
        break;
      case GLUT_KEY_DOWN:
        trans[current_obj][0]-=0.1;
        break;
      case GLUT_KEY_LEFT:
        trans[current_obj][1]-=0.1;
        break;
      case GLUT_KEY_RIGHT:
        trans[current_obj][1]+=0.1;
        break;
    }
  } else {
    //Rotate
    switch(key) {
      case GLUT_KEY_UP:
        rot[current_obj][0]+=2;
        break;
      case GLUT_KEY_DOWN:
        rot[current_obj][0]-=2;
        break;
      case GLUT_KEY_LEFT:
        rot[current_obj][1]-=2;
        break;
      case GLUT_KEY_RIGHT:
        rot[current_obj][1]+=2;
        break;
    }
  }

  glutPostRedisplay();
}

//****************************************************
// function to process mouse input
//***************************************************
void myMouse(int button, int state, int x, int y) {
  //Do something later.
  //Possible button inputs: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
  //Possible state inputs: GLUT_UP or GLUT_DOWN
  // glutPostRedisplay();
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    arcball_on = true;
    prev_x = curr_x = x;
    prev_y = curr_y = y;
  } else {
    arcball_on = false;
  }
}

void myMouseMotion(int mouseX, int mouseY) {
  // // Location of the center of pixel relative to center of sphere
  // double radius = mouseLightRadius;
  // double x = (mouseX-viewport.getW()/2.0);
  // double y = -(mouseY-viewport.getH()/2.0);
  // double dist = sqrt(std::pow(x,2) + std::pow(y,2));
  // if (dist > radius) { dist = radius;}
  // double z = sqrt(radius*radius-dist*dist);
  // if (lights.size()>0){
  //   lights[mouseLight]->moveLight(Vector(x,y,z));
  // }

  // glutPostRedisplay();
  if (arcball_on) {  // if left button is pressed
    curr_x = mouseX;
    curr_y = mouseY;
  }
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  //This parses the arguments and sets up the global variables
  Parser argParser = Parser();
  argParser.parse(&argc, argv);

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels, and a depth buffer.
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport
  viewport = Viewport(400,400);

  //The size and position of the window
  glutInitWindowSize(viewport.getW(), viewport.getH());
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutKeyboardFunc(myKeyboard);           // function to run when its time to read keyboard input
  glutSpecialFunc(myArrowKeys);           // function to run when arrow keys are pressed
  glutMotionFunc(myMouseMotion);          // function to run when mouse input is received
  glutMouseFunc(myMouse);                 // function to run when mouse input is received
  glutDisplayFunc(myDisplay);             // function to run when its time to draw something
  glutReshapeFunc(myReshape);             // function to run when the window gets resized
  glutMainLoop();                         // infinite loop that will keep drawing and resizing

  return 0;
}

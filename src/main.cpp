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

//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
std::vector<BezPatch> patches;
std::vector<std::vector<double> > trans;
std::vector<std::vector<double> > rot;
int current_obj = 0;

//
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;


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
  glShadeModel (GL_SMOOTH);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

}

//****************************************************
// function to process mouse input
//***************************************************

Point get_arcball_vector(int x, int y) {
  Point P = Point(1.0*x/viewport.getW()*2 - 1.0,
        1.0*y/viewport.getH()*2 - 1.0,
        0);
  P.setY(-1*P[1]);
  float OP_squared = P[0] * P[0] + P[1] * P[1];
  if (OP_squared <= 1*1)
    P.setY(sqrt(1*1 - OP_squared));  // Pythagore
  else
    P.norm();  // nearest point
  return P;
}

void myMouse(int button, int state, int x, int y) {
  //Do something later.
  //Possible button inputs: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
  //Possible state inputs: GLUT_UP or GLUT_DOWN
  // glutPostRedisplay();
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    arcball_on = true;
    last_mx = cur_mx = x;
    last_my = cur_my = y;
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
    cur_mx = mouseX;
    cur_my = mouseY;
  }
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Code to draw objects
  //-----------------------------------------------------------------------
  // if (!fileName.empty()){
  //   viewport.beginImage();
  // }

  // Start drawing
  glTranslatef(trans[current_obj][0], trans[current_obj][1], trans[current_obj][2]);
  glRotatef(rot[current_obj][0],1.0,0.0,0.0);
  glRotatef(rot[current_obj][1],0.0,1.0,0.0);
  glRotatef(rot[current_obj][2],0.0,0.0,1.0);

  //FILLED
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //WIRED
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  for (int i=0; i<patches.size(); i++) {
    patches[i].draw();
  }


  //HIDDEN
  // http://www.glprogramming.com/red/chapter14.html#name16
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glColor3f(1.0,0.0,0.0);
  // for (int i=0; i<patches.size(); i++) {
  //   patches[i].draw();
  // }

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glEnable(GL_POLYGON_OFFSET_FILL);
  // glPolygonOffset(1.0, 1.0);
  // glColor3f(0.0f, 0.0f, 0.0f); // Background color
  // for (int i=0; i<patches.size(); i++) {
  //   patches[i].draw();
  // }
  // glDisable(GL_POLYGON_OFFSET_FILL);

  // glBegin(GL_TRIANGLES);
  //   glColor3f(1.0,0.0,0.0);
  //   glVertex3f(0.0,1,0.0);
  //   glVertex3f(-1,-1,-1);
  //   glVertex3f(1,-1,-1);

  //   glColor3f(0.0,1.0,0.0);
  //   glVertex3f(0.0,1,0.0);
  //   glVertex3f(-1,-1,-1);
  //   glVertex3f(0.0,-1,1);

  //   glColor3f(0.0,0.0,1.0);
  //   glVertex3f(0.0,1,0.0);
  //   glVertex3f(1,-1,-1);
  //   glVertex3f(0.0,-1,1);

  //   glColor3f(1.0,1,1.0);
  //   glVertex3f(-1,-1,-1);
  //   glVertex3f(1,-1,-1);
  //   glVertex3f(0.0,-1,1);
  // glEnd();

  //glutPostRedisplay();

  // if (!fileName.empty()){
  //   viewport.endImage();
  //   viewport.writeImage(fileName.c_str());
  //   exit(0);
  // }

  //-----------------------------------------------------------------------

  /* onIdle() */
  if (cur_mx != last_mx || cur_my != last_my) {
    Point va = get_arcball_vector(last_mx, last_my);
    Point vb = get_arcball_vector( cur_mx,  cur_my);
    float angle = acos(fmin(1.0f, va.dot(vb)));
    Point axis_in_camera_coord = va.cross(vb);
    // Eigen::Matrix3d camera2object = (std::transform[MODE_CAMERA] * Eigen::Matrix3d(mesh.object2world)).inverse;
    // Point axis_in_object_coord = camera2object * axis_in_camera_coord;
  // mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
  // last_mx = cur_mx;
  // last_my = cur_my;
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
  }
  if (key == 'w') {
    //toggle between filled and wireframe mode
  }
  if (key == 'h') {
    //Optional: toggle between filled and hidden-line mode.
  }
  if (key == '+') {
    trans[current_obj][2]+=0.1;
    std::cout << "ZOMM IN" << std::endl;
  }
  if (key == '-') {
    trans[current_obj][2]-=0.1;
    std::cout << "ZOOM OUT" << std::endl;
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
        std::cout << "SHIFT+UP" << std::endl;
        break;
      case GLUT_KEY_DOWN:
        trans[current_obj][0]-=0.1;
        std::cout << "SHIFT+DOWN" << std::endl;
        break;
      case GLUT_KEY_LEFT:
        trans[current_obj][1]-=0.1;
        std::cout << "SHIFT+LEFT" << std::endl;
        break;
      case GLUT_KEY_RIGHT:
        trans[current_obj][1]+=0.1;
        std::cout << "SHIFT+RIGHT" << std::endl;
        break;
    }
  } else {
    //Rotate
    switch(key) {
      case GLUT_KEY_UP:
        rot[current_obj][0]+=2;
        std::cout << "UP" << std::endl;
        break;
      case GLUT_KEY_DOWN:
        rot[current_obj][0]-=2;
        std::cout << "DOWN" << std::endl;
        break;
      case GLUT_KEY_LEFT:
        rot[current_obj][1]-=2;
        std::cout << "LEFT" << std::endl;
        break;
      case GLUT_KEY_RIGHT:
        rot[current_obj][1]+=2;
        std::cout << "RIGHT" << std::endl;
        break;
    }
  }

  glutPostRedisplay();
}


//Functions to test our code
void test() {
  // std::cout << std::endl << "BEGIN TEST FUNCTION" << std::endl;
  // viewport.beginImage();
  // myDisplay();
  // viewport.endImage();
  // viewport.writeImage("test.png");

  // //Test Color class
  // Color c1 = Color();
  // assert(c1.getR()==0.0 && c1.getG()==0.0 && c1.getB()==0.0);
  // std::cout << c1 << std::endl;
  // Color c2 = Color(0.1,0.2,0.5);
  // assert(c2.getR()==0.1 && c2.getG()==0.2 && c2.getB()==0.5);
  // std::cout << c2 << std::endl;
  // Color c3 = c2+c2+c2;
  // std::cout << c3 << std::endl;

  // //Test Vector class
  // Vector v1 = Vector();
  // Vector v2 = Vector(2,1,2);
  // Vector v3 = Vector(9);
  // Vector v4 = Vector(1,1,1);
  // Vector v5 = Vector(1,-1,1);
  // std::cout << v1 << std::endl;
  // std::cout << v2 << std::endl;
  // std::cout << v3 << std::endl;
  // std::cout << v4.cross(v5) << std::endl;
  // std::cout << v5.cross(v4) << std::endl;
  // std::cout << v2.dot(v3) << std::endl;
  // std::cout << v2.magnitude() << std::endl;
  // std::cout << v2/v2.magnitude() << std::endl;

  // //Test Light class
  // PointLight pl1 = PointLight(Vector(11,12,13));
  // DirectionLight dl1 = DirectionLight(Vector(14,15,16));
  // std::cout << pl1 << std::endl;
  // std::cout << dl1 << std::endl;

  // std::cout << "END TEST FUNCTION" << std::endl << std::endl;
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

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

  //This parses the arguments and sets up the global variables
  Parser argParser = Parser();
  argParser.parse(&argc, argv);

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

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


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.resize(w,h);

  glViewport (0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(45.0, (double)w / (double)h, 1.0, 2000.0);

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system

  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/WINDOW_WIDTH, w/WINDOW_WIDTH, -h/WINDOW_HEIGHT, h/WINDOW_HEIGHT, 1, -1); // resize type = center

  //glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch
  //------------------------------gl------------------------------
  glMatrixMode(GL_MODELVIEW);
}


//****************************************************
// Simple init function
//****************************************************
void initScene(){
  // // Nothing to do here for this simple example.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear to black, fully transparent
  glClearDepth(1.0f);
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // gluPerspective(45.0,WINDOW_WIDTH/WINDOW_HEIGHT,1.0,2000.0);
  // glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // myReshape(viewport.getW(),viewport.getH());

}


//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  //glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Code to draw objects
  //-----------------------------------------------------------------------
  // if (!fileName.empty()){
  //   viewport.beginImage();
  // }

  // Start drawing
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for (int i=0; i<patches.size(); i++) {
    glColor3f(1.0,0.0,0.0);
    glTranslatef(0.0, 0.5, -0.5);
    glRotatef(40,0.0,1.0,1.0);
      // glTranslatef(trans[0][0], trans[0][1], trans[0][2]);
      // glRotatef(rot[0],1.0,0.0,0.0);
      // glRotatef(rot[1],0.0,1.0,0.0);
      // glRotatef(rot[2],0.0,0.0,1.0);
      // glShadeModel(GL_SMOOTH);
    patches[i].draw();
  }

// http://www.glprogramming.com/red/chapter14.html#name16
// glEnable(GL_DEPTH_TEST);
// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// set_color(foreground);
// draw_object_with_filled_polygons();

// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// glEnable(GL_POLYGON_OFFSET_FILL);
// glPolygonOffset(1.0, 1.0);
// set_color(background);
// draw_object_with_filled_polygons();
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

  glutPostRedisplay();
  
  // if (!fileName.empty()){
  //   viewport.endImage();
  //   viewport.writeImage(fileName.c_str());
  //   exit(0);
  // }

  //-----------------------------------------------------------------------

  glFlush();
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
    //Zoom in
  }
  if (key == '-') {
    //Zoom out
  }

  glutPostRedisplay();
}

void myArrowKeys(int key, int x, int y) {
  int modifier = glutGetModifiers();
  if (modifier&GLUT_ACTIVE_SHIFT != 0) {
    //Translate
    switch(key) {
      case GLUT_KEY_UP:
        std::cout << "SHIFT+UP" << std::endl;
        break;
      case GLUT_KEY_DOWN:
        std::cout << "SHIFT+DOWN" << std::endl;
        break;
      case GLUT_KEY_LEFT:
        std::cout << "SHIFT+LEFT" << std::endl;
        break;
      case GLUT_KEY_RIGHT:
        std::cout << "SHIFT+RIGHT" << std::endl;
        break;
    }
  } else {
    //Rotate
    switch(key) {
      case GLUT_KEY_UP:
        std::cout << "UP" << std::endl;
        break;
      case GLUT_KEY_DOWN:
        std::cout << "DOWN" << std::endl;
        break;
      case GLUT_KEY_LEFT:
        std::cout << "LEFT" << std::endl;
        break;
      case GLUT_KEY_RIGHT:
        std::cout << "RIGHT" << std::endl;
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
}

void myMouseMotion(int mouseX, int mouseY) {
  // // Location of the center of pixel relative to center of sphere
  // float radius = mouseLightRadius;
  // float x = (mouseX-viewport.getW()/2.0);
  // float y = -(mouseY-viewport.getH()/2.0);
  // float dist = sqrt(std::pow(x,2) + std::pow(y,2));
  // if (dist > radius) { dist = radius;}
  // float z = sqrt(radius*radius-dist*dist);
  // if (lights.size()>0){
  //   lights[mouseLight]->moveLight(Vector(x,y,z));
  // }

  // glutPostRedisplay();
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
  //glutReshapeFunc(myReshape);             // function to run when the window gets resized
  glutMainLoop();                         // infinite loop that will keep drawing and resizing

  return 0;
}









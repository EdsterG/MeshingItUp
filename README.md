CS184 - Foundations of Computer Graphics
Assignment 3: Bezier Subdivision
==========================================

Names:
------
  Wisam Reid (cs184-ds)
  Eddie Groshev (cs184-en)

Platform:
---------
  Mac OSX

  Tested on Mac OSX 10.9.5 and Mac OSX 10.6.8 on personal computer[s].

  Checked on

Submission:
-------------
  Code by Eddie Groshev (cs184-en).
  README by Wisam Reid (cs184-ds)

BUILD:
----------
  Run 'make' to build

SOURCE FILES:
---------------
  src:
  ---
  main.cpp
  objmodel.cpp
  parser.cpp
  point.cpp
  vertex.cpp
  viewpoint.cpp
  bezpatch.cpp

  include:
  -------
  objmodel.h
  parser.h
  point.h
  vertex.h
  viewpoint.h
  bezpatch.h

LIBRARIES:
------------
  eigen


FUNCTIONALITY (SPEC):
-----------------------

  1) Tessellate/triangulate the patch using either a uniform or adaptive technique. (You have to
implement both, your program will have a command-line flag to determine which is used.)

  2) Running the executable opens a window and uses OpenGL to Render the object.

  Controls:
  --------
    1) When "s" is pressed the program will toggle between flat and smooth shading.

    2) When "w" is pressed the program will toggle between filled and wireframe mode.  

FUNCTIONALITY (E/C):
----------------------

  1) When "h" is pressed the program will toggle between filled and hidden-line mode.

  2) When the arrow keys are pressed the object will be rotated.

  3) When the shift+arrow keys are pressed the object will be translated.

  4) When launched the initial zoom will show the entire object.

  5) Pressing the +/- keys will zoom in/out.

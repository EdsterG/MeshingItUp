##########################################
CS184 - Foundations of Computer Graphics
Assignment 3: Bezier Subdivision
##########################################

Authors:
----------
  Wisam Reid (cs184-ds)
  Eddie Groshev (cs184-en)

Platform:
-----------
  Mac OSX

  Tested on Mac OSX 10.9.5 and Mac OSX 10.6.8 on personal computer(s).

  Checked on

Submission:
-------------
  Code by Eddie Groshev (cs184-en).
  README by Wisam Reid (cs184-ds)

BUILD:
--------
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

Libraries and credits:
------------------------
  eigen
    https://github.com/cryos/eigen

  hidden-line removal code segment
    http://www.glprogramming.com/red/chapter14.html#name16

FUNCTIONALITY (SPEC):
-----------------------

  1) Tessellation using either a uniform or adaptive technique.

        image-01.png
          input: ./as3 models/teapot.bez 0.1
          key command: "w"

        image-02.png
          input: ./as3 models/teapot.bez 0.1 -a
          key command: "w"

        image-05.png
          input: ./as3 models/test.bez 0.01 -a
          key command: "w"

  2) Running the executable opens a window and uses OpenGL to Render the object.

        image-03.png
          input: ./as3 models/teapot.bez 0.1

  Controls:
  --------

    1) When "s" is pressed the program will toggle between flat and smooth shading.

            See BezierDemo.gif

            image-06.png
              input: ./as3 models/bunny.obj  0.1
              key command: "s"

    2) When "w" is pressed the program will toggle between filled and wireframe mode.

            See BezierDemo.gif

            image-01.png
              input: ./as3 models/teapot.bez 0.1
              key command: "w"

            image-02.png
              input: ./as3 models/teapot.bez 0.1 -a
              key command: "w"

            image-05.png
              input: ./as3 models/test.bez 0.01 -a
              key command: "w"

  Command line arguments:
  ----------------------

    1) Two parameters input file name and subdivision parameter

            eg: ./as3 teapot.bez 0.1

                image-01.png

    2) flag for uniform or adaptive tessellation (Uniform tessellation by default -a for adaptive)

            eg: ./as3 teapot.bez 0.1 -a

                image-02.png


FUNCTIONALITY (E/C):
----------------------

  1) When "h" is pressed the program will toggle between filled and hidden-line mode.

          See BezierDemo.gif

          image-07.png
            input: ./as3 models/test.bez  0.01
            key command: "h"

  2) When the arrow keys are pressed the object will be rotated.

          See BezierDemo.gif

  3) When the shift+arrow keys are pressed the object will be translated.

          See BezierDemo.gif

  4) When launched the initial zoom will show the entire object.

          See BezierDemo.gif

  5) Pressing the +/- keys will zoom in/out.

          See BezierDemo.gif

  6) Load and display OBJ files

          image-04.png
            input: ./as3 models/bunny.obj  0.1

  7) Output OBJ files

  8) The ability to load multiple objects
      - mix of .bez and .obj files
      - display them each with different transformations


IMAGES:
---------

BezierDemo.gif

image-01.png
  input: ./as3 models/teapot.bez 0.1
  key command: "w"

image-02.png
  input: ./as3 models/teapot.bez 0.1 -a
  key command: "w"

image-03.png
  input: ./as3 models/teapot.bez 0.1

image-04.png
  input: ./as3 models/bunny.obj 0.1

image-05.png
  input: ./as3 models/teapot.bez 0.1
  key command: "w"

image-06.png
  input: ./as3 models/bunny.obj  0.1
  key command: "s"

image-07.png
  input: ./as3 models/test.bez  0.01
  key command: "h"

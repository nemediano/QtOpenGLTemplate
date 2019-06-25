# QtOpenGLTemplate
A template to develop OpenGL appliations using Qt

This template is aimed to create an application that
contains:

* A app widget, with a sidebar, menu status bar and a
  central widget inheriting from QOpenGLWidget that can
  be used for rendering a scene.

* An already made connection with a QOpenGLDebugLogger
  that output OpenGL debug messages to console.

* Functionality to query OpenGL and Qt Versions used.

* Placeholder to specify a very specific OpenGL context

* Functionality to go into full-screen mode

* Functionality to take screnshoots in png format from
  the rendering widget

* Uses [GLM](http://glm.g-truc.net) for the mathematics code since its univerally adopted

  * And includes conversion functions between GLM basic types and
    equivalent Qt types, so you can use Qt-OpenGL helper functionality

* A class to abstract a simple trackball camera.

* A simple "Hello world" example, that renders simple tetrahedron

The aim of this template is to have minimal code that let you focus
in the OpenGL rather than the Qt.

Is perfect to start learning OpenGL using the [RedBook](http://www.opengl-redbook.com/) or the [OpenGL SuperBible](http://www.openglsuperbible.com/). Or by following any of the websites [OpenGL tutorial](http://www.opengl-tutorial.org/) and [Learn OpenGL](https://learnopengl.com/)

If you follow any of the above, you can (and should) use this template instead of GLEW, glf, freeglut, etc. This template substitute a windows manager library and an OpenGL loader library.
Since, Qt already have an image loader, you will only need a Mesh loader library like [Open Asset Importer](http://www.assimp.org/) in order to fullfill your graphics needs.

## Usage

You need to have a working installation of Qt. Ideally, one that includes QtCreator. In that case you need to clone (or extract) the MyGLWidget folder inside of your Qt workspace and then
fullfill the rest of the requisites which are easy to deduct from the [Qt pro file](MyGLWidget/MyGLWidget.pro)
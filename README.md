# QtOpenGLTemplate
A template to develop OpenGL appliations using QT

This template is aimed to create an application that
contains:

* A app widget, with a sidebar, menu status bar and a
  central widget inheriting from QtOpenGLWidget that can
  be used for rendering a scene.

* An already made connection with a QOpenGLDebugLogger
  that output OpenGL debug messages to console.

* Functionality to query OpenGL and Qt Versions used.

* Placeholde to specify a very specific OpenGl context

* Functionality to go into fullscreen mode

* Functionality to take screnshoots in png format from
  the rendering widget

* Conversion functions between GLM basic types and
  equivalent Qt types

* A class to abstract a simple trackball camera.

* A simple "Hello world" example, that renders simple thet

The aim of this template is to have minimal code that let you focus
in the OpenGL rather than the Qt.
#ifndef TESTOGLWIDGET_H
#define TESTOGLWIDGET_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QVector>

#include "baseoglwidget.h"

#include <glm/glm.hpp>

//! Custom structure for sending data to GPU
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

//! Main OpenGL application class
/*!
  This is an example of how to use the template. This class inherits
  \class BaseOGLWidget and use it to implement the main OpenGL application's
  logic.

  Since, also contains \class QOpenGLWidget as his ancestor. You should
  override and implement: initializeGL, resizeGL and paintGL

  Additionally, you can use the functionality provided by BaseGLWidget.
*/
class TestOGLWidget : public BaseOGLWidget {
public:
    explicit TestOGLWidget(QWidget* parent = nullptr);
    ~TestOGLWidget() override;
    //! Public interface to the rest (UI for example)
    bool getRotation() const;

protected:
    //! Initial logic (the context is ready at this point)
    void initializeGL() override;
    //! Resize logic, to update calculations like the projection
    void resizeGL(int width, int height) override;
    //! OpenGL render logic
    void paintGL() override;
    //! An OpenGL program (the one that defines the pipeline)
    QOpenGLShaderProgram* mGLProgPtr;

    // Qt objects to interact with OpenGL: store and send data to GPU
    //! Vertex data to send to GPU
    QOpenGLBuffer mVertexBuffer;
    //! Index data to send to GPU
    QOpenGLBuffer mIndexBuffer;
    //! Vertex Array object for OpenGL
    QOpenGLVertexArrayObject mVAO;

    // Program specific variables
    //! Application logic variable
    bool mRotating{false};
    //! Frame counter
    int mFrame{0};

    // CPU side buffers
    //! CPU array of Vertices
    QVector<Vertex> mVertices;
    //! CPU array of indexes (this is an indexed mesh)
    QVector<unsigned int> mIndexes;

    // Helper functions
    //! Logic to create the geometry and fill the CPU side buffers
    void createGeometry();
    //! Free OpenGL resources
    void tearDownGL();

    // Example of an interaction between the OpenGL application and the user interface
public slots:
    //! (Un)Set the rotation of the mesh
    void setRotation(bool rotate);
    //! (Un)Set the rotation of the mesh
    void toogleRotation();
};

#endif // TESTOGLWIDGET_H

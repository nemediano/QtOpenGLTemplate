#ifndef TESTOGLWIDGET_H
#define TESTOGLWIDGET_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QVector>

#include "baseoglwidget.h"

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class TestOGLWidget : public BaseOGLWidget {
public:
    explicit TestOGLWidget(QWidget* parent = nullptr);
    ~TestOGLWidget();
    bool getRotation() const;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    QOpenGLShaderProgram* mGLProgPtr;

    // OpenGL State Information
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mIndexBuffer;
    QOpenGLVertexArrayObject mVAO;

    //This program specific variables
    bool mRotating{false};
    int mFrame{0};

    QVector<Vertex> mVertices;
    QVector<unsigned int> mIndexes;
    void createGeometry();
    void tearDownGL();

public slots:
    void setRotation(bool rotate);
    void toogleRotation();
};

#endif // TESTOGLWIDGET_H

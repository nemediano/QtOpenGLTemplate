#ifndef MESHLOAD_H
#define MESHLOAD_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QVector>

#include "model.h"
#include "baseGLwindow.h"

class MeshLoad : public BaseGLWindow
{
public:
    MeshLoad();
    ~MeshLoad() override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    GLuint64 mNanoseconds;
    GLuint mTimerQuery;
    QOpenGLShaderProgram* mGLProgPtr;
    QVector<QOpenGLTexture*> mTextPtr;
    QVector<QString> mTextNames;
    QVector<glm::vec3> mColors;
    QVector<MeshData> mSeparators;
    QString mModelFolder;

    int mFrame;
    float mAlpha;
    bool mRotating;

    // OpenGL State Information
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mIndexBuffer;
    QOpenGLVertexArrayObject mVAO;

    QVector<Vertex> mVertices;
    QVector<unsigned int> mIndexes;
    void createGeometry();
    void initTexture();
    void tearDownGL();

    void keyPressEvent(QKeyEvent* event) override;
};

#endif // MESHLOAD_H

#include "testoglwidget.h"
#include <glm/gtc/matrix_transform.hpp>


using glm::vec3;
using glm::mat4;
using glm::sqrt;
using glm::perspective;
using glm::ortho;
using glm::lookAt;
using glm::rotate;
using glm::scale;
using glm::radians;

//! This code is the actual USE of the template
/*!
  This is a sample of using the template.
  All the code here should change to accommodate your actual application. At the very minimum
  you will want to implement: initializeGL, paintGL and resizeGL.
  Remember, that in order to use OpenGL, you need to initialize OpenGL functions
*/

TestOGLWidget::TestOGLWidget(QWidget* parent) : BaseOGLWidget(parent), mGLProgPtr(nullptr) {
    richText(false);
}

void TestOGLWidget::initializeGL() {
    // The first thing to do, if we want to Use OpenGL, we need to do it here
    // since, is until here that we are sure our context is current
    initializeOpenGLFunctions();
    // optionally, start the OpenGL debug logger
    startLog();
    //Fill the arrays with data from the file into CPU side
    createGeometry();
    //Prepare the OpenGL shader program
    mGLProgPtr = new QOpenGLShaderProgram(this);
    mGLProgPtr->addShaderFromSourceFile(QOpenGLShader::Vertex, "../MyGLWidget/shaders/simplevert.vert");
    mGLProgPtr->addShaderFromSourceFile(QOpenGLShader::Fragment, "../MyGLWidget/shaders/simplefrag.frag");
    mGLProgPtr->link();
    int posAttr = mGLProgPtr->attributeLocation("posAttr");
    int colAttr = mGLProgPtr->attributeLocation("colAttr");
    // Transfer data form CPU to GPU and prepare the inputs for the Graphics pipeline
    {
        mGLProgPtr->bind();
        // Create Vertex Array Object (Remember this needs to be done BEFORE binding the vertex)
        // To store all the inputs prepared for this pipeline
        mVAO.create();
        mVAO.bind();
        // Create Buffers (Do not release until VAO is created, and released)
        mVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        mVertexBuffer.create();
        mVertexBuffer.bind();
        mVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mVertexBuffer.allocate(mVertices.constData(), mVertices.size() * int(sizeof(Vertex)));
        //Another one for the indices
        mIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        mIndexBuffer.create();
        mIndexBuffer.bind();
        mIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mIndexBuffer.allocate(mIndexes.constData(), mIndexes.size() * int(sizeof(unsigned int)));
        //Feed up vertex attribute to the Shader program
        mGLProgPtr->enableAttributeArray(posAttr);
        mGLProgPtr->enableAttributeArray(colAttr);
        //This is an interleaved VBO
        mGLProgPtr->setAttributeBuffer(posAttr, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
        mGLProgPtr->setAttributeBuffer(colAttr, GL_FLOAT, offsetof(Vertex, color), 3, sizeof(Vertex));
        // Release (unbind) all
        mVAO.release();
        mGLProgPtr->disableAttributeArray(posAttr);
        mGLProgPtr->disableAttributeArray(colAttr);
        mIndexBuffer.release();
        mIndexBuffer.release();
        mGLProgPtr->release();
        //Once we have a copy in the GPU, there is no need to keep a CPU copy (unless you want to)
        mIndexes.clear();
        mVertices.clear();
    }
    //Some application's specific graphic initial state
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //Some camera projection parameters. Actual matrices will be calculated in resize
    mFovY = 30.0f;
    mNear = 2.0f;
    mFar = 5.0f;
    // Camera's default position
    vec3 eye = vec3(0.0f, 0.0f, 4.0f);
    vec3 center = vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    mV = lookAt(eye, center, up);
}

void TestOGLWidget::resizeGL(int width, int height) {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, int(width * retinaScale), int(height * retinaScale));
    float aspectRatio = float(width) / float(height);
//    if (width < height) {
//        mP = ortho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, 2.0f, -2.0f);
//    } else {
//        mP = ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 2.0f, -2.0f);
//    }
    mP = perspective(radians(mFovY), aspectRatio, mNear, mFar);
    mBall.setWindowSize(width, height);
}

void TestOGLWidget::paintGL() {
    //Clear screen and start the show
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mGLProgPtr->bind();
    //Calculate view matrix
    mat4 V = mV * mBall.getRotation();
    //Calculate model matrix
    mM = mat4(1.0f);
    if (mRotating) {
        // Approx 90 degrees per second rotation
        float angle = 360.0f * mFrame / 200.0f;
        vec3 axis = vec3(0.0f, 0.0f, 1.0f);
        mM = rotate(mM, radians(angle), axis);
    }
    //Pass uniform values to shaders
    mGLProgPtr->setUniformValue("PVM", toQt(mP * V * mM));
    mVAO.bind();
    {
        // Technically it's
        // glDrawElements(GL_TRIANGLES, mIndexBuffer.size(), GL_UNSIGNED_INT, (const void*)(N * sizeof(unsigned int)));
        // where N is the offset of the indices. Since we want to draw all indices; offset is zero and
        // the pointer it's just null
        glDrawElements(GL_TRIANGLES, mIndexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    }
    mVAO.release();
    mGLProgPtr->release();

    mFrame = (mFrame + 1) % 200;
    update();
}

void TestOGLWidget::createGeometry() {
    mVertices.clear();
    mIndexes.clear();

    mVertices.push_back(Vertex{{ sqrt(8.0f / 9.0f), 0.0f, -1.0f / 3.0f},              {1.0f, 0.25f, 0.25f}});
    mVertices.push_back(Vertex{{-sqrt(2.0f / 9.0f), sqrt(2.0f / 3.0f), -1.0f / 3.0f}, {0.25f, 1.0f, 0.25f}});
    mVertices.push_back(Vertex{{-sqrt(2.0f / 9.0f),-sqrt(2.0f / 3.0f), -1.0f / 3.0f}, {0.25f, 0.25f, 1.0f}});
    mVertices.push_back(Vertex{{0.0f, 0.0f, 1.0f},                                    {1.0f,  1.0f, 0.25f}});

    mIndexes.push_back(0u);
    mIndexes.push_back(2u);
    mIndexes.push_back(1u);

    mIndexes.push_back(0u);
    mIndexes.push_back(1u);
    mIndexes.push_back(3u);

    mIndexes.push_back(3u);
    mIndexes.push_back(1u);
    mIndexes.push_back(2u);

    mIndexes.push_back(3u);
    mIndexes.push_back(2u);
    mIndexes.push_back(0u);
}

void TestOGLWidget::tearDownGL() {
    //Release GPU memory
    mVertexBuffer.destroy();
    mIndexBuffer.destroy();
    //Destroy pipeline configuration
    mVAO.destroy();
    if (mGLProgPtr) {
        delete mGLProgPtr;
    }
    //Stop logging in this context
    stopLog();
}

TestOGLWidget::~TestOGLWidget() {
    //Make our OpenGL context the current context, so we can destroy it
    makeCurrent();
    tearDownGL();
}

bool TestOGLWidget::getRotation() const {
    return mRotating;
}

/* These two function are a sample of an interface with UI.
 * They become slots called by the main window */
void TestOGLWidget::setRotation(bool rotate) {
    if (rotate != mRotating) {
        mRotating = rotate;
        update();
    }
}

void TestOGLWidget::toogleRotation() {
    mRotating = !mRotating;
    update();
}



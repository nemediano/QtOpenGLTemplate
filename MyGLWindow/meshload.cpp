#include "meshload.h"
#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <QString>
#include <QtGui/QScreen>
#include <QFileInfo>

using glm::vec3;
using glm::mat4;
using glm::sqrt;
using glm::perspective;
using glm::lookAt;
using glm::rotate;
using glm::scale;
using glm::radians;

MeshLoad::MeshLoad() : mNanoseconds(0), mGLProgPtr(nullptr), mFrame(0) {
    richText(false);
    mAlpha = 1.5f;
    mRotating = false;
    mModelFolder = "../models/Nyra/";
}

MeshLoad::~MeshLoad() {
    //Make this OpenGl context the current context, so we can destroy it
    makeCurrent();
    tearDownGL();
}

void MeshLoad::tearDownGL() {
    //Release GPU memmory
    mVertexBuffer.destroy();
    mIndexBuffer.destroy();
    //Destry pipeline configuration
    mVAO.destroy();
    if (mGLProgPtr) {
        delete mGLProgPtr;
    }
    //Release more GPU memmory (textures)
    for (int i = 0; i < mTextPtr.length(); ++i) {
        if (mTextPtr[i]) {
            delete mTextPtr[i];
        }
    }
    //Stop logging in this context
    stopLog();
}

void MeshLoad::createGeometry() {
    mVertices.clear();
    mIndexes.clear();
    /*This is the code that we are testing, we load a model
     * that consist of several Meshes and textures into memmory CPU*/
    Model model(mModelFolder + "Nyra_pose.obj");
    model.toUnitCube();
    mSeparators = QVector<MeshData>::fromStdVector(model.getSeparators());
    mIndexes = QVector<unsigned int>::fromStdVector(model.getIndices());
    mVertices = QVector<Vertex>::fromStdVector(model.getVertices());
    //Since we use the model to get the paths for the textures, I need to do this here
    for (auto t : model.getTextures()) {
        QFileInfo file = QString::fromStdString(t.filePath);
        mTextNames.push_back(mModelFolder + file.fileName());
    }
}

void MeshLoad::initTexture()  {
    //Remember for QT texture object as well as GLProgram needs to be pointers
    for (int i = 0; i < mTextNames.length(); ++i) {
        QOpenGLTexture* textPtr= new QOpenGLTexture(QImage(mTextNames[i]).mirrored());
        textPtr->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        textPtr->setMagnificationFilter(QOpenGLTexture::Linear);
        textPtr->setWrapMode(QOpenGLTexture::Repeat);
        mTextPtr.push_back(textPtr);
    }
}

void MeshLoad::resizeGL(int width, int height) {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, int(width * retinaScale), int(height * retinaScale));
    float aspectRatio = float(width) / float(height);
    mP = perspective(radians(mFovY), aspectRatio, mNear, mFar);
    mBall.setWindowSize(width, height);
}

void MeshLoad::initializeGL() {
    initializeOpenGLFunctions();
    startLog();
    qDebug().noquote() << versionInfo();
    //Fill the arrays with data from the file into CPU side
    createGeometry();
    //Load the textures from images into GLtextures
    initTexture();
    //Prepare the OpenGL shader program
    mGLProgPtr = new QOpenGLShaderProgram(this);
    mGLProgPtr->addShaderFromSourceFile(QOpenGLShader::Vertex, "../MyGLWindow/shaders/texturedVertex.vert");
    mGLProgPtr->addShaderFromSourceFile(QOpenGLShader::Fragment, "../MyGLWindow/shaders/phongTexture.frag");
    mGLProgPtr->link();
    int posAttr = mGLProgPtr->attributeLocation("posAttr");
    int normAttr = mGLProgPtr->attributeLocation("normalAttr");
    int textAttr = mGLProgPtr->attributeLocation("textCoordAttr");
    // Transfer data from CPU to GPU and prepare the inputs for the Graphics pipeline
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
        //Feed up vertex atribute to the Shader program
        mGLProgPtr->enableAttributeArray(posAttr);
        mGLProgPtr->enableAttributeArray(normAttr);
        mGLProgPtr->enableAttributeArray(textAttr);
        //This is an interleaved VBO
        mGLProgPtr->setAttributeBuffer(posAttr, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
        mGLProgPtr->setAttributeBuffer(normAttr, GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));
        mGLProgPtr->setAttributeBuffer(textAttr, GL_FLOAT, offsetof(Vertex, textCoords), 2, sizeof(Vertex));
        // Release (unbind) all
        mVAO.release();
        mGLProgPtr->disableAttributeArray(posAttr);
        mGLProgPtr->disableAttributeArray(normAttr);
        mGLProgPtr->disableAttributeArray(textAttr);
        mIndexBuffer.release();
        mVertexBuffer.release();
        mGLProgPtr->release();
        //Once we have a copy in the GPU there is no need to keep a CPU copy (unless you want to)
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
    mNear = 1.0f;
    mFar = 5.0f;
    // Camera's default position
    vec3 eye = vec3(0.0f, 0.0f, 4.0f);
    vec3 center = vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    mV = lookAt(eye, center, up);
    //For the time query
    glGenQueries(1, &mTimerQuery);
    glBeginQuery(GL_TIME_ELAPSED, mTimerQuery);
}

void MeshLoad::paintGL() {
    //Finish the previous time query
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectui64v(mTimerQuery, GL_QUERY_RESULT, &mNanoseconds);

    //Clear screen and start the show
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mGLProgPtr->bind();
    //Calculate view matrix
    mat4 V = mV * mBall.getRotation();
    //Calculate model matrix
    mM = mat4(1.0f);
    if (mRotating) {
        // Approx 90 degress per second rotation
        float angle = 90.0f * mFrame / float(screen()->refreshRate());
        vec3 axis = vec3(0.0f, 1.0f, 0.0f);
        mM = rotate(mM, radians(angle), axis);
    }
    mM = scale(mM, vec3(1.5f));
    //Pass uniform values to shaders
    mGLProgPtr->setUniformValue("PVM", toQt(mP * V * mM));
    mGLProgPtr->setUniformValue("VM", toQt(V * mM));
    //Since we are working in view space in fragment shader
    mGLProgPtr->setUniformValue("NormalMat", toQt(glm::inverse(glm::transpose(V * mM))));
    mGLProgPtr->setUniformValue("uAlpha", mAlpha);
    mVAO.bind();
    {
        for (int i = 0; i < mSeparators.size(); ++i) {
            MeshData sep = mSeparators[i];
            if (sep.specIndex == -1 || sep.diffuseIndex == -1) {
                //This mesh does not have specular texture
                //Do not render (Not with this shader at least)
                continue;
            }
            //Bind the texture pointer as texture unit 0
            mTextPtr[sep.diffuseIndex]->bind(0);
            mGLProgPtr->setUniformValue("uDiffuseMap", 0);
            //Bind the texture pointer as texture unit 0
            mTextPtr[sep.specIndex]->bind(1);
            mGLProgPtr->setUniformValue("uSpecularMap", 1);
            glDrawElementsBaseVertex(GL_TRIANGLES, sep.howMany, GL_UNSIGNED_INT,
                                     reinterpret_cast<void*>(sep.startIndex * int(sizeof(unsigned int))),
                                     sep.startVertex);
            mTextPtr[sep.specIndex]->release();
        }
    }
    mVAO.release();
    mGLProgPtr->release();
    ++mFrame;
    update();
    //Start a timer query
    glBeginQuery(GL_TIME_ELAPSED, mTimerQuery);
}

//Application specific (keys)
void MeshLoad::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Left:
            mAlpha = glm::clamp(mAlpha - 0.5f, 0.0f, 16.0f);
            event->accept();
        break;

        case Qt::Key_Right:
            mAlpha = glm::clamp(mAlpha + 0.5f, 0.0f, 16.0f);
            event->accept();
        break;

        case Qt::Key_R:
            mRotating = !mRotating;
            event->accept();
        break;

        default:
            //You did not handle it pass the event to parent
            BaseGLWindow::keyPressEvent(event);
        break;
    }
}

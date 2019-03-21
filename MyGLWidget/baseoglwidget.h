#ifndef BASEOGLWIDGET_H
#define BASEOGLWIDGET_H

#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <QtGui/QOpenGLDebugLogger>
#include <QtGui/QOpenGLDebugMessage>
#include <QMouseEvent>

#include "trackball.h"

#include <glm/glm.hpp>

class BaseOGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Core
{
    Q_OBJECT

public:
    explicit BaseOGLWidget(QWidget* parent = nullptr);
    ~BaseOGLWidget();
    //! get a string that contains the OpenGL context info.
    /*!
    The string format can be controlled by the rich text option
    */
    QString versionInfo();
    //!  Set the format for the Error messages and the context info string
    void richText(bool enable = false);
    //! Start logging OpenGL errors
    void startLog(int level = 0);
    //!  Stop logging OpenGL errors
    void stopLog();
    //!  Select which errors are reported, only high or also warnings
    void logLevel(int level = 0);

protected:
    bool mRichText;
    QOpenGLDebugLogger* mLogger;
    int mLogLevel;
    QString formatMsg(const QOpenGLDebugMessage& msg);

    glm::mat4 mP;
    glm::mat4 mV;
    glm::mat4 mM;
    float mFovY;
    float mNear;
    float mFar;

    Trackball mBall;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

protected slots:
    void messageLogged(const QOpenGLDebugMessage& msg);
signals:
    void newMessage(const QString& error);
};

//! Convert a 3D vector from GLM to a Qt.
const QVector3D toQt(const glm::vec3& v);
//! Convert a 4x4 matrix from GLM to a Qt.
const QMatrix4x4 toQt(const glm::mat4& m);
//! Convert a 4D vector from GLM to a Qt.
const QVector4D toQt(const glm::vec4& v);
//! Convert a 3x3 matrix from GLM to a Qt.
const QMatrix3x3 toQt(const glm::mat3& m);
//! Convert a quaternion from GLM to a Qt.
const QQuaternion toQt(const glm::quat& q);
//! Convert a 3D vector from Qt to GLM
const glm::vec3 toGLM(const QVector3D& v);
//! Convert a 4x4 matrix from Qt to GLM
const glm::mat4 toGLM(const QMatrix4x4& m);
//! Convert a 4D vector from Qt to GLM
const glm::vec4 toGLM(const QVector4D& v);
//! Convert a 3x3 matrix from Qt to GLM
const glm::mat3 toGLM(const QMatrix3x3& m);
//! Convert a quaternion from Qt to GLM
const glm::quat toGLM(const QQuaternion& q);
//! Convert a Qt color to a GLM 3D vector.
/*!
  The restulting vector is created where each component is represented with
  floating point values in the range [0,1] representing RGB components
*/
const glm::vec3 toGLM(const QColor& color);

#endif // BASEOGLWIDGET_H

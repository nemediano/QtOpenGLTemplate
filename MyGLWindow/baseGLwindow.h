#ifndef BASEGLWINDOW_H
#define BASEGLWINDOW_H

#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <QVector3D>
#include <QMatrix4x4>

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions_4_5_Core>
#include <QtGui/QOpenGLDebugLogger>
#include <QtGui/QOpenGLDebugMessage>
#include <QMouseEvent>

#include "trackball.h"
//!  A base class for a window that will be used to render OpenGL graphics
/*!
  This class should be used as a base class when you need a window to
  display OpenGL graphics. If you need a widget you sue the other tempate.
  You do not use this class directly, you need to inherit it.

  This class can initialize OpenGL functions, and can be queried for the
  version info. Additionally, manages an OpenGL debug logger which you can
  connect to a slot to get OpenGL errors. It also contains an Trackball camera
  already connected to be used with the mouse. Finally, defines some helper
  functions to convert between GLM and Qt data types.
*/
class BaseGLWindow : public QOpenGLWindow, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT

public:
    //! Only constructor
    /*!
      Should be called as an inline parameter in the derived class constructor(s)
    */
    BaseGLWindow();
    //! Only destuctor
    ~BaseGLWindow() override;
    //! Get a string that contains the OpenGL context info.
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
    /*!
    Only two options so far:
        0 - Only OpenGL errors
        1 - All: error, warnings, recommendations and informative messages
    */
    void logLevel(int level = 0);

protected:
    //!  If the error and the version info strings are returned as rich text or plain text
    bool mRichText;
    //!  OpenGL debug logger
    QOpenGLDebugLogger* mLogger;
    //!  Controls if the error logger reports all messages or just errors
    int mLogLevel;
    //!  Return an OpenGL error message formatted as a \class QString
    QString formatMsg(const QOpenGLDebugMessage& msg);
    //Model, View and Projection matrices.
    //Technically, I only need projection here because of trackball, but
    //I place them here to keep the code organized (all matrices in one place)
    glm::mat4 mP;
    glm::mat4 mV;
    glm::mat4 mM;
    //Rest of the needed parameters for the perspective projection
    float mFovY;
    float mNear;
    float mFar;
    //!  Trackball object to control the camera movement
    Trackball mBall;
    //To correct control the camera trackball we use this functions
    //!  To register the start of interaction with trackball
    void mousePressEvent(QMouseEvent* event) override;
    //!  To register the end of interaction with trackball
    void mouseReleaseEvent(QMouseEvent* event) override;
    //!  To update a continuous interaction with trackball
    void mouseMoveEvent(QMouseEvent* event) override;
    //!  To control camera fovy (which is zoom in and out)
    void wheelEvent(QWheelEvent* event) override;
    //!  To count the number of screenshot taken
    int mScreenShoots;
    //!  To interact wih keyboard.
    /*!
        Currentlly, exit application with esc and take screenshoot with space
    */
    void keyPressEvent(QKeyEvent* event) override;

protected slots:
    //!  To handle an incoming OpenGL errors
    void messageLogged(const QOpenGLDebugMessage& msg);
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

#endif //BASEGLWINDOW_H

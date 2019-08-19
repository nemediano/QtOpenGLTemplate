#include <QDebug>

#include "baseGLwindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Give default values for: the perspective projection parameters
// the log level is in only errors and the rich text is disabled
BaseGLWindow::BaseGLWindow() : mRichText(false), mLogLevel(0), mFovY(60.0f), mNear(0.1f),
mFar(1000.0f), mScreenShoots(0){

}

BaseGLWindow::~BaseGLWindow() {
    // If you have an active OpenGL debug error logger stop it
    stopLog();
    // If you ever had a logger destroy it
    if (mLogger) {
        delete mLogger;
    }
}

void BaseGLWindow::logLevel(int level) {
     // Only two log levels so far
    if (level >= 0 && level <= 1) {
        mLogLevel = level;
    }
}

void BaseGLWindow::richText(bool enable) {
    mRichText = enable;
}
/* We receive a message from the logger, we filter it (maybe it's a
 * message not that important). If we decide to react, we print to
 * console */
void BaseGLWindow::messageLogged(const QOpenGLDebugMessage& msg) {
    // Define the log levels
    if (mLogLevel == 0) {
        if (
            msg.severity() == QOpenGLDebugMessage::HighSeverity ||
            msg.severity() == QOpenGLDebugMessage::MediumSeverity ||
            msg.severity() == QOpenGLDebugMessage::LowSeverity
            ) {
            qDebug().noquote() << formatMsg(msg);
            qDebug().noquote() << msg.message();
        }
    } else {
        qDebug().noquote() << formatMsg(msg);
        qDebug().noquote() << msg.message();
    }

}
// Create an string from the actual \class QOpenGLDebugMessage object
// by taking into account your rich text option
QString BaseGLWindow::formatMsg(const QOpenGLDebugMessage& msg) {
    QString info{""};

    info += mRichText ? "<br>" : "\n";
    info += "Severity: ";
    switch (msg.severity()) {
        case QOpenGLDebugMessage::InvalidSeverity:
            info +=  mRichText ? "<b>Invalid</b>" : "Invalid";
        break;

        case QOpenGLDebugMessage::HighSeverity:
            info += mRichText ? "<b>High</b>" : "High";
        break;

        case QOpenGLDebugMessage::MediumSeverity:
            info += mRichText ? "<b>Medium</b>" : "Medium";
        break;

        case QOpenGLDebugMessage::LowSeverity:
            info += mRichText ? "<b>Low</b>" : "Low";
        break;

        case QOpenGLDebugMessage::NotificationSeverity:
            info += mRichText ? "<b>Notification</b>" : "Notification";
        break;

        case QOpenGLDebugMessage::AnySeverity:
        break;
    }
    info += mRichText ? "<br>" : "\n";

    info += "Source: ";
    switch (msg.source()) {
        case QOpenGLDebugMessage::InvalidSource:
            info += mRichText ? "<b>Invalid</b>" : "Invalid";
        break;

        case QOpenGLDebugMessage::APISource:
            info += mRichText ? "<b>API</b>" : "API";
        break;

        case QOpenGLDebugMessage::WindowSystemSource:
            info += mRichText ? "<b>Window System</b>" : "Window System";
        break;

        case QOpenGLDebugMessage::ShaderCompilerSource:
            info += mRichText ? "<b>Shader Compiler</b>" : "Shader Compiler";
        break;

        case QOpenGLDebugMessage::ThirdPartySource:
            info += mRichText ? "<b>Third party</b>" : "Third party";
        break;

        case QOpenGLDebugMessage::ApplicationSource:
            info += mRichText ? "<b>Application</b>" : "Application";
        break;

        case QOpenGLDebugMessage::OtherSource:
            info += mRichText ? "<b>Other</b>" : "Other";
        break;

        case QOpenGLDebugMessage::AnySource:
        break;
    }
    info += mRichText ? "<br>" : "\n";

    info += "Type: ";
    switch (msg.type()) {
        case QOpenGLDebugMessage::InvalidType:
            info += mRichText ? "<b>Invalid</b>" : "Invalid";
        break;

        case QOpenGLDebugMessage::ErrorType:
            info += mRichText ? "<b>Error</b>" : "Error";
        break;

        case QOpenGLDebugMessage::DeprecatedBehaviorType:
            info += mRichText ? "<b>Deprecated<br>" : "Deprecated";
        break;

        case QOpenGLDebugMessage::UndefinedBehaviorType:
            info += mRichText ? "<b>Undefined</b>" : "Undefined";
        break;

        case QOpenGLDebugMessage::PortabilityType:
            info += mRichText ? "<b>Portabillity</b>" : "Portabillity";
        break;

        case QOpenGLDebugMessage::PerformanceType:
            info += mRichText ? "<b>Performance</b>" : "Performnce";
        break;

        case QOpenGLDebugMessage::OtherType:
            info += mRichText ? "<b>Other</b>" : "Other";
        break;

        case QOpenGLDebugMessage::MarkerType:
            info += mRichText ? "<b>Marker</b>" : "Marker";
        break;

        case QOpenGLDebugMessage::GroupPushType:
            info += mRichText ? "<b>Group Push</b>" : "Group Push";
        break;

        case QOpenGLDebugMessage::GroupPopType:
            info += mRichText ? "<b>Group Pop</b>" : "Group Pop";
        break;

        case QOpenGLDebugMessage::AnyType:
        break;
    }

    return info;
}
// If the logger was ever created, stop it
void BaseGLWindow::stopLog() {
    if (mLogger) {
        mLogger->stopLogging();
    }
}
// Create the logger and activate it (start logging)
void BaseGLWindow::startLog(int level) {
    mLogger = new QOpenGLDebugLogger(this);
    if (mLogger->initialize()) {
        connect(mLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
                    this,   SLOT(messageLogged(QOpenGLDebugMessage)));
        mLogger->startLogging();
    }
    logLevel(level);
}

// Get an string with all the relevant version information
// Currently, your GLM version, your Qt version and your OpenGL version
// (from your actual context).
// Remember that even if your driver support certain version, you can
// ask for an older one. Also you can ask for a core or compatibility profile
QString BaseGLWindow::versionInfo() {
    // Get libraries version
    QString qtVersion{QT_VERSION_STR};
    QString glmVersion = QString::number(GLM_VERSION / 1000) + "." +
            QString::number(GLM_VERSION / 100) +
            "." + QString::number(GLM_VERSION % 100 / 10) +
            "." + QString::number(GLM_VERSION % 10);
    // Get OpenGL version Information form the actual current context
    QString glType{(context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL (Desktop)"};
    QString glVersion{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
    QString glRenderer{reinterpret_cast<const char*>(glGetString(GL_RENDERER))};
    QString glVendor{reinterpret_cast<const char*>(glGetString(GL_VENDOR))};
    // Get Profile Information
    QString glProfile;
    switch (context()->format().profile())	{
    case QSurfaceFormat::OpenGLContextProfile::CoreProfile:
        glProfile = "Core";
        break;

    case QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile:
        glProfile = "Compatibility";
        break;

    case QSurfaceFormat::OpenGLContextProfile::NoProfile:
    //default:
        glProfile = "unspecified";
        break;
    }
    // Construct the actual info string
    QString info;
    if (mRichText) {
        //info += "Using Qt version: <i>" + qtVersion + "</i><br>";
        info += "Using GLM version: <i>" + glmVersion + "</i><br>";
        info += "GPU: <b>" + glRenderer + "</b><br>";
        info += "Using <i>" + glType + "</i> in <b>" + glProfile + " profile</b><br>";
        info += "Version: <b>" + glVersion + "</b><br>";
        info += "Driver provided by: <i>" + glVendor + "</i>";
    } else {
        info += "Using GLM version: " + glmVersion + "\n";
        info += "Using Qt version: " + qtVersion + "\n";
        info += "GPU: " + glRenderer + "\n";
        info += "Using " + glType + " in " + glProfile + " profile\n";
        info += "Version: " + glVersion + "\n";
        info += "Driver provided by: " + glVendor + "\n";
    }
    return info;
}
// In order to use the track ball camera correctly we need to let him know when
// and where a dragging event started
void BaseGLWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        mBall.startDrag(glm::vec2(event->localPos().x(), event->localPos().y()));
        event->accept();
    }
}
// In order to use the track ball correctly we need to let him know when
// a dragging event finishes
void BaseGLWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        mBall.endDrag();
        event->accept();
    }
}
// We need to register when we are using (editing) the track ball camera's position
void BaseGLWindow::mouseMoveEvent(QMouseEvent* event) {
    mBall.drag(glm::vec2(event->localPos().x(), event->localPos().y()));
    event->accept();
}
// We use the wheel to zoom in/out by changing the camera's fovy (field of view along y axis)
void BaseGLWindow::wheelEvent(QWheelEvent* event) {
    QPoint numDegrees = event->angleDelta() / 16;
    mFovY += numDegrees.y();
    if (mFovY <= 10.0f) {
        mFovY = 10.0f;
    } else if (mFovY >= 170.0f) {
        mFovY = 170.0f;
    }

    mP = glm::perspective(glm::radians(mFovY), width() / float(height()), mNear, mFar);

    event->accept();
}

void BaseGLWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Escape:
            event->accept();
            this->close();
        break;

        case Qt::Key_Space:
        {
            QImage screenShoot = this->grabFramebuffer();
            QString fileName{"img_"};
            fileName += QString("%1").arg(mScreenShoots++, 4, 10, QChar('0'));
            fileName += ".png";
            screenShoot.save(fileName);
            event->accept();
        }
        break;

        case Qt::Key_F11:
        {
            if (windowState() != Qt::WindowFullScreen) {
                showFullScreen();
            } else {
                showNormal();
            }
        }
        break;

        default:
            //You did not handle it pass to parent
            QOpenGLWindow::keyPressEvent(event);
    }

}

const QVector3D toQt(const glm::vec3& v) {
    return QVector3D(v.x, v.y, v.z);
}
// Remember Qt and GLM are opposed in row or column order
const QMatrix4x4 toQt(const glm::mat4& m) {
    return QMatrix4x4(glm::value_ptr(glm::transpose(m)));
}

const QVector4D toQt(const glm::vec4& v) {
    return QVector4D(v.x, v.y, v.z, v.w);
}
// Remember Qt and GLM are opposed in row or column order
const QMatrix3x3 toQt(const glm::mat3& m) {
    return QMatrix3x3(glm::value_ptr(glm::transpose(m)));
}

const QQuaternion toQt(const glm::quat& q) {
    return QQuaternion(q.w, q.x, q.y, q.z);
}

const glm::vec3 toGLM(const QVector3D& v) {
    return glm::vec3(v.x(), v.y(), v.z());
}

const glm::vec3 toGLM(const QColor& color) {
    return glm::vec3(color.redF(), color.greenF(), color.blueF());
}
// Remember Qt and GLM are opposed in row or column order
const glm::mat4 toGLM(const QMatrix4x4& m) {
    return glm::transpose(glm::make_mat4(m.data()));
}

const glm::vec4 toGLM(const QVector4D& v) {
    return glm::vec4(v.x(), v.y(), v.z(), v.w());
}
// Remember Qt and GLM are opposed in row or column order
const glm::mat3 toGLM(const QMatrix3x3& m) {
    return glm::transpose(glm::make_mat3(m.data()));
}

const glm::quat toGLM(const QQuaternion& q) {
    return glm::quat(q.scalar(), q.x(), q.y(), q.z());
}

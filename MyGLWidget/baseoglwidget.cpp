#include <QDebug>

#include "baseoglwidget.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


BaseOGLWidget::BaseOGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    mRichText(false), mLogLevel(0), mFovY(60.0f), mNear(0.1f), mFar(1000.0f){
}

BaseOGLWidget::~BaseOGLWidget() {
    stopLog();
    if (mLogger) {
        delete mLogger;
    }
}

void BaseOGLWidget::logLevel(int level) {
    if (level >= 0 && level <= 1) {
        mLogLevel = level;
    }
}

void BaseOGLWidget::richText(bool enable) {
    mRichText = enable;
}

void BaseOGLWidget::messageLogged(const QOpenGLDebugMessage& msg) {
    if (mLogLevel == 0) {
        if (
            msg.severity() == QOpenGLDebugMessage::HighSeverity ||
            msg.severity() == QOpenGLDebugMessage::MediumSeverity ||
            msg.severity() == QOpenGLDebugMessage::LowSeverity
            ) {
            //qDebug().noquote() << formatMsg(msg);
            //qDebug().noquote() << msg.message();
            emit newMessage(formatMsg(msg));
        }
    } else {
        //qDebug().noquote() << formatMsg(msg);
        //qDebug().noquote() << msg.message();
        emit newMessage(formatMsg(msg));
    }

}

QString BaseOGLWidget::formatMsg(const QOpenGLDebugMessage& msg) {
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

void BaseOGLWidget::stopLog() {
    if (mLogger) {
        mLogger->stopLogging();
    }
}

void BaseOGLWidget::startLog(int level) {
    mLogger = new QOpenGLDebugLogger(this);
    if (mLogger->initialize()) {
        connect(mLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
                    this,   SLOT(messageLogged(QOpenGLDebugMessage)));
        mLogger->startLogging();
    }
    logLevel(level);
}

QString BaseOGLWidget::versionInfo() {

    QString glType;
    QString glVersion;
    QString glProfile;
    QString glRenderer;
    QString glVendor;
    QString qtVersion = QT_VERSION_STR;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL (Desktop)";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    // Get Profile Information

    switch (context()->format().profile())	{
    case QSurfaceFormat::OpenGLContextProfile::CoreProfile:
        glProfile = "Core";
        break;

    case QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile:
        glProfile = "Compatibility";
        break;

    case QSurfaceFormat::OpenGLContextProfile::NoProfile:
    default:
        glProfile = "unspecified";
        break;
    }

    QString info;
    if (mRichText) {
        //info += "Using Qt version: <i>" + qtVersion + "</i><br>";
        info += "GPU: <b>" + glRenderer + "</b><br>";
        info += "Using <i>" + glType + "</i> in <b>" + glProfile + " profile</b><br>";
        info += "Version: <b>" + glVersion + "</b><br>";
        info += "Driver provided by: <i>" + glVendor + "</i>";
    } else {
        info += "Using Qt version: " + qtVersion + "\n";
        info += "GPU: " + glRenderer + "\n";
        info += "Using " + glType + " in " + glProfile + " profile\n";
        info += "Version: " + glVersion + "\n";
        info += "Driver provided by: " + glVendor + "\n";
    }


    return info;
}

void BaseOGLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        mBall.startDrag(glm::vec2(event->localPos().x(), event->localPos().y()));
        event->accept();
    }
}

void BaseOGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        mBall.endDrag();
        event->accept();
    }
}

void BaseOGLWidget::mouseMoveEvent(QMouseEvent* event) {
    mBall.drag(glm::vec2(event->localPos().x(), event->localPos().y()));
    event->accept();
}

void BaseOGLWidget::wheelEvent(QWheelEvent* event) {
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

const QVector3D toQt(const glm::vec3& v) {
    return QVector3D(v.x, v.y, v.z);
}

const QMatrix4x4 toQt(const glm::mat4& m) {
    return QMatrix4x4(glm::value_ptr(glm::transpose(m)));
}

const QVector4D toQt(const glm::vec4& v) {
    return QVector4D(v.x, v.y, v.z, v.w);
}

const QMatrix3x3 toQt(const glm::mat3& m) {
    return QMatrix3x3(glm::value_ptr(glm::transpose(m)));
}

const QQuaternion toQt(const glm::quat& q) {
    return QQuaternion(q.x, q.y, q.z, q.w);
}

const glm::vec3 toGLM(const QVector3D& v) {
    return glm::vec3(v.x(), v.y(), v.z());
}

const glm::vec3 toGLM(const QColor& color) {
    return glm::vec3(color.redF(), color.greenF(), color.blueF());
}

const glm::mat4 toGLM(const QMatrix4x4& m) {
    return glm::transpose(glm::make_mat4(m.data()));
}

const glm::vec4 toGLM(const QVector4D& v) {
    return glm::vec4(v.x(), v.y(), v.z(), v.w());
}

const glm::mat3 toGLM(const QMatrix3x3& m) {
    return glm::transpose(glm::make_mat3(m.data()));
}

const glm::quat toGLM(const QQuaternion& q) {
    return glm::quat(q.scalar(), q.x(), q.y(), q.z());
}

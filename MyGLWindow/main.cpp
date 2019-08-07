#include "meshload.h"

#include <QSurfaceFormat>
#include <QtGui/QGuiApplication>
//!  Main function in Qt usual format.
/*!
  This is just the blueprint main function for any Qt class using OpenGL
  The only interesting part is that we use it to specify an very specific
  OpenGl context options
*/
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);

    MeshLoad window;
    window.setFormat(format);
    window.resize(640, 480);
    window.setTitle("Hierachical Mesh Loader");
    window.show();

    return app.exec();
}

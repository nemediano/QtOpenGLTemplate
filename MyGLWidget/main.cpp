#include <QObject>
#include <QApplication>
#include "mainwindow.h"

/*
 * Very standard main function for a Qt program
*/

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);

    MainWindow window;
    window.setWindowTitle(QObject::tr("OpenGL widget inside main window test"));
    window.resize(1200, 900);
    window.move(200, 100);
    window.show();

    return application.exec();
}

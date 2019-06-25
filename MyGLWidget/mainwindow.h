#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>

#include "testoglwidget.h"

//! Sample of a \class QMainWindow that contains an OpenGL render widget
/*!
  This class can be used as a template to be the main application window

  The window's UI contains the OpenGL render widget, a sidebar to place
  controls on it, menu bar and status bar.

  In terms of functionality, this class will receive keyboard input and
  has code to toggle the OpenGL widget to full screen, display OpenGL errors
  take screenshots, display OpenGL's and Qt version info and show/hide
  the UI menu.
*/

class MainWindow : public QMainWindow {
    Q_OBJECT

protected:
    //!  Pointer to the OpenGL widget
    TestOGLWidget* mViewerPtr;
    //!  Pointer to the side menu, to access the UI values
    QDockWidget* mSideMenuPtr;
    //!  Counter of screen shoots, to calculate the filename
    size_t mScrShts;
    //!  If the OpenGL widget is in fullscreen
    bool mIsMaximized;
    //!  Handle keyboard input
    void keyPressEvent(QKeyEvent* event) override;
    //!  Implement here global action that can be trigger.
    /*!
    Actions implemented here can be connected to be triggered by different inputs.
    For example, the screenshoot taking can be trigger by a button in the menu or
    by hot key combination.
    */
    void createActions();
    //!  Implement here the user interface that goes in the sidebar.
    /*!
    Create, and connect the UI that goes into the sidebar
    This class then, should be the bridge between the user interface and the
    render widget.
    */
    void createGUI();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

signals:

private slots:
    //!  grab the OpenGL frame buffer and save it as an image file.
    void takeScreenShoot();
    //!  Make (or return) the OpenGL widget in full screen mode.
    void toogleFullScreen();
    //!  Show/hide side bar menu.
    void toogleMenu();
    //!  Display a dialog box with the libraries version and context info
    void about();
    //!  Example of an interface with the OpenGL render widget
    void setRotation(bool rotate);
    //!  Display OpenGL error message in status bar
    void errorLog(const QString& error);
};

#endif // MAINWINDOW_H

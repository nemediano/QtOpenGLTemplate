#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>

#include "testoglwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

protected:
    TestOGLWidget* mViewerPtr;
    QDockWidget* mSideMenuPtr;
    size_t mScrShts;
    bool mIsMaximized;
    void keyPressEvent(QKeyEvent* event) override;
    void createActions();
    void createGUI();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void takeScreenShoot();
    void toogleFullScreen();
    void toogleMenu();
    void about();
    void setRotation(bool rotate);
    void errorLog(const QString& error);

};

#endif // MAINWINDOW_H

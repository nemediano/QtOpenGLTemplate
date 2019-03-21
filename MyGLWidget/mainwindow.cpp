#include <QObject>
#include <QSurfaceFormat>
#include <QtWidgets>
#include <QImage>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mScrShts(0) {
    QSurfaceFormat format;
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 4);
    format.setSwapInterval(1);
    QSurfaceFormat::setDefaultFormat(format);

    mViewerPtr = new TestOGLWidget();
    mViewerPtr->richText(false);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(mViewerPtr);
    setCentralWidget(mViewerPtr);
    mIsMaximized = false;

    //Connect error log
    connect(mViewerPtr, &TestOGLWidget::newMessage, this, &MainWindow::errorLog);

    createGUI();
    createActions();
}

MainWindow::~MainWindow() {
    delete mViewerPtr;
    delete mSideMenuPtr;
}

void MainWindow::takeScreenShoot() {
    QImage screenShoot = mViewerPtr->grabFramebuffer();
    QString fileName{"img_"};
    fileName += QString("%1").arg(mScrShts++, 4, 10, QChar('0'));
    fileName += ".png";
    screenShoot.save(fileName);
    this->statusBar()->showMessage(tr("Image saved as: ") + fileName);
}

void MainWindow::toogleFullScreen() {
    if (this->isFullScreen()) {
        mSideMenuPtr->show();
        this->menuBar()->show();
        this->showNormal();
    } else {
        mSideMenuPtr->hide();
        this->menuBar()->hide();
        this->statusBar()->hide();
        this->showFullScreen();
    }
}

void MainWindow::toogleMenu() {
    if (mSideMenuPtr->isHidden()) {
        mSideMenuPtr->show();
    } else {
        mSideMenuPtr->hide();
    }
}

void MainWindow::createActions() {

    QMenu *actionsMenu = menuBar()->addMenu(tr("&Actions"));
    const QIcon fullScreenIcon{":/icons/icons/full-screen.png"};
    QAction *fullAct = new QAction(fullScreenIcon, tr("&Full screen"), this);
    fullAct->setShortcuts(QKeySequence::FullScreen);
    fullAct->setStatusTip(tr("Go to full screen mode"));
    connect(fullAct, &QAction::triggered, this, &MainWindow::toogleFullScreen);
    actionsMenu->addAction(fullAct);

    const QIcon screenShootIcon{":/icons/icons/screen-shoot.png"};
    QAction *screenShootAct = new QAction(screenShootIcon, tr("Screen shoot"), this);
    screenShootAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    screenShootAct->setStatusTip(tr("Save image from the viewer window"));
    connect(screenShootAct, &QAction::triggered, this, &MainWindow::takeScreenShoot);
    actionsMenu->addAction(screenShootAct);

    const QIcon showMenuIcon{":/icons/icons/show-menu.png"};
    QAction *showMenuAct = new QAction(showMenuIcon, tr("Show/hide menu"), this);
    showMenuAct->setShortcut(QKeySequence(Qt::Key_M));
    showMenuAct->setStatusTip(tr("Show/hide menu"));
    connect(showMenuAct, &QAction::triggered, this, &MainWindow::toogleMenu);
    actionsMenu->addAction(showMenuAct);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction* aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::errorLog(const QString& error) {
    this->statusBar()->showMessage(error, 1500);
}

void MainWindow::createGUI() {
    mSideMenuPtr = new QDockWidget(tr("Options"), this);
    mSideMenuPtr->setAllowedAreas(Qt::LeftDockWidgetArea);

    QVBoxLayout* menuLayout = new QVBoxLayout();

    QCheckBox* checkbox = new QCheckBox(tr("&Rotate"));
    menuLayout->addWidget(checkbox);
    checkbox->setChecked(mViewerPtr->getRotation());

    connect(checkbox, SIGNAL(clicked(bool)), this, SLOT(setRotation(bool)));

    QSpacerItem* pusherTop = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    menuLayout->addSpacerItem(pusherTop);
    QGroupBox* box = new QGroupBox(tr("Menu"));
    box->setLayout(menuLayout);

    mSideMenuPtr->setWidget(box);
    mSideMenuPtr->setFixedWidth(300);

    addDockWidget(Qt::LeftDockWidgetArea, mSideMenuPtr);
}

void MainWindow::setRotation(bool rotate) {
    mViewerPtr->setRotation(rotate);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About OpenGL"), mViewerPtr->versionInfo());
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Escape:
            event->accept();
            this->close();
        break;

        default:
           //You did not handle it pass to parent
        QMainWindow::keyPressEvent(event);
    }
}

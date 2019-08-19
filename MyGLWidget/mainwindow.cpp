#include <QObject>
#include <QSurfaceFormat>
#include <QtWidgets>
#include <QImage>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mScrShts(0) {
    // This is the configuration for the OpenGL context
    QSurfaceFormat format;
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 5);
    format.setSwapInterval(1);
    QSurfaceFormat::setDefaultFormat(format);
    // once you have a default context, create an OpenGL widget
    mViewerPtr = new TestOGLWidget();
    mViewerPtr->richText(false);
    // place the widget inside the main window
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(mViewerPtr);
    setCentralWidget(mViewerPtr);
    mIsMaximized = false; // we start in non full-screen size
    // Connect the error log
    connect(mViewerPtr, &TestOGLWidget::newMessage, this, &MainWindow::errorLog);

    createGUI();
    createActions();
}

MainWindow::~MainWindow() {
    delete mViewerPtr;
    delete mSideMenuPtr;
}
// Capture the OpenGL widget's framebuffer as an PNG image
void MainWindow::takeScreenShoot() {
    // Grab image
    QImage screenShoot = mViewerPtr->grabFramebuffer();
    // Calculate filename
    QString fileName{"img_"};
    fileName += QString("%1").arg(mScrShts++, 4, 10, QChar('0'));
    fileName += ".png";
    // Save image as a file
    screenShoot.save(fileName);
    // Show the filename in the status bar
    this->statusBar()->showMessage(tr("Image saved as: ") + fileName);
}
// Toggle between full screen and normal mode
void MainWindow::toogleFullScreen() {
    if (this->isFullScreen()) { // Go to normal mode
        this->showNormal();
        mSideMenuPtr->show();
        this->menuBar()->show();
    } else { // Go to full screen mode
        // First hide the elements except for the OpenGL widget
        mSideMenuPtr->hide();
        this->menuBar()->hide();
        this->statusBar()->hide();
        // Then make the main window go into full screen mode
        this->showFullScreen();
    }
}
// Menu side bar hide/show
void MainWindow::toogleMenu() {
    if (mSideMenuPtr->isHidden()) {
        mSideMenuPtr->show();
    } else {
        mSideMenuPtr->hide();
    }
}
// Create the items in the main menu
void MainWindow::createActions() {
    // Create sub menu
    QMenu *actionsMenu = menuBar()->addMenu(tr("&Actions"));
    // Create Full-screen action and add to menu
    const QIcon fullScreenIcon{":/icons/icons/full-screen.png"};
    QAction *fullAct = new QAction(fullScreenIcon, tr("&Full screen"), this);
    fullAct->setShortcuts(QKeySequence::FullScreen);
    fullAct->setStatusTip(tr("Go to full screen mode"));
    // Connect it to a function
    connect(fullAct, &QAction::triggered, this, &MainWindow::toogleFullScreen);
    connect(mViewerPtr, SIGNAL(toggleFullScreen), this, SLOT(toogleFullScreen));
    actionsMenu->addAction(fullAct);

    // Create take screen-shoot action and add to menu
    const QIcon screenShootIcon{":/icons/icons/screen-shoot.png"};
    QAction *screenShootAct = new QAction(screenShootIcon, tr("Screen shoot"), this);
    screenShootAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    screenShootAct->setStatusTip(tr("Save image from the viewer window"));
    // Connect it to a function
    connect(screenShootAct, &QAction::triggered, this, &MainWindow::takeScreenShoot);
    actionsMenu->addAction(screenShootAct);

    // Create show/hide side menu action and add to menu
    const QIcon showMenuIcon{":/icons/icons/show-menu.png"};
    QAction *showMenuAct = new QAction(showMenuIcon, tr("Show/hide menu"), this);
    showMenuAct->setShortcut(QKeySequence(Qt::Key_M));
    showMenuAct->setStatusTip(tr("Show/hide menu"));
    // Connect it to a function
    connect(showMenuAct, &QAction::triggered, this, &MainWindow::toogleMenu);
    actionsMenu->addAction(showMenuAct);

    // Create sub menu
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    // Create show version info action and add to menu
    QAction* aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    // Create show Qt version action and add to menu
    QAction* aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}
// This is the slot connected to the OpenGL error log signal
void MainWindow::errorLog(const QString& error) {
    // Take the formatted error msg and display it for 1.5s on the status bar
    this->statusBar()->showMessage(error, 1500);
}
// Create the side bar menu
// All the object here communicate with the MainWindow object (this object), which serves as a bridge
// therefore we communicate with the OpenGL widget
void MainWindow::createGUI() {
    // Create widget
    mSideMenuPtr = new QDockWidget(tr("Options"), this);
    mSideMenuPtr->setAllowedAreas(Qt::LeftDockWidgetArea);
    // Create layout, we will add the controls to this layout
    QVBoxLayout* menuLayout = new QVBoxLayout();

    // create sample control (checkbox to control rotation)
    QCheckBox* checkbox = new QCheckBox(tr("&Rotate"));
    menuLayout->addWidget(checkbox);
    checkbox->setChecked(mViewerPtr->getRotation());
    // Connect it with our slot
    connect(checkbox, SIGNAL(clicked(bool)), this, SLOT(setRotation(bool)));

    // Spacer to push control to the top of widget
    QSpacerItem* pusherTop = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    menuLayout->addSpacerItem(pusherTop);
    QGroupBox* box = new QGroupBox(tr("Menu"));
    box->setLayout(menuLayout);
    // Add all to actual widget
    mSideMenuPtr->setWidget(box);
    mSideMenuPtr->setFixedWidth(300);
    addDockWidget(Qt::LeftDockWidgetArea, mSideMenuPtr);
}

// Here we communicate with the OpenGL widget
void MainWindow::setRotation(bool rotate) {
    mViewerPtr->setRotation(rotate);
}

// Information dialog box
void MainWindow::about() {
    // Ask the OpenGL widget about his current context info, present it in a dialog box
    QMessageBox::about(this, tr("About OpenGL"), mViewerPtr->versionInfo());
}

// Handle key events press
void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
        case Qt::Key_Escape:
            event->accept();
            this->close();
        break;

        case Qt::Key_F11:
            event->accept();
            this->toogleFullScreen();
        break;

        case Qt::Key_S:
            event->accept();
            this->takeScreenShoot();
        break;

        default:
        //You did not handle the event pass it to parent
        QMainWindow::keyPressEvent(event);
    }
}

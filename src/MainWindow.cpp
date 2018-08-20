/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "MainWindow.h"
#include <QDir>
#include <QDesktopWidget>
#include <QMessageBox>
#include "UI.h"

MainWindow::MainWindow(QMainWindow* parent, Qt::WFlags fl, Data*     data)
    : QMainWindow( parent, fl )
    , _data(data)
{
    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width  = widget->width();
    int desktop_height = widget->height();
    int x = desktop_width / 2  - 550;
    int y = desktop_height / 2 - 400;
    move(QPoint(x, y));

    setState(SCENARIO_DEFAULT);

    ////////////////////////////////////////////////////////////////////////
    //              MENU BAR
    ////////////////////////////////////////////////////////////////////////
    QMenuBar* menuBar = new QMenuBar(this);

    QMenu* menuFile    = new QMenu("File");
    QMenu* menuDisplay = new QMenu("Display");
    QMenu* menuAbout   = new QMenu("About");

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuDisplay);
    menuBar->addMenu(menuAbout);

    menuFile->addAction("Open Heightmap", this, SLOT(slot_OpenHeightmap()));
    menuFile->addAction("Save Village", this, SLOT(slot_SaveVillage()));
    menuFile->addAction("Load Village", this, SLOT(slot_LoadVillage()));
    menuFile->addAction("Export Geometry", this, SLOT(slot_ExportGeometry()));
    menuFile->addSeparator();
    menuFile->addAction("Exit", this, SLOT(close()) );

    menuDisplay->addAction("Enables/Disables WireFrame", this, SLOT(slot_WireFrame()));
    menuDisplay->addSeparator ();
    menuDisplay->addAction("Normal", this, SLOT(slot_display0()));
    menuDisplay->addAction("IGN", this, SLOT(slot_display1()));
    menuDisplay->addAction("GRID", this, SLOT(slot_display2()));
    /*
    menuDisplay->addSeparator ();
           menuDisplay->addAction("Global Interest", this, SLOT(slot_display3()));
           menuDisplay->addAction("Social Interest", this, SLOT(slot_display4()));
           menuDisplay->addAction("Roads Interest", this, SLOT(slot_display5()));
           menuDisplay->addAction("Domination Interest", this, SLOT(slot_display6()));
           menuDisplay->addAction("Altitude Interest", this, SLOT(slot_display7()));
           menuDisplay->addAction("Sun Interest", this, SLOT(slot_display8()));
           menuDisplay->addAction("Walls Interest", this, SLOT(slot_display9()));
           menuDisplay->addAction("Water Interest", this, SLOT(slot_display10()));
           menuDisplay->addAction("Cult Interest", this, SLOT(slot_display11()));
           */

    menuAbout->addAction("About us", this, SLOT(slot_about()));

    setMenuBar(menuBar);


    ////////////////////////////////////////////////////////////////////////
    //              DOCK WIDGET
    ////////////////////////////////////////////////////////////////////////
    QHBoxLayout *layout = new QHBoxLayout;
    QWidget* dock = new QWidget;
    dock->setLayout(layout);
    setCentralWidget(dock);

    ////////////////////////////////////////////////////////////////////////
    //              GL WIDGET
    ////////////////////////////////////////////////////////////////////////


    QGLFormat qglFormat;
    qglFormat.setVersion(3,1);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    qglFormat.setSampleBuffers(true);


    _GL_Widget = new GL_Widget(qglFormat);
    layout->addWidget(_GL_Widget);

    ////////////////////////////////////////////////////////////////////////
    //              UI WIDGET
    ////////////////////////////////////////////////////////////////////////
    _ui = new UI(data);
    _ui->_GL_Widget = _GL_Widget;
    _ui->_window = this;
    layout->addWidget(_ui);


    layout->setSpacing(0);

}

MainWindow::~MainWindow()
{
}


void MainWindow::keyPressEvent (QKeyEvent * e)
{
    _GL_Widget->keyPressEvent(e);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    _GL_Widget->mousePressEvent(e);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    _GL_Widget->mouseReleaseEvent(e);
}
void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    _GL_Widget->mouseDoubleClickEvent(e);
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    _GL_Widget->mouseMoveEvent(e);
}

QString MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this,
                   "Choose a file to open",
                   QString::null,
                   QString::null);
    QDir dir(QDir::currentPath());
    return  QString("./")+dir.relativeFilePath(file);

}

void MainWindow::slot_WireFrame()
{
    _GL_Widget->togglewireframe();
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display0()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::NORMAL);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}


void MainWindow::slot_display1()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::IGN);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display2()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::GRID);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display3()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::GLOBAL_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display4()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::SOCIAL_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display5()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::ROADS_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display6()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::DOMINATION_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display7()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::ALTITUDE_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display8()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::SUN_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display9()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::FORTIFICATION_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display10()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::WATER_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_display11()
{
    _GL_Widget->_renderer->setDisplayMode(Renderer::CULT_INTEREST);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_OpenHeightmap()
{
    QString file;
    file = openFile();
    std::cout << "File : " << file.toStdString().c_str() << std::endl;
    if (file != "./")
    {
        std::cout << "DATA EMPTY : " << std::endl;
        _data->empty();
        std::cout << "DATA INIT : " << std::endl;
        _data->initialize(file);
        std::cout << "RENDERER INIT : " << std::endl;
        _GL_Widget->_renderer->initData(_data);
        std::cout << "PAINT : " << std::endl;
        _GL_Widget->paintGL();
        _GL_Widget->updateGL();
    }
}

void MainWindow::slot_about()
{
    QMessageBox::about(this,
                       "About",
                       "Procedural Generation of Villages.\n"\
                       "Developped by Arnaud Emilien.\n");
}


void MainWindow::setState(State state)
{
    _state = state;
    std::cout << "State : " << state << std::endl;
}

void MainWindow::slot_LoadVillage()
{
    QString file;
    file = openFile();

    if (file != "./")
    {
        _data->system->load(file.toStdString());
    }
    _GL_Widget->_renderer->updateTextures();
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}

void MainWindow::slot_SaveVillage()
{
    QString file;
    file = QFileDialog::getSaveFileName(this,
                                        tr("Save as..."),
                                        QString(),
                                        tr("All Files (*)"));
    if (! file.isEmpty())
    {
        _data->system->save(file.toStdString());
    }
}

void MainWindow::slot_ExportGeometry()
{
    QString file;
    file = QFileDialog::getSaveFileName(this,
                                        tr("Save as..."),
                                        QString(),
                                        tr("All Files (*)"));
    if (! file.isEmpty())
    {
        _data->system->exportGeometry(file.toStdString());
    }
}

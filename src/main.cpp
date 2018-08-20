/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "MainWindow.h"
#include "UI.h"

#include <QApplication>
#include <iostream>


int main(int argc, char** argv)
{
    QApplication application(argc,argv);

    // ---------------------------------------------------------------------
    // DATA
    // ---------------------------------------------------------------------
    Data* data = new Data();
    data->initialize();


    // ---------------------------------------------------------------------
    // VIEWER
    // ---------------------------------------------------------------------
    MainWindow window(0, Qt::Window, data);
    window.setWindowTitle("Procedural Village Generation");
    window.resize(1200,900);
    window.show();


    // ---------------------------------------------------------------------
    // RENDERER
    // ---------------------------------------------------------------------
    Renderer* renderer = new Renderer();
    renderer->init();
    window._GL_Widget->_data = data;
    window._GL_Widget->_renderer = renderer;
    window._ui->_renderer = renderer;
    window._data = data;

    window._GL_Widget->_renderer->initData(data);
    std::cout << "Begin PaintGL" << std::endl;
    window._GL_Widget->paintGL();
    std::cout << "End PaintGL" << std::endl;
    window._GL_Widget->updateGL();
    std::cout << "End updateGL" << std::endl;

    renderer->_ui = window._ui;
    renderer->_GL_Widget = window._GL_Widget;


    return application.exec();

}


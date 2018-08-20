/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _MAIN_WINDOW__H
#define _MAIN_WINDOW__H

#include <GL/glew.h>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QFileDialog>
#include <QString>
#include "GL_Widget.h"
#include "Data.h"

class UI;

class MainWindow : public QMainWindow
{

    Q_OBJECT

public :
    UI*        _ui;
    GL_Widget* _GL_Widget;
    Data*      _data;


    enum State
    {
        SCENARIO_DEFAULT,

        ATLAS_DEFAULT,

        VILLAGE_DEFAULT,
        ROADS_DEFAULT,
        BUILDINGS_DEFAULT,

        GEOMETRY_DEFAULT,

        LANDSCAPE_DEFAULT,
        LANDSCAPE_PAINT_DEFAULT,

        GENERATION_DEFAULT
    };

    State _state;

public :
    MainWindow(QMainWindow* parent = 0, Qt::WFlags fl = Qt::Window, Data* data = NULL);
    ~MainWindow();
    void keyPressEvent (QKeyEvent * e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    QString openFile();

public slots:
    void slot_display0();

    void slot_display1();
    void slot_display2();
    void slot_display3();

    void slot_display4();
    void slot_display5();
    void slot_display6();
    void slot_display7();
    void slot_display8();
    void slot_display9();
    void slot_display10();
    void slot_display11();

    void slot_WireFrame();
    void slot_OpenHeightmap();

    void slot_about();

    void slot_SaveVillage();
    void slot_LoadVillage();

    void slot_ExportGeometry();


    void setState(State state);

}; // class MainWindow

#endif // _MAIN_WINDOW__H

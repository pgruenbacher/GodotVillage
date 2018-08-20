/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GL_WIDGET_H
#define _GL_WIDGET_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <QGLWidget>
#include <QKeyEvent>
#include <QLineEdit>
#include "Data.h"
#include "Renderer.h"
#include "./Engine/Node.h"
#include "./Engine/Camera.h"
#include "./Engine/Mesh.h"
#include "./Engine/Ray.h"

class GL_Widget : public QGLWidget
{
private :
    Q_OBJECT

private :
    bool      _cameraMouse;
    bool      _cameraMouse2;
    bool      _cameraMouse3;

public :
    enum State {Navigation,
                Painting,
                Picking
               };
    Image* toPaint;
    float   paintRadius;
    float   paintCoeff;
    QLineEdit   *pickEx;
    QLineEdit   *pickEy;
public :
    State _state;
    int       _lastX;
    int       _lastY;
    Vector3F  _intersection;

    bool wireframe;

public :
    Renderer* _renderer;
    Data*     _data;

public :
    GL_Widget(const QGLFormat& format, QWidget *parent=0);
    ~GL_Widget();
    void paintGL();
    void initializeGL();
    void togglewireframe();
    void resizeGL(int w, int h);

    void fpsCorrection();

    void keyPressEvent (QKeyEvent * e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    Ray ConvertPixelToRay(const Vector2I& pixel);

}; // class MainWindow
#endif // _GL_WIDGET_H

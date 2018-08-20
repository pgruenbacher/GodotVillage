/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "GL_Widget.h"
#include "./Image/Painter.h"
#include "./Image/Painter.h"
#include <iostream>


GL_Widget::GL_Widget(const QGLFormat& format, QWidget *parent) : QGLWidget(format,parent)
{
    _cameraMouse  = false;
    _cameraMouse2 = false;
    _cameraMouse3 = false;
    _lastX        = 0;
    _lastY        = 0;
    wireframe     = false;
    _state        = Navigation;
    toPaint       = NULL;
    paintRadius   = 50.0;
    paintCoeff    = 0.1;
}

GL_Widget::~GL_Widget()
{
}

void GL_Widget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    _renderer->draw();
}



void GL_Widget::togglewireframe()
{
    wireframe = ! wireframe;
    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    updateGL();
}

void  GL_Widget::initializeGL()
{
    // GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Glew not ok ! " << std::endl;
    }
    else
    {
        std::cout << "Glew initialized. " << std::endl;
    }

    // Extensions
    std::cout << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion() << std::endl;
    std::cout << "Context valid: " << context()->isValid() << std::endl;
    std::cout  << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion() << std::endl;
    std::cout  << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR) << std::endl;
    std::cout  << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER) << std::endl;
    std::cout << "                    VERSION:      " << (const char*)glGetString(GL_VERSION) << std::endl;
    std::cout  << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    //std::cout  << "                    Extensions: " << (const char*)glGetString(GL_EXTENSIONS) << std::endl;


    // OPENGL
    glClearColor (0.0f, 0.6f, 0.9f, 1.0);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glEnable (GL_LINE_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    GLfloat light_position0[] = { 1.0F,1.0F,1.0F,1.0F };
    GLfloat light_position1[] = { 1.0F,1.0F,1.0F,1.0F };
    GLfloat light_color[] = { 1.0F,1.0F,1.0F,1.0F };
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_color);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_color);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_color);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_color);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
    glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_AUTO_NORMAL);

    resizeGL(800,600);
}

void GL_Widget::fpsCorrection()
{
    Camera* camera = _renderer->_cam;
    if (!camera)
        return;

    Vector3F position;

    position = camera->getNode()->getPosition();
    if (_data)
    {
        if (_data->system)
        {
            float height = 0.0;
            //height = 1.0 * _data->system->_heightmapLayout->getValueAt(Vector2I(position.x, 512 + position.z));
            height = Painter::getGaussian(Vector2I(position.x, 512 + position.z),
                                          1,
                                          _data->system->_heightmapLayout->getImage()).x;
            position.y = 1.8f + height / 255.0 * 500.0;
        }
    }
    camera->getNode()->setPosition(position);
}

void GL_Widget::resizeGL( int w, int h )
{

    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 15000.0);
    glMatrixMode( GL_MODELVIEW );

}


void  GL_Widget::keyPressEvent (QKeyEvent * e)
{

    Camera* camera = _renderer->_cam;
    if (!camera)
        return;

    switch (e->key())
    {




    case Qt::Key_A :
        camera->getNode()->moveLeft(1.0);
        fpsCorrection();
        break;
    case Qt::Key_E :
        camera->getNode()->moveLeft(-1.0);
        fpsCorrection();
        break;
    case Qt::Key_Z :
        fpsCorrection();
        camera->getNode()->moveForward(1.0);
        break;
    case Qt::Key_S :
        camera->getNode()->moveForward(-1.0);
        fpsCorrection();
        break;


    case Qt::Key_Q :
        camera->getNode()->rotate(Quaternion::createFromAxisAndAngle(0.15, Vector3F::YAXIS));
        break;
    case Qt::Key_D :
        camera->getNode()->rotate(Quaternion::createFromAxisAndAngle(-0.15, Vector3F::YAXIS));
        break;



    case Qt::Key_PageUp :
        camera->getNode()->translate(Vector3F::YAXIS *30.f);
        break;
    case Qt::Key_PageDown :
        camera->getNode()->translate(Vector3F::YAXIS * (-30.f) );
        break;
    case Qt::Key_Plus :
        camera->getNode()->rotateAroundLeft(-0.05);
        break;
    case Qt::Key_Minus :
        camera->getNode()->rotateAroundLeft(0.05);
        break;

    case Qt::Key_F12 :
        _renderer->reloadShaders();
        break;

    default:
        QGLWidget::keyPressEvent(e);
    }

    updateGL();

}

Ray GL_Widget::ConvertPixelToRay(const Vector2I& pixel)
{
    GLint viewport[4];
    // OpenGL matrixes
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;
    GLdouble wx, wy, wz;

    glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    realy=viewport[3]- (GLint) pixel.y- 1;
    gluUnProject ((GLdouble) pixel.x, (GLdouble) realy, 1.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    Vector3F end = Vector3F(wx,wy,wz);

    gluUnProject ((GLdouble) pixel.x, (GLdouble) realy, 0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    Vector3F start = Vector3F(wx,wy,wz);

    Ray ray;
    ray._position  = start;
    ray._direction = normalize(end - start);
    return ray;
}

void GL_Widget::mousePressEvent(QMouseEvent *e)
{

    _lastX = e->x();
    _lastY = e->y();

    switch(_state)
    {
    case Navigation :
    {
        if (e->button() == Qt::LeftButton)
            _cameraMouse = true;

        if (e->button() == Qt::RightButton)
            _cameraMouse2 = true;

        if (e->button() == Qt::MidButton)
            _cameraMouse3 = true;
    }
    break;
    case Painting :
    {
        Camera* camera = _renderer->_cam;
        if (!camera)
            return;
        Ray ray = ConvertPixelToRay(Vector2I(_lastX, _lastY));


        float distance = ray.distFromASurface(_data->system->_terrain->getNode().getPosition(),
                                              Vector3F(0,1,0));
        _intersection = ray.intersectionWithASurface(_data->system->_terrain->getNode().getPosition(),
                        Vector3F(0,1,0));

        if (toPaint == NULL)
            return;

        Vector3F color;
        if (toPaint->getFormat() == Color::RGB_32)
        {
            color = Vector3F(1,1,1);
        }
        else
        {
            color = Vector3F(255,255,255);
        }
        Painter::drawGaussian(Vector2I(_intersection.x, -_intersection.z),
                              paintRadius,
                              toPaint,
                              color,
                              paintCoeff);

        if (toPaint == _data->system->_terrain->getHeightMap())
        {
            _renderer->updateTextures(7);
        }
        else if (toPaint == _data->system->_terrain->getWater())
        {
            _renderer->updateTextures(0);
        }
        else if (toPaint == _data->system->_terrain->getVegetation())
        {
            _renderer->updateTextures(1);
        }
        else if (toPaint == _data->system->_terrain->getRoads())
        {
            _renderer->updateTextures(2);
        }
        else if (toPaint == _data->system->_terrain->getRock())
        {
            _renderer->updateTextures(3);
        }
        else if (toPaint == _data->system->_terrain->getSnow())
        {
            _renderer->updateTextures(4);
        }
        else if (toPaint == _data->system->_buildingsLayout->getImage())
        {
            _renderer->updateTextures(5);
        }
        else if (toPaint == _data->system->_wallsLayout->getImage())
        {
            _renderer->updateTextures(6);
        }
    }
    break;

    case Picking :
    {
        Camera* camera = _renderer->_cam;
        if (!camera)
            return;
        Ray ray = ConvertPixelToRay(Vector2I(_lastX, _lastY));

        float distance = ray.distFromASurface(_data->system->_terrain->getNode().getPosition(),
                                              Vector3F(0,1,0));
        _intersection = ray.intersectionWithASurface(_data->system->_terrain->getNode().getPosition(),
                        Vector3F(0,1,0));

        pickEx->clear();
        pickEy->clear();
        pickEx->insert(QString().setNum(int(_intersection.x)));
        pickEy->insert(QString().setNum(int( - _intersection.z)));
    }
    break;
    }
    updateGL();
}

void GL_Widget::mouseReleaseEvent(QMouseEvent *e)
{

    _cameraMouse  = false;
    _cameraMouse2 = false;
    _cameraMouse3 = false;
}

void GL_Widget::mouseDoubleClickEvent(QMouseEvent *e)
{
}

void GL_Widget::mouseMoveEvent(QMouseEvent *e)
{

    Camera* camera = _renderer->_cam;
    if (!camera)
        return;

    int dx = e->x() - _lastX;
    int dy = e->y() - _lastY;
    _lastX = e->x();
    _lastY = e->y();

    switch(_state)
    {
    case Navigation :
    {
        if(_cameraMouse)
        {
            camera->getNode()->rotate(Quaternion::createFromAxisAndAngle(dx*M_PI/180.0/15.0, Vector3F::YAXIS));
            camera->getNode()->rotateAroundLeft(float(-dy*M_PI/180.0/15.0));
        }
        if(_cameraMouse2)
        {
            camera->getNode()->moveUp(float(dy*1.0));
            camera->getNode()->moveLeft(float(dx*1.0));
        }
        if(_cameraMouse3)
        {
            camera->getNode()->moveForward(float(dy*1.0));
        }
    }
    break;
    case Picking :
    {
        Camera* camera = _renderer->_cam;
        if (!camera)
            return;
        Ray ray = ConvertPixelToRay(Vector2I(_lastX, _lastY));

        float distance = ray.distFromASurface(_data->system->_terrain->getNode().getPosition(),
                                              Vector3F(0,1,0));
        _intersection = ray.intersectionWithASurface(_data->system->_terrain->getNode().getPosition(),
                        Vector3F(0,1,0));

        pickEx->clear();
        pickEy->clear();
        pickEx->insert(QString().setNum(int(_intersection.x)));
        pickEy->insert(QString().setNum(int(- _intersection.z)));
    }
    break;
    case Painting :
    {
        Camera* camera = _renderer->_cam;
        if (!camera)
            return;
        Ray ray = ConvertPixelToRay(Vector2I(_lastX, _lastY));


        float distance = ray.distFromASurface(_data->system->_terrain->getNode().getPosition(),
                                              Vector3F(0,1,0));
        _intersection = ray.intersectionWithASurface(_data->system->_terrain->getNode().getPosition(),
                        Vector3F(0,1,0));
        if (toPaint == NULL)
            return;

        Vector3F color;
        if (toPaint->getFormat() == Color::RGB_32)
        {
            color = Vector3F(1,1,1);
        }
        else
        {
            color = Vector3F(255,255,255);
        }
        Painter::drawGaussian(Vector2I(_intersection.x, -_intersection.z),
                              paintRadius,
                              toPaint,
                              color,
                              paintCoeff);

        if (toPaint == _data->system->_terrain->getHeightMap())
        {
            _renderer->updateTextures(7);
        }
        else if (toPaint == _data->system->_terrain->getWater())
        {
            _renderer->updateTextures(0);
        }
        else if (toPaint == _data->system->_terrain->getVegetation())
        {
            _renderer->updateTextures(1);
        }
        else if (toPaint == _data->system->_terrain->getRoads())
        {
            _renderer->updateTextures(2);
        }
        else if (toPaint == _data->system->_terrain->getRock())
        {
            _renderer->updateTextures(3);
        }
        else if (toPaint == _data->system->_terrain->getSnow())
        {
            _renderer->updateTextures(4);
        }
        else if (toPaint == _data->system->_buildingsLayout->getImage())
        {
            _renderer->updateTextures(5);
        }
        else if (toPaint == _data->system->_wallsLayout->getImage())
        {
            _renderer->updateTextures(6);
        }


    }
    break;

    }
    updateGL();
}



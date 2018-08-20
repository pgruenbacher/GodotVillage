/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Texture2D.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <QGLWidget>


Texture2D::Texture2D(const std::string& name, QImage* image)
    : _image(image)
    , _id(0)
{
    this->_name = name;
}

QImage* Texture2D::getImage()
{
    return _image;
}

void Texture2D::_bind()
{
    glBindTexture(GL_TEXTURE_2D, _id);
}
void Texture2D::_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(unsigned int pos)
{
    // Check
    if (_id == 0)
        _loadInCg();

    // Check2
    if (pos >= 8)
        return;

    // Enable
    glEnable(GL_TEXTURE_2D);

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Bind
    _bind();

    // Filter, Param

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}

void Texture2D::unbind(unsigned int pos)
{
    // Check
    if (pos >= 8)
        return;

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Unbind
    _unbind();
    glDisable(GL_TEXTURE_2D);
}

void Texture2D::update()
{
    if (_id == 0)
        _loadInCg();
    else
        _updateInCg();
}

void Texture2D::_updateInCg()
{
    // Check
    if (_id == 0)
        return;

    // Check2
    if (_image->width()<= 0 || _image->height() <= 0)
        return;

    // Bind
    _bind();

    // Load
    QImage t = QGLWidget::convertToGLFormat(*_image);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 3,
                 t.width(),
                 t.height(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 t.bits() );
    // Unbind
    _unbind();
}

void Texture2D::_loadInCg()
{
    // Check
    if (_id != 0)
        return;

    // Gen
    glGenTextures( 1, &_id);

    // Load
    _updateInCg();
}

void Texture2D::remove()
{
    // Check
    if (_id == 0)
        return;

    // Remove
    glDeleteTextures( 1, &_id);
}

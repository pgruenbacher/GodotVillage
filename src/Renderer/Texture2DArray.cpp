/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Texture2DArray.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <QGLWidget>


Texture2DArray::Texture2DArray(const std::string& name, const Table<Image*>& images, bool contains)
    : _images(images)
    , _id(0)
    , _contains(contains)
{
    this->_name = name;
}

Table<Image*>& Texture2DArray::getImages()
{
    return _images;
}

void Texture2DArray::_bind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
}
void Texture2DArray::_unbind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture2DArray::bind(unsigned int pos)
{
    // Check
    if (_id == 0)
        _loadInCg();

    // Check2
    if (pos >= 8)
        return;

    // Enable
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_TEXTURE_3D);

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Bind
    _bind();

    // Filter, Param
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

void Texture2DArray::unbind(unsigned int pos)
{
    // Check
    if (pos >= 8)
        return;

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Bind
    _unbind();
    //glDisable(GL_TEXTURE_3D);
    glDisable(GL_TEXTURE_2D);
}

void Texture2DArray::update(unsigned int index)
{
    if (_id == 0)
        _loadInCg();
    else
        _updateInCg(index);
}

void Texture2DArray::update()
{
    if (_id == 0)
        _loadInCg();
    else
        _updateInCg();
}

void Texture2DArray::_updateInCg()
{
    // Bind
    _bind();

    // Check2
    if (_images[0]->getWidth()<= 0 || _images[0]->getHeight() <= 0)
        return;

    // Prepare
    int formatIntern;
    int formatGL;
    int formatData;
    switch (_images[0]->getFormat())
    {
    case Color::RGB_32 :
        formatIntern = GL_RGB32F_ARB;
        formatGL     = GL_RGB;
        formatData   = GL_FLOAT;
        break;
    default :
        formatIntern = GL_RGB;
        formatGL     = GL_RGB;
        formatData   = GL_UNSIGNED_BYTE;
        break;
    }

    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,
                 formatIntern,
                 _images[0]->getWidth(),
                 _images[0]->getHeight(),
                 _images.getSize(),
                 0,
                 formatGL,
                 formatData,
                 NULL);

    // Load
    for (unsigned int i = 0; i < _images.getSize(); ++i)
    {
        std::cout << "Size i : " << i << " " <<  _images[i]->getWidth() << " X " << _images[i]->getHeight()
                  << " Format "  << _images[i]->getFormat()
                  << std::endl;
        _updateInCg(i);
    }
    std::cout << "Texture array loading done" << std::endl;

    // Unbind
    _unbind();
}

void Texture2DArray::_updateInCg(unsigned int index)
{
    // Bind
    _bind();


    int formatIntern;
    int formatGL;
    int formatData;
    switch (_images[0]->getFormat())
    {
    case Color::RGB_32 :
        formatIntern = GL_RGB32F_ARB;
        formatGL     = GL_RGB;
        formatData   = GL_FLOAT;
        break;
    default :
        formatIntern = GL_RGB;
        formatGL     = GL_RGB;
        formatData   = GL_UNSIGNED_BYTE;
        break;
    }

    Image copy = *(_images[index]);
    Image::convertToGLFormat(&copy);
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0,
                    index,
                    _images[index]->getWidth(),
                    _images[index]->getHeight(),
                    1,
                    formatGL,
                    formatData,
                    copy.getData());

    // unbind
    _unbind();
}

void Texture2DArray::_loadInCg()
{
    // Check
    if (_id != 0)
        return;

    // Gen
    glGenTextures(1, &_id);

    std::cout << "Texture " << _id << " created." << std::endl;

    // Load
    _updateInCg();
}

void Texture2DArray::remove()
{
    // Check
    if (_id == 0)
        return;

    // Remove
    glDeleteTextures(1, &_id);

    std::cout << "Texture " << _id << " removed." << std::endl;


    if (_contains)
    {
        for (unsigned int i = 0; i < _images.getSize(); ++i)
        {
            delete _images[i];
        }
        _images.empty();
    }

}

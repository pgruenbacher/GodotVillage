/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "TextureCubeMap.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <QGLWidget>

#include "TextureCubeMap.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <QGLWidget>


TextureCubeMap::TextureCubeMap(const std::string& name, Image* image)
    : _image(image)
    , _id(0)
{
    this->_name = name;
}

Image* TextureCubeMap::getImage()
{
    return _image;
}

void TextureCubeMap::_bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
}
void TextureCubeMap::_unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::bind(unsigned int pos)
{
    // Check
    if (_id == 0)
        _loadInCg();

    // Check2
    if (pos >= 8)
        return;

    // Enable
    glEnable(GL_TEXTURE_CUBE_MAP);

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Bind
    _bind();

    // Filter, Param
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void TextureCubeMap::unbind(unsigned int pos)
{
    // Check
    if (pos >= 8)
        return;

    // Channel
    glActiveTexture(GL_TEXTURE0 + pos);

    // Unbind
    _unbind();
}

void TextureCubeMap::update()
{
    if (_id == 0)
        _loadInCg();
    else
        _updateInCg();
}

void TextureCubeMap::_updateInCg()
{
    // Check
    if (_id == 0)
        return;

    // Check2
    if (_image->getWidth()<= 0 || _image->getHeight() <= 0)
        return;

    // Bind
    _bind();

    // Load
    glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

    // IMAGE FACES EXTRACTION
    unsigned int face_width  = _image->getWidth()/3;
    unsigned int face_height = _image->getHeight()/4;
    Image face = Image(face_width,
                       face_height,
                       _image->getFormat());

    int formatIntern;
    int formatGL;
    int formatData;
    switch (_image->getFormat())
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

    // POSITIVE Z
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 3 * face_height + line ;
            int col_d = 1 * face_width + col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );


    // NEGATIVE Z
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 1 * face_height + line ;
            int col_d = 1 * face_width + col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );

    // POSITIVE X
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 2 * face_height - 1 - line ;
            int col_d = 3 * face_width - 1 - col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );

    // NEGATIVE X
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 2 * face_height - 1 - line ;
            int col_d = 1 * face_width - 1 - col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );

    // POSITIVE Y
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 2 * face_height + line ;
            int col_d = 1 * face_width + col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );

    // NEGATIVE Y
    for (int line=0; line < face_height; line++)
    {
        for (int col=0; col < face_width; col++)
        {
            int line_d = 0 * face_height + line ;
            int col_d = 1 * face_width + col;
            face.setAt(col, line,_image->getAt( col_d, line_d ) );
        }
    }
    Image::convertToGLFormat(&face);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                 0,
                 formatIntern,
                 face_width,
                 face_height,
                 0,
                 formatGL,
                 formatData,
                 face.getData() );

    // Unbind
    _unbind();
}

void TextureCubeMap::_loadInCg()
{
    // Check
    if (_id != 0)
        return;

    // Gen
    glGenTextures( 1, &_id);

    // Load
    _updateInCg();
}

void TextureCubeMap::remove()
{
    // Check
    if (_id == 0)
        return;

    // Remove
    glDeleteTextures( 1, &_id);
}


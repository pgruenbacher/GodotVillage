/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _Texture2D_H
#define _Texture2D_H

#include <QImage>
#include <string>
using namespace std;

#include "Texture.h"

// Pre-Declaration
class Texture2DManager;

class Texture2D : public Texture
{
private :
    QImage       *_image;
    unsigned int  _id;

    void _loadInCg();
    void _updateInCg();
    void _bind();
    void _unbind();
public :
    Texture2D(const std::string& name, QImage* image);
    QImage* getImage();
    void bind(unsigned int pos = 0);
    void unbind(unsigned int pos = 0);
    void update();
    void remove();

    friend class Texture2DManager;

}; // class Texture2D

#endif // _Texture2D_H

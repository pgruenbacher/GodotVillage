/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TEXTURE_CUBEMAP_H_
#define TEXTURE_CUBEMAP_H_

#include <string>
using namespace std;

#include "Texture.h"
#include "../Image/Image.h"

class TextureCubeMap : public Texture
{
private :
    Image       *_image;
    unsigned int  _id;

    void _loadInCg();
    void _updateInCg();
    void _bind();
    void _unbind();

public :
    TextureCubeMap(const std::string& name, Image* image);
    Image* getImage();
    void update();
    void remove();
    void bind(unsigned int position = 0);
    void unbind(unsigned int position = 0);
}; // class CubeMap

#endif // TEXTURE_CUBEMAP_H_

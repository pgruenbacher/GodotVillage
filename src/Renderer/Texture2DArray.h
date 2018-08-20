/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _Texture2DArray_H
#define _Texture2DArray_H

#include "../Image/Image.h"
#include "../Utils/Table.h"

#include "Texture.h"

// Pre-Declaration
class Texture2DArrayManager;

class Texture2DArray : public Texture
{
private :
    Table<Image*> _images;
    unsigned int _id;
    bool _contains;
    void _loadInCg();
    void _updateInCg();
    void _updateInCg(unsigned int index);
    void _bind();
    void _unbind();
public :
    Texture2DArray(const std::string& name, const Table<Image*>& images, bool contains = false);
    Table<Image*>& getImages();
    void bind(unsigned int pos = 0);
    void unbind(unsigned int pos = 0);
    void update();
    void update(unsigned int index);
    void remove();

    friend class Texture2DArrayManager;

}; // class Texture2DArray

#endif // _Texture2DArray_H

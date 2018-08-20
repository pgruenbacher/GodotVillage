/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>

class Texture
{
protected :
    std::string   _name;
public :
    virtual std::string getName()
    {
        return _name;
    }
    virtual void bind  (unsigned int pos = 0) = 0;
    virtual void unbind(unsigned int pos = 0) = 0;
    virtual void update()                     = 0;
    virtual void remove()                     = 0;
}; // class Texture

#endif // _TEXTURE_H

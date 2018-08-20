/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include <string>
#include <map>
using namespace std;

#include <QImage>

#include "Texture.h"

class TextureManager
{
private :
    static TextureManager* _instance;
    TextureManager();
    ~TextureManager();
public :
    static void create();
    static void destroy();
    static TextureManager* getInstance();

private :
    map<std::string, Texture*> _textures;

public :
    void addTexture(Texture* texture);
    Texture* getTexture(const std::string& name);
    void removeTexture(const std::string& name);

}; // class TextureManager

#endif // _TEXTURE_MANAGER_H

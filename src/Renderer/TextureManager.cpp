/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "TextureManager.h"
#include <iostream>

TextureManager* TextureManager::_instance = NULL;

void TextureManager::create()
{
    if(_instance == NULL)
    {
        _instance = new TextureManager();
    }
}

void TextureManager::destroy()
{
    if(_instance != NULL)
    {
        delete _instance;
        _instance = NULL;
    }
}

TextureManager *TextureManager::getInstance()
{
    return _instance;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::addTexture(Texture* texture)
{
    // Check
    if(texture == NULL)
        return;

    // Check name
    map<std::string, Texture*>::iterator it;
    it = _textures.find(texture->getName());
    if(it != _textures.end())
    {
        std::cout << "addTexture : name already exists : "
                  << texture->getName()
                  << std::endl;
        return;
    }

    // Insert
    std::cout << "Adding texture " << texture->getName() << std::endl;
    _textures[texture->getName()] = texture;
}

Texture* TextureManager::getTexture(const std::string& name)
{
    // Check name
    map<std::string, Texture*>::iterator it;
    it = _textures.find(name);
    if(it == _textures.end())
        return NULL;
    else
        return it->second;
}

void TextureManager::removeTexture(const std::string& name)
{
    // Check name
    map<std::string, Texture*>::iterator it;
    it = _textures.find(name);
    if(it == _textures.end())
    {
        return;
    }
    else
    {
        std::cout << "Removing texture " << name << std::endl;
        it->second->remove();
        _textures.erase(it);
    }
}


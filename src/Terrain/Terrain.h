/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <iostream>
#include "../Image/Image.h"

#include "../Engine/Object3D.h"
#include "../Engine/Mesh.h"

class Terrain : public Object3D
{

private :
    unsigned int _width;
    unsigned int _height;

    float        _maxElevation;

    Image       _snow;
    Image       _heightmap;
    Image       _water;
    Image       _vegetation;
    Image       _rock;
    Image       _roads;
    Image       _houses;

public :
    Mesh*        _mesh;


    Terrain(unsigned int width,
            unsigned int height,
            float maxElevation);

    ~Terrain();

    unsigned int getHeight();
    unsigned int getWidth();

    float getMaxElevation();
    void  setMaxElevation(float maxElevation);

    Image* getSnow();
    Image* getRock();
    Image* getHeightMap();
    Image* getWater();
    Image* getVegetation();
    Image* getRoads();
    Image* getHouses();

    void setSnow(const Image& image);
    void setRock(const Image& image);
    void setHeightMap(const Image& image);
    void setWater(const Image& image);
    void setVegetation(const Image& image);
    void setRoads(const Image& image);
    void setHouses(const Image& image);

    Mesh* getMesh() const
    {
        return _mesh;
    }

    Mesh* generateGeometry();

}; // class Terrain

#endif // _TERRAIN_H

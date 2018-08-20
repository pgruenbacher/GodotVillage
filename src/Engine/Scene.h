/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _SCENE_H
#define _SCENE_H

#include "../Terrain/Terrain.h"
#include "Camera.h"

/** Contains the objects. */
class Scene
{

private :
    Terrain* _terrain;
    Camera*  _camera;

public :

    Scene();
    void setTerrain(Terrain* terrain);
    Terrain* getTerrain();

    void setCamera(Camera* camera);
    Camera* getCamera();

}; // class Scene

#endif // _SCENE_H

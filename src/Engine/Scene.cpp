/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Scene.h"

Scene::Scene()
    : _terrain(NULL)
    , _camera(NULL)
{
}

void Scene::setTerrain(Terrain* terrain)
{
    _terrain = terrain;
}

Terrain* Scene::getTerrain()
{
    return _terrain;
}

void Scene::setCamera(Camera* camera)
{
    _camera = camera;
}

Camera* Scene::getCamera()
{
    return _camera;
}

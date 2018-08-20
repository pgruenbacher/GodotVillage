/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _RENDERER_H
#define _RENDERER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

#include "./Image/Image.h"

#include "Data.h"
#include "./Renderer/Shader.h"
#include "./Maths/Vector.h"
#include "./Loaders/ShaderLoader.h"
#include "./Generator/VillageGenerator.h"
#include "./Generator/Road.h"
#include "./Generator/Building.h"
#include "./Generator/Village.h"
#include "./Utils/LinkedList.h"



// Pre-declaration
class Data;
class Terrain;
class Scene;
class Mesh;
class Camera;
class UI;
class GL_Widget;

class Renderer
{
public :
    Camera*   _cam;
    enum DISPLAY
    {
        NORMAL,
        IGN,
        GRID,

        GLOBAL_INTEREST,
        SOCIAL_INTEREST,
        ROADS_INTEREST,
        ALTITUDE_INTEREST,
        DOMINATION_INTEREST,
        SUN_INTEREST,
        FORTIFICATION_INTEREST,
        MILITARY_INTEREST,
        WATER_INTEREST,
        CULT_INTEREST,
        GLOBAL_FEASABILITY,
        SLOPE_FEASABILITY,
        SPACE_FEASABILITY,
        WATER_FEASABILITY,
        VEGETATION_FEASABILITY,
        NON_FEASABILITY,
        AGGREGATION
    };


    GL_Widget* _GL_Widget;
    UI*     _ui;
private:
    Shader* _terrainShader;
    Shader* _BuildingShader;
    Shader* _skyboxShader;
    Data*   _data;

    Image _interestTexture;

    Image _vegetationColorTexture;
    Image _waterColorTexture;
    Image _groundColorTexture;
    Image _rockColorTexture;
    Image _sandColorTexture;
    Image _snowColorTexture;
    Image _roadColorTexture;

    Image _wallColorTexture;
    Image _roofColorTexture;

    Image _skybox;

    void _initTextures();

    bool wireframe;

    DISPLAY   _displayMode;

public :
    void initData(Data* data);

    Renderer();
    void init();
    void draw();
    void reloadShaders();

    void togglewireframe();
    void setDisplayMode(DISPLAY displayMode);
    void setDisplayMode(BuildingsAtlas selectedAtlas, Building::Type selectedBuildingType, DISPLAY displayMode);

    void updateView(Camera& camera);
    void updateTextures(int index = -1);

    void drawSkybox();

    void drawBuildings(LinkedList<Building> Buildings);
    void drawRoads(LinkedList<Road> roads);
    void drawRoad(Road& road);
    void draw(Building& building);

    void drawWall(Wall& road);
    void drawWalls(LinkedList<Wall> roads);

    void draw(VillageGenerator& system);

    void draw(Mesh& mesh);
    void draw(Terrain& terrain);
    void draw(Scene& scene);

    void drawPen();


};

#endif // _RENDERER_H

/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Renderer.h"
#include "UI.h"
#include "./Terrain/Terrain.h"
#include "./Engine/Mesh.h"
#include "./Engine/Scene.h"
#include "./Engine/Camera.h"
#include "./Renderer/TextureManager.h"
#include "./Renderer/Texture.h"
#include "./Renderer/Texture2D.h"
#include "./Renderer/Texture2DArray.h"
#include "./Renderer/TextureCubeMap.h"
#include "./Engine/Ray.h"
#include "./Generator/Tree.h"


using namespace std;


Renderer::Renderer()
    : _interestTexture(Image(512,512,Color::RGB_32))
    , _vegetationColorTexture(Image(512,512,Color::RGB_8))
    , _waterColorTexture(Image(512,512,Color::RGB_8))
    , _groundColorTexture(Image(512,512,Color::RGB_8))
    , _rockColorTexture(Image(512,512,Color::RGB_8))
    , _sandColorTexture(Image(512,512,Color::RGB_8))
    , _snowColorTexture(Image(512,512,Color::RGB_8))
    , _roadColorTexture(Image(512,512,Color::RGB_8))
    , _wallColorTexture(Image(512,512,Color::RGB_8))
    , _roofColorTexture(Image(512,512,Color::RGB_8))
    , _skybox(Image(1536,512,Color::RGB_8))
    , _GL_Widget(0)
{

    _cam = new Camera(Vector3F(1000, 1900, 1760),
                      Quaternion::createFromAxisAndAngle(-26.0*M_PI/180.0, Vector3F::XAXIS));
    _data = NULL;
    _ui   = NULL;

    setDisplayMode(IGN);
}


void Renderer::drawSkybox()
{
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    Vector3F position      = Vector3F(0,0,0);
    Vector3F scale         = Vector3F(10000,10000,10000);
    Quaternion orientation = Quaternion();

    glTranslatef(position.x, position.y, position.z);
    glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    glScalef    (scale.x, scale.y, scale.z);

    _skyboxShader->bind();
    TextureManager::getInstance()->getTexture("skybox")->bind(0);

    glFrontFace(GL_CW);
    GLUquadricObj *sphere = gluNewQuadric();
    gluSphere(sphere,1,30,30);
    glFrontFace(GL_CCW);

    TextureManager::getInstance()->getTexture("skybox")->unbind(0);
    _skyboxShader->unbind();

    glPopMatrix();
}

void Renderer::setDisplayMode(DISPLAY displayMode)
{
    _displayMode = displayMode;
    std::cout << "Display mode : " << displayMode << std::endl;
}


void Renderer::setDisplayMode(BuildingsAtlas selectedAtlas, Building::Type selectedBuildingType, DISPLAY displayMode)
{
    _displayMode = displayMode;
    std::cout << "Display mode : " << displayMode << std::endl;

    if (!_data)
        return;
    if (!_data->system)
        return;

    int drawScale = _data->system->getScale() * 2.0;
    if ( drawScale == 0)
        drawScale = 1;


    Building building;
    selectedAtlas.updateParameters(building, selectedBuildingType);

    float (VillageGenerator::*compute) (Building& building) = NULL;
    float coeff;
    switch (displayMode)
    {
    case GLOBAL_INTEREST :
        compute = &VillageGenerator::interest;
        break;
    case SOCIAL_INTEREST :
        compute = &VillageGenerator::sociabilityInterest;
        break;
    case ROADS_INTEREST :
        compute = &VillageGenerator::roadsInterest;
        break;
    case ALTITUDE_INTEREST :
        compute = &VillageGenerator::altitudeInterest;
        break;
    case DOMINATION_INTEREST :
        compute = &VillageGenerator::dominationInterest;
        break;
    case SUN_INTEREST :
        compute = &VillageGenerator::sunInterest;
        break;
    case FORTIFICATION_INTEREST :
        compute = &VillageGenerator::wallInterest;
        break;
    case MILITARY_INTEREST :
        compute = &VillageGenerator::militaryInterest;
        break;
    case WATER_INTEREST :
        compute = &VillageGenerator::waterInterest;
        break;
    case CULT_INTEREST :
        compute = &VillageGenerator::cultInterest;
        break;
    case GLOBAL_FEASABILITY :
        compute = &VillageGenerator::feasability;
        break;
    case SLOPE_FEASABILITY :
        compute = &VillageGenerator::slopeFeasability;
        break;
    case SPACE_FEASABILITY :
        compute = &VillageGenerator::spaceFeasability;
        break;
    case WATER_FEASABILITY :
        compute = &VillageGenerator::waterFeasability;
        break;
    case VEGETATION_FEASABILITY :
        compute = &VillageGenerator::vegetationFeasability;
        break;
    case NON_FEASABILITY :
        compute = &VillageGenerator::unbuildableFeasability;
        break;
    case AGGREGATION :
        compute = &VillageGenerator::aggregationProba;
        break;
    default :
        return;
        break;
    }

    _interestTexture = Image(_data->system->getWidth(), _data->system->getHeight(), Color::RGB_32);

    if (!_ui)
        return;

    for (unsigned int i = 0; i < _data->system->getHeight(); i += drawScale)
    {
        for (unsigned int j = 0; j < _data->system->getWidth(); j += drawScale)
        {
            building.getNode().setPosition(Vector3F(i,0,j));
            float interest = (_data->system->*compute)(building);
            _interestTexture.setAt(i, j, Color::RGBF(interest, interest, interest));
            for (unsigned int si = 0; si < drawScale; ++si)
            {
                for (unsigned int sj = 0; sj < drawScale; ++sj)
                {
                    if (i+si < _data->system->getHeight()
                            &&  j+sj < _data->system->getWidth())
                        _interestTexture.setAt(i+si, j+sj, Color::RGBF(interest, interest, interest));
                }
            }
        }
    }

    // Update
    TextureManager::getInstance()->removeTexture("interest");
    Table<Image*> colorImages;
    colorImages.pushLast(&_interestTexture);
    Texture* texture = new Texture2DArray("interest", colorImages);
    TextureManager::getInstance()->addTexture(texture);


}

void Renderer::draw(Mesh& mesh)
{
    // Enable States
    glEnableClientState(GL_VERTEX_ARRAY);

    // LOAD
    if(mesh.getVerticesArray().getSize() > 0)
    {
        glVertexPointer(3, GL_FLOAT, 0, mesh.getVerticesArray().getArray());
    }

    if(mesh.getTexCoordsArray().getSize() > 0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh.getTexCoordsArray().getArray());
    }

    if(mesh.getVerticesNormalsArray().getSize() > 0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, mesh.getVerticesNormalsArray().getArray());
    }

    // DRAW
    glDrawElements(GL_TRIANGLES,
                   3 * mesh.getTrianglesArray().getSize(),
                   GL_UNSIGNED_INT,
                   mesh.getTrianglesArray().getArray());

    // Disable states
    if(mesh.getTexCoordsArray().getSize() > 0)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if(mesh.getVerticesNormalsArray().getSize() > 0)
    {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}


void Renderer::updateTextures(int index)
{
    Texture2DArray* textureh = NULL;
    Texture2DArray* textured = NULL;

    textureh = (Texture2DArray*) TextureManager::getInstance()->getTexture("terrain_h");
    textured = (Texture2DArray*) TextureManager::getInstance()->getTexture("terrain_d");

    if (!textureh)
        return;
    if (!textured)
        return;

    if (index < 0)
    {
        textureh->update();
        textured->update();
    }
    else if (index < 7)
    {
        textured->update(index);
    }
    else if (index == 7)
    {
        textureh->update(0);
    }

}

void Renderer::initData(Data* data)
{
    // DATA
    _data = data;

    // TERRAIN TEXTURE
    Texture* texture = NULL;
    Terrain* terrain = _data->terrain;

    // Check
    std::cout << "INIT DATA CHECK" << std::endl;

    texture = TextureManager::getInstance()->getTexture("terrain_h");
    if (texture != NULL)
    {
        std::cout << "Clearing terrain textures" << std::endl;
        TextureManager::getInstance()->removeTexture("terrain_h");
    }
    texture = TextureManager::getInstance()->getTexture("terrain_d");
    if (texture != NULL)
    {
        std::cout << "Clearing terrain textures" << std::endl;
        TextureManager::getInstance()->removeTexture("terrain_d");
    }

    std::cout << "INIT DATA END CHECK" << std::endl;

    // Terrain
    Table<Image*> terrainImages;
    terrainImages.pushLast(terrain->getHeightMap());
    terrainImages.pushLast(_data->system->_watershed->getImage());
    texture = new Texture2DArray("terrain_h", terrainImages);
    TextureManager::getInstance()->addTexture(texture);

    terrainImages.empty();
    terrainImages.pushLast(terrain->getWater());
    terrainImages.pushLast(terrain->getVegetation());
    terrainImages.pushLast(terrain->getRoads());
    terrainImages.pushLast(terrain->getRock());
    terrainImages.pushLast(terrain->getSnow());
    terrainImages.pushLast(_data->system->_buildingsLayout->getImage());
    terrainImages.pushLast(_data->system->_wallsLayout->getImage());
    texture = new Texture2DArray("terrain_d", terrainImages);
    TextureManager::getInstance()->addTexture(texture);

}

void Renderer::_initTextures()
{
    Texture* texture = NULL;

    // TERRAIN
    Table<Image*> colorImages;
    colorImages.pushLast(&_groundColorTexture);
    colorImages.pushLast(&_rockColorTexture);
    colorImages.pushLast(&_sandColorTexture);
    colorImages.pushLast(&_snowColorTexture);
    colorImages.pushLast(&_waterColorTexture);
    colorImages.pushLast(&_vegetationColorTexture);
    colorImages.pushLast(&_roadColorTexture);
    texture = new Texture2DArray("colors", colorImages);
    TextureManager::getInstance()->addTexture(texture);


    // Building
    colorImages.empty();
    colorImages.pushLast(&_wallColorTexture);
    texture = new Texture2DArray("Building", colorImages);
    TextureManager::getInstance()->addTexture(texture);


    // ROOF
    colorImages.empty();
    colorImages.pushLast(&_roofColorTexture);
    texture = new Texture2DArray("roof", colorImages);
    TextureManager::getInstance()->addTexture(texture);



    // Skybox
    texture = new TextureCubeMap("skybox", &_skybox);
    TextureManager::getInstance()->addTexture(texture);

    // Interest
    colorImages.empty();
    colorImages.pushLast(&_interestTexture);
    texture = new Texture2DArray("interest", colorImages);
    TextureManager::getInstance()->addTexture(texture);

}

void Renderer::draw(Terrain& terrain)
{
    // Check
    Mesh* mesh = terrain.getMesh();
    if(!mesh)
        return;

    // Default color
    glColor3f(1.0, 1.0f, 1.0);

    // Texture Bind
    Texture* texture;
    texture = TextureManager::getInstance()->getTexture("terrain_h");
    if (texture == NULL)
    {
        return;
    }
    texture = TextureManager::getInstance()->getTexture("terrain_d");
    if (texture == NULL)
    {
        return;
    }

    TextureManager::getInstance()->getTexture("terrain_h")->bind(0);

    TextureManager::getInstance()->getTexture("colors")->bind(1);

    TextureManager::getInstance()->getTexture("interest")->bind(2);

    TextureManager::getInstance()->getTexture("terrain_d")->bind(3);


    // Shader bind
    _terrainShader->bind();
    _terrainShader->linkInt("_displayMode", _displayMode);
    _terrainShader->linkFloat("_maxElevation", terrain.getMaxElevation());



    // Push
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    Vector3F position      = terrain.getNode().getPosition();
    Vector3F scale         = Vector3F(1,1,1);
    Quaternion orientation = terrain.getNode().getOrientation();

    glTranslatef(position.x, position.y, position.z);
    glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    glScalef    (scale.x, scale.y, scale.z);

    // Draw
    draw(*mesh);

    // Pop
    glPopMatrix();

    // Shader unbind
    _terrainShader->unbind();

    // Texture Unbind
    TextureManager::getInstance()->getTexture("terrain_h")->unbind(0);
    TextureManager::getInstance()->getTexture("colors")->unbind(1);
    TextureManager::getInstance()->getTexture("interest")->unbind(2);
    TextureManager::getInstance()->getTexture("terrain_d")->unbind(3);

}

void Renderer::draw(Scene& scene)
{

}

void Renderer::draw(VillageGenerator& system)
{
    draw (*(system._terrain));

    drawRoads (system._village.getRoads());

    drawWalls (system._village.getWalls());

    drawBuildings (system._village.getBuildings());

    drawSkybox();


    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0, 0, 0);
    Quaternion orientation = Quaternion();
    glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    glScalef    (1.0,1.0, 1.0);
    _BuildingShader->bind();

    LinkedList<Tree>::LinkedListIterator it(&system._trees);
    while (it.hasNext())
    {
        it.next();
        glColor4f(0.6, 0.3, 0.0, 1.0);
        draw(it.value()._mesh);
        glColor4f(0.0, 0.9, 0.0, 1.0);
        draw(it.value()._mesh1);
    }

    glColor4f(0.25, 0.5, 0.5, 1.0);
    LinkedList<Mesh>::LinkedListIterator it2(&system._wood);
    while (it2.hasNext())
    {
        it2.next();
        draw(it2.value());
    }

    glColor4f(0.3, 0.3, 0.9, 1.0);
    LinkedList<Mesh>::LinkedListIterator it3(&system._glass);
    while (it3.hasNext())
    {
        it3.next();
        draw(it3.value());
    }


    glColor4f(1.0, 0.3, 0.3, 1.0);
    LinkedList<Mesh>::LinkedListIterator it4(&system._tiles);
    while (it4.hasNext())
    {
        it4.next();
        draw(it4.value());
    }


    _BuildingShader->unbind();
    glPopMatrix();


    if (_GL_Widget && _GL_Widget->_state != GL_Widget::Navigation)
    {
        drawPen();
    }

}

void Renderer::drawBuildings(LinkedList<Building> buildings)
{
    LinkedList<Building>::LinkedListIterator it(&buildings);
    while (it.hasNext())
    {
        it.next();
        draw (it.value());
    }
}


void Renderer::drawRoads(LinkedList<Road> roads)
{
    LinkedList<Road>::LinkedListIterator it(&roads);
    while (it.hasNext())
    {
        it.next();
        drawRoad (it.value());
    }
}

void Renderer::drawWalls(LinkedList<Wall> roads)
{
    LinkedList<Wall>::LinkedListIterator it(&roads);
    while (it.hasNext())
    {
        it.next();
        drawWall (it.value());
    }
}

void Renderer::drawPen()
{

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    Ray ray = _GL_Widget->ConvertPixelToRay(Vector2I(_GL_Widget->_lastX, _GL_Widget->_lastY));


    float distance = ray.distFromASurface(_data->system->_terrain->getNode().getPosition(),
                                          Vector3F(0,1,0));

    Vector3F intersection = ray.intersectionWithASurface(_data->system->_terrain->getNode().getPosition(),
                            Vector3F(0,1,0));

    Vector2F cur;
    cur.x = intersection.x;
    cur.y = intersection.z;

    Vector3F position;
    position.x = cur.x;
    position.z = cur.y;

    float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x,-cur.y));
    std::cout << "Vector2I(cur.x,-cur.y) " << Vector2I(cur.x,-cur.y) << std::endl;
    height1 = height1 * _data->system->_terrain->getMaxElevation();
    std::cout << "height1 " << height1 << std::endl;
    position += Vector3F(0.0, height1,0.0);

    Vector3F scale         = Vector3F(2.0,2.0,2.0);
    Quaternion orientation;
    glTranslatef(position.x, position.y, position.z);
    glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    glScalef    (scale.x, scale.y, scale.z);

    glColor4f(0.0,1.0,1.0,0.3);
    GLUquadricObj *sphere = gluNewQuadric();
    gluSphere(sphere,1.1,100,100);

    glPopMatrix();
}

void Renderer::drawWall(Wall& road)
{
    if (road._nodes.getSize()==1)
        return;

    Vector2F prev, cur;
    LinkedList<Vector2F>::LinkedListIterator it(&road._nodes);
    if (it.hasNext())
    {
        it.next();
        prev = it.value();
    }
    while(it.hasNext())
    {
        it.next();
        cur = it.value();

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        Vector3F position1, position2;
        position1.x = prev.x;
        position1.z = - prev.y;
        position2.x = cur.x;
        position2.z = - cur.y;

        float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(prev.x, prev.y));
        height1 = 3.0 + height1 * _data->system->_terrain->getMaxElevation();
        position1 += Vector3F(0.0, height1,0.0);

        float height2 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
        height2 = 3.0 + height2 * _data->system->_terrain->getMaxElevation();
        position2 += Vector3F(0.0, height2,0.0);

        Vector3F scale         = Vector3F(1.0,1.0,1.0);
        Quaternion orientation;
        glTranslatef(0, 0, 0);
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (1,1,1);

        //GLUquadricObj *sphere = gluNewQuadric();
        //gluSphere(sphere,2,20,20);
        glLineWidth(4.f);
        glBegin(GL_LINES);
        glColor4f(0.3,0.3,0.3,1.0);
        glVertex3f(position1.x,position1.y, position1.z);
        glVertex3f(position2.x,position2.y, position2.z);
        glEnd();
        glLineWidth(1.f);

        glPopMatrix();

        prev = cur;
    }

    LinkedList<Vector2F>::LinkedListIterator it2(&road._nodes);
    while(it2.hasNext())
    {
        it2.next();
        cur = it2.value();

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        Vector3F position;

        position.x = cur.x;
        position.z = - cur.y;

        float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
        height1 = 3.0 + height1 * _data->system->_terrain->getMaxElevation();
        position += Vector3F(0.0, height1,0.0);

        Vector3F scale         = Vector3F(1.0,1.0,1.0);
        Quaternion orientation;
        glTranslatef(position.x, position.y, position.z);
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (scale.x, scale.y, scale.z);

        GLUquadricObj *sphere = gluNewQuadric();
        gluSphere(sphere,2.5,6,6);

        glPopMatrix();
    }
}

void Renderer::drawRoad(Road& road)
{
    if (road._path.getSize()==1)
        return;

    if (_displayMode == IGN || _displayMode == NORMAL )
        return;

    Vector2F prev, cur;
    LinkedList<Vector2F>::LinkedListIterator it(&road._path);
    if (it.hasNext())
    {
        it.next();
        prev = it.value();
    }
    while(it.hasNext())
    {
        it.next();
        cur = it.value();

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        Vector3F position1, position2;
        position1.x = prev.x;
        position1.z = - prev.y;
        position2.x = cur.x;
        position2.z = - cur.y;

        float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(prev.x, prev.y));
        height1 = 1.0 + height1 * _data->system->_terrain->getMaxElevation();
        position1 += Vector3F(0.0, height1,0.0);

        float height2 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
        height2 = 1.0 + height2 * _data->system->_terrain->getMaxElevation();
        position2 += Vector3F(0.0, height2,0.0);

        Vector3F scale         = Vector3F(1.0,1.0,1.0);
        Quaternion orientation;
        glTranslatef(0, 0, 0);
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (1,1,1);

        //GLUquadricObj *sphere = gluNewQuadric();
        //gluSphere(sphere,2,20,20);
        glLineWidth(2.f);
        glBegin(GL_LINES);
        glColor4f(0.8,0.0,0.0,0.6);
        glVertex3f(position1.x,position1.y, position1.z);
        glVertex3f(position2.x,position2.y, position2.z);
        glEnd();
        glLineWidth(1.f);

        glPopMatrix();

        prev = cur;
    }

    LinkedList<Vector2F>::LinkedListIterator it2(&road._path);
    while(it2.hasNext())
    {
        it2.next();
        cur = it2.value();

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        Vector3F position;

        position.x = cur.x;
        position.z = - cur.y;

        float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
        height1 = 1.0 + height1 * _data->system->_terrain->getMaxElevation();
        position += Vector3F(0.0, height1,0.0);

        Vector3F scale         = Vector3F(1.0,1.0,1.0);
        Quaternion orientation;
        glTranslatef(position.x, position.y, position.z);
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (scale.x, scale.y, scale.z);

        GLUquadricObj *sphere = gluNewQuadric();
        gluSphere(sphere,0.5,6,6);

        glPopMatrix();
    }
}
void Renderer::draw(Building& building)
{
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);

    glColor4f(0.0,1.0,0.0, 1.0);

    Vector3F position      = building.getNode().getPosition();

    float height = _data->system->_heightmapLayout->getValueAt(Vector2I(position.x, position.z));
    height = height * _data->system->_terrain->getMaxElevation();
    position += Vector3F(0.0, height,0.0);


    position.z = - position.z;
    Vector3F scale         = building.getNode().getScale();
    Quaternion orientation =  building.getNode().getOrientation();

    if (building._mesh.getVerticesArray().getSize() > 0)
    {
        _BuildingShader->bind();
        glTranslatef(0, 0, 0);
        orientation = Quaternion();
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (1.0,1.0, 1.0);
        glColor4f   (1.0,0.9, 0.6, 1.0);
        draw(building._mesh);
        glPopMatrix();
        _BuildingShader->unbind();
        return;
    }

    glTranslatef(position.x, position.y, position.z);
    glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    //glScalef    (building._spaceMin, building._spaceMin, building._spaceMin);
    glScalef    (2.0,2.0, 2.0);

    switch (building._type)
    {
    case Building::HOUSE :
        glColor3f(0.0,1.0,0.0);
        break;
    case Building::VILLA :
        glColor3f(1.0,1.0,1.0);
        break;
    case Building::GARDEN :
        glColor3f(0.7,1.0,0.7);
        break;
    case Building::TRADE :
        glColor3f(1.0,1.0,0.0);
        break;
    case Building::CHURCH :
        glColor3f(0.0,0.0,1.0);
        break;
    case Building::STATUE :
        glColor3f(0.3,0.3,1.0);
        break;
    case Building::FIELD :
        glColor3f(0.0,0.5,0.0);
        break;
    case Building::WATER_MILL :
        glColor3f(0.0,1.0,1.0);
        break;
    case Building::WIND_MILL :
        glColor3f(0.3,1.0,1.0);
        break;
    case Building::TOWER_DEFENSE :
        glColor3f(0.3,0.3,0.3);
        break;
    case Building::CASTLE :
        glColor3f(0.6,0.6,0.6);
        break;
    case Building::WALL :
        glColor3f(0.1,0.1,0.1);
        break;
    }

    GLUquadricObj *sphere = gluNewQuadric();
    gluSphere(sphere,0.5,20,20);
    /*
    _BuildingShader->bind();

    TextureManager::getInstance()->getTexture("Building")->bind(0);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);  glVertex3f(-2,0,-5);
    glTexCoord2f(0.0, 4.0);  glVertex3f(-2,5,-5);
    glTexCoord2f(4.0, 4.0);  glVertex3f(3,7,-5);
    glTexCoord2f(4.0, 0.0);  glVertex3f(3,0,-5);

    glTexCoord2f(0.0, 0.0);  glVertex3f(3,0,5);
    glTexCoord2f(0.0, 4.0);  glVertex3f(3,7,5);
    glTexCoord2f(4.0, 4.0);  glVertex3f(-2,5,5);
    glTexCoord2f(4.0, 0.0);  glVertex3f(-2,0,5);

    glTexCoord2f(0.0, 0.0);  glVertex3f(-2,0,5);
    glTexCoord2f(0.0, 4.0);  glVertex3f(-2,5,5);
    glTexCoord2f(4.0, 4.0);  glVertex3f(-2,5,-5);
    glTexCoord2f(4.0, 0.0);  glVertex3f(-2,0,-5);

    glTexCoord2f(0.0, 0.0);  glVertex3f(3,7,-5);
    glTexCoord2f(4.0, 0.0);  glVertex3f(3,7,5);
    glTexCoord2f(4.0, 4.0);  glVertex3f(3,0,5);
    glTexCoord2f(0.0, 4.0);  glVertex3f(3,0,-5);

    glEnd();

           TextureManager::getInstance()->getTexture("Building")->unbind(0);
           TextureManager::getInstance()->getTexture("roof")->bind(0);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);  glVertex3f(-2,5, 5);
    glTexCoord2f(0.0, 5.0);  glVertex3f(3,7, 5);
    glTexCoord2f(5.0, 5.0);  glVertex3f(3,7,-5);
    glTexCoord2f(5.0, 0.0);  glVertex3f(-2,5,-5);

    glEnd();

           TextureManager::getInstance()->getTexture("roof")->unbind(0);
           _BuildingShader->unbind();
    */

    glPopMatrix();


    // CONTOUR
    if (building._2DFootPrint.getSize() <= 1)
        return;

    Vector2F prev, cur;
    prev = building._2DFootPrint[building._2DFootPrint.getSize()-1];
    for (unsigned int i = 0; i < building._2DFootPrint.getSize(); ++i)
    {
        cur = building._2DFootPrint[i];

        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        Vector3F position1, position2;
        position1.x = prev.x;
        position1.z = - prev.y;
        position2.x = cur.x;
        position2.z = - cur.y;

        float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(prev.x, prev.y));
        height1 = 1.0 + height1 * _data->system->_terrain->getMaxElevation();
        position1 += Vector3F(0.0, height1,0.0);

        float height2 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
        height2 = 1.0 + height2 * _data->system->_terrain->getMaxElevation();
        position2 += Vector3F(0.0, height2,0.0);

        Vector3F scale         = Vector3F(1.0,1.0,1.0);
        Quaternion orientation;
        glTranslatef(0, 0, 0);
        glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
        glScalef    (1,1,1);

        glLineWidth(2.f);
        glBegin(GL_LINES);
        glColor4f(0.8,1.0,0.0,0.6);
        glVertex3f(position1.x,position1.y, position1.z);
        glVertex3f(position2.x,position2.y, position2.z);
        glEnd();
        glLineWidth(1.f);


        glTranslatef(position1.x, position1.y, position1.z);
        GLUquadricObj *sphere = gluNewQuadric();
        gluSphere(sphere,0.5,6,6);

        glPopMatrix();

        prev = cur;
    }

    // LinkedList<Vector2F>::LinkedListIterator it2(&road._path);
    // while(it2.hasNext())
    // {
    //         it2.next();
    //         cur = it2.value();

    //         glPushMatrix();
    //         glMatrixMode(GL_MODELVIEW);

    //         Vector3F position;

    //         position.x = cur.x;
    //         position.z = - cur.y;

    //         float height1 = _data->system->_heightmapLayout->getValueAt(Vector2I(cur.x, cur.y));
    // 	height1 = 1.0 + height1 * _data->system->_terrain->getMaxElevation();
    //         position += Vector3F(0.0, height1,0.0);

    //         Vector3F scale         = Vector3F(1.0,1.0,1.0);
    //         Quaternion orientation;
    //         glTranslatef(position.x, position.y, position.z);
    //         glRotatef   (orientation.angle, orientation.vector.x, orientation.vector.y, orientation.vector.z);
    //         glScalef    (scale.x, scale.y, scale.z);

    //         GLUquadricObj *sphere = gluNewQuadric();
    //         gluSphere(sphere,0.5,6,6);

    //         glPopMatrix();
    // }
















}

void Renderer::updateView(Camera& camera)
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    Vector3F pos = camera.getNode()->getPosition();
    Vector3F at  = camera.getNode()->getPosition() + camera.getNode()->getForward();
    Vector3F up  = camera.getNode()->getUp();

    gluLookAt(pos.x, pos.y, pos.z,
              at.x, at.y, at.z,
              up.x, up.y, up.z);

}

void Renderer::draw()
{
    updateView(*_cam);
    if (_data)
    {
        if(_data->system)
        {
            draw (*_data->system);
        }
    }
}

void Renderer::init()
{
    unsigned int terrainShaderId = LoadProgram("../shaders/Terrain.vs",
                                   "../shaders/Terrain.fs");
    _terrainShader = new Shader(terrainShaderId,
                                "../shaders/Terrain.vs",
                                "../shaders/Terrain.fs");

    _terrainShader->linkTextureNameToTexturePosition("_terrain_h", 0);
    _terrainShader->linkTextureNameToTexturePosition("_colors", 1);
    _terrainShader->linkTextureNameToTexturePosition("_interest", 2);
    _terrainShader->linkTextureNameToTexturePosition("_terrain_d", 3);


    unsigned int BuildingShaderId = LoadProgram("../shaders/House.vs",
                                    "../shaders/House.fs");
    _BuildingShader = new Shader(BuildingShaderId,
                                 "../shaders/House.vs",
                                 "../shaders/House.fs");

    _BuildingShader->linkTextureNameToTexturePosition("_Building", 0);

    _vegetationColorTexture.load ("../data/Textures/Tex4.png");
    _waterColorTexture.load ("../data/Textures/Tex5.png");
    _groundColorTexture.load ("../data/Textures/Tex2.png");
    _rockColorTexture.load ("../data/Textures/Tex1.png");
    _sandColorTexture.load ("../data/Textures/Tex3.png");
    _snowColorTexture.load ("../data/Textures/Tex6.png");
    _roadColorTexture.load ("../data/Textures/road.png");

    _wallColorTexture.load("../data/Textures/brickWall.png");
    _roofColorTexture.load("../data/Textures/roof.png");


    _skybox.load("../data/Textures/skybox.png");

    unsigned int skyboxShaderId = LoadProgram("../shaders/SkyBox.vs",
                                  "../shaders/SkyBox.fs");
    _skyboxShader = new Shader(skyboxShaderId,
                               "../shaders/SkyBox.vs",
                               "../shaders/SkyBox.fs");

    _skyboxShader->linkTextureNameToTexturePosition("_texcube", 0);

    // TextureManager
    TextureManager::create();

    _initTextures();
}

void Renderer::reloadShaders()
{
    _terrainShader->deleteProgram();
    unsigned int terrainShaderId = LoadProgram("../shaders/Terrain.vs",
                                   "../shaders/Terrain.fs");
    _terrainShader->setProgramId(terrainShaderId);

    _BuildingShader->deleteProgram();
    unsigned int BuildingShaderId = LoadProgram("../shaders/House.vs",
                                    "../shaders/House.fs");
    _BuildingShader->setProgramId(BuildingShaderId);


    _skyboxShader->deleteProgram();
    unsigned int skyboxShaderId = LoadProgram("../shaders/SkyBox.vs",
                                  "../shaders/SkyBox.fs");
    _skyboxShader->setProgramId(skyboxShaderId);

    std::cout << "Shader reloaded. " << std::endl;
}



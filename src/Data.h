/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef DATA_H
#define DATA_H

#include "Renderer.h"
#include "./Generator/VillageGenerator.h"
#include "./Generator/Scenario.h"
#include "./Generator/RoadsAtlas.h"
#include "./Generator/BuildingsAtlas.h"
#include "./Maths/Vector.h"
#include "./Terrain/Terrain.h"
#include "./Utils/LinkedList.h"
#include "./Utils/HashTable.h"

#include <QImage>
#include <QString>
#include <iostream>

struct Data
{


    HashTable<std::string, Scenario>       scenarios;

    Terrain* terrain;
    VillageGenerator*  system;
    QString  heightmapName;

    Data();
    void empty();
    void initialize(QString heightmapName = "");
};

#endif // DATA_H

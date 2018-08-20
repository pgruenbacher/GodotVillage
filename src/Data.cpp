/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Data.h"
#include "./Renderer/Texture2DArray.h"

Data::Data()
{
    terrain = NULL;
    system  = NULL;
}

void Data::empty()
{
    std::cout << "R.A.Z " << std::endl;
    delete terrain;
    terrain = NULL;
    delete system;
    system = NULL;
    scenarios.empty();
}

void Data::initialize(QString heightmapName)
{
    std::cout << "Initialisation " << std::endl;
    // ---------------------------------------------------------------------
    // TERRAIN
    // --------------------------------------------------------------------
    // Heightmap
    Image heightmap(Image(2048,2048,Color::RGB_32));
    if(heightmapName == "")
    {
        heightmap.fill(Color::RGBF(0.01,0.01,0.01));
    }
    else if(! heightmap.load (heightmapName.toStdString().c_str()))
    {
        std::cout << "Failed to load " << heightmapName.toStdString().c_str() << std::endl;
        //return;
    }

    // Terrain
    std::cout << "TERRAIN INIT " << std::endl;
    terrain = new Terrain(2048, 2048, 255.f);
    terrain->setHeightMap(heightmap);
    terrain->setRock(Image(2048,2048,Color::RGB_8));
    terrain->setWater(Image(2048,2048,Color::RGB_8));
    terrain->setVegetation(Image(2048,2048,Color::RGB_8));
    terrain->getVegetation()->fill(Color::RGBX(255,255,255));

    // Roads
    std::cout << "ROADS" << std::endl;
    Image roads(2048,2048, Color::RGB_8);
    terrain->setRoads(roads);

    // ---------------------------------------------------------------------
    // SYSTEM
    // ---------------------------------------------------------------------
    std::cout << "VILLAGE" << std::endl;
    system = new VillageGenerator(terrain);
    VillageGenerator::_instance = system;

    // ---------------------------------------------------------------------
    // SCENARIOS
    // ---------------------------------------------------------------------
    std::cout << "SCENARS" << std::endl;
    Scenario scenario1;
    scenario1.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario1.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario1.addSequence(Sequence::Generation(Building::HOUSE, 1));
    scenarios.insert("Scenario1", scenario1);

    Scenario scenario2;
    scenario2.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario2.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario2.addSequence(Sequence::Generation(Building::FIELD, 1));
    scenarios.insert("Scenario2", scenario2);

    Scenario scenario3;
    scenario3.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario3.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario3.addSequence(Sequence::Generation(Building::FIELD, 10));
    scenarios.insert("Scenario3", scenario3);

    Scenario scenario4;
    scenario4.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario4.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1000,1000));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1100,1000));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1100,1150));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1050,1020));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1142,1050));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1150,1050));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1158,1050));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1150,1058));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1150,1042));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1020,1150));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1028,1150));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1036,1150));

    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1020,1020));
    scenario4.addSequence(Sequence::BuildingCreation(Building::HOUSE, 1028,1020));
    scenarios.insert("Scenario4", scenario4);

    Scenario scenario5;
    scenario5.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario5.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario5.addSequence(Sequence::Generation(Building::CHURCH, 1));
    scenario5.addSequence(Sequence::Generation(Building::HOUSE, 1));
    scenario5.addSequence(Sequence::Generation(Building::VILLA, 1));
    scenarios.insert("Scenario5", scenario5);


    Scenario scenario6;
    scenario6.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario6.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario6.addSequence(Sequence::Generation(Building::HOUSE, 10));
    scenarios.insert("Scenario6", scenario6);

    Scenario scenario7;
    scenario7.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario7.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario7.addSequence(Sequence::Generation(Building::VILLA, 10));
    scenarios.insert("Scenario7", scenario7);

    Scenario scenario8;
    scenario8.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario8.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario8.addSequence(Sequence::Generation(Building::CHURCH, 1));
    scenarios.insert("Scenario8", scenario8);

    Scenario scenario9;
    scenario9.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario9.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario9.addSequence(Sequence::WallCreation(1000,1000,200));
    scenarios.insert("Scenario9", scenario9);

    Scenario scenario10;
    scenario10.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario10.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario10.addSequence(Sequence::Generation(Building::VILLA, 1));
    scenarios.insert("Scenario10", scenario10);

    Scenario scenario11;
    scenario11.addSequence(Sequence::BuildingsAtlasSelection("B Atlas 1"));
    scenario11.addSequence(Sequence::RoadsAtlasSelection("R Atlas 1"));
    scenario11.addSequence(Sequence::Generation(Building::CHURCH, 1));
    scenario11.addSequence(Sequence::Generation(Building::HOUSE, 7));
    scenario11.addSequence(Sequence::Generation(Building::VILLA, 10));
    scenario11.addSequence(Sequence::Generation(Building::FIELD, 10));
    scenario11.addSequence(Sequence::Generation(Building::VILLA, 10));
    scenario11.addSequence(Sequence::Generation(Building::FIELD, 20));
    scenarios.insert("Scenario11", scenario11);


    // ---------------------------------------------------------------------
    // ROADS ATLAS
    // ---------------------------------------------------------------------
    RoadsAtlas roadAtlas1;
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_travelWeight, 1.0);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_curvatureWeight, 0.0);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_curvatureMax, 10.0);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_slopeWeight, 10000.0);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_slopeMax, 0.1);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_waterWeight, 1.0);
    roadAtlas1.setParameters(Road::TRAIL, RoadsAtlas::_roadsWeight, 1.0);

    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_travelWeight, 1.0);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_curvatureWeight, 1.0);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_curvatureMax, 10.0);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_slopeWeight, 1000000.0);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_slopeMax, 0.07);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_waterWeight, 1.0);
    roadAtlas1.setParameters(Road::DIRT_ROAD, RoadsAtlas::_roadsWeight, 0.5);

    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_travelWeight, 1.0);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_curvatureWeight, 10.0);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_curvatureMax, 10.0);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_slopeWeight, 2000000.0);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_slopeMax, 0.1);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_waterWeight, 1.0);
    roadAtlas1.setParameters(Road::ROAD, RoadsAtlas::_roadsWeight, 0.0001);

    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_travelWeight, 1.0);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_curvatureWeight, 0.0);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_curvatureMax, 10.0);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_slopeWeight, 1000000.0);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_slopeMax, 0.15);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_waterWeight, 1.0);
    roadAtlas1.setParameters(Road::STREET, RoadsAtlas::_roadsWeight, 0.1);

    system->_roadsAtlasList.insert("R Atlas 1", roadAtlas1);


    RoadsAtlas roadAtlas2;
    system->_roadsAtlasList.insert("R Atlas 2", roadAtlas2);


    // ---------------------------------------------------------------------
    // BUILDINGS ATLAS
    // ---------------------------------------------------------------------

    BuildingsAtlas buildingsAtlas1;
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityWeight, 0.7);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityMin, 5.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityBest, 15.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityMax, 30.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_roadsWeight, 0.3);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_roadsMax, 25.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_dominationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_fortificationWeight, 0.7);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_fortificationRmax, 100.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_militaryWeight, 0.3);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_waterWeight, 0.1);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_waterRmax, 50.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_cultWeight, 0.9);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_cultRmax, 50.0);

    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_spaceMin, 2.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_slopeVarMax, 0.025);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::HOUSE, BuildingsAtlas::_vegetationFeasability, 0.0);


    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_sociabilityWeight, 0.7);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_sociabilityMin, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_sociabilityBest, 70.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_sociabilityMax, 150.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_roadsWeight, 0.3);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_roadsMax, 50.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_dominationWeight, 0.7);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_cultWeight, 0.2);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_spaceMin, 15.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_slopeVarMax, 0.02);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::VILLA, BuildingsAtlas::_vegetationFeasability, 0.0);


    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_sociabilityBest, 10.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_sociabilityMax, 50.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_dominationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_spaceMin, 5.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::GARDEN, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_sociabilityMin, 0.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_sociabilityBest, 5.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_sociabilityMax, 30.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_roadsWeight, 0.3);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_dominationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_fortificationWeight, 1.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_militaryWeight, 0.3);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_waterWeight, 0.1);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_spaceMin, 3.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_slopeVarMax, 0.02);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::TRADE, BuildingsAtlas::_vegetationFeasability, 0.0);


    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_sociabilityWeight, 0.7);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_sociabilityMin, 2.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_sociabilityBest, 25.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_sociabilityMax, 200.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_roadsWeight, 0.1);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_dominationWeight, 1.5);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_spaceMin, 12.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_slopeVarMax, 0.010);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::CHURCH, BuildingsAtlas::_vegetationFeasability, 0.0);


    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_sociabilityWeight, 0.1);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_sociabilityBest, 100.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_sociabilityMax, 500.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_roadsWeight, 0.1);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_roadsMax, 50.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_dominationWeight, 0.8);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_dominationR, 150.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_spaceMin, 3.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::STATUE, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_sociabilityWeight, 0.6);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_sociabilityMin, 10.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_sociabilityBest, 50.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_sociabilityMax, 200.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_roadsWeight, 0.4);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_roadsMax, 150.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_dominationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_sunWeight, 0.7);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_militaryWeight, 0.2);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_militaryRmax, 200.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_waterWeight, 0.5);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_waterRmax, 400.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_spaceMin, 35.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_slopeVarMax, 0.015);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::FIELD, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_sociabilityBest, 10.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_sociabilityMax, 50.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_dominationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_spaceMin, 5.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::WATER_MILL, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_sociabilityBest, 10.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_sociabilityMax, 50.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_dominationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_spaceMin, 5.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::WIND_MILL, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_sociabilityBest, 50.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_sociabilityMax, 150.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_roadsMax, 50.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_dominationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_fortificationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_militaryWeight, 0.6);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_spaceMin, 6.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::TOWER_DEFENSE, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_sociabilityBest, 150.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_sociabilityMax, 400.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_roadsMax, 250.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_dominationWeight, 0.8);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_fortificationWeight, 1.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_militaryWeight, 0.5);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_militaryRmax, 500.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_waterWeight, 0.2);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_waterRmax, 200.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_cultWeight, 0.2);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_cultRmax, 500.0);

    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_spaceMin, 100.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::CASTLE, BuildingsAtlas::_vegetationFeasability, 1.0);


    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_sociabilityMin, 4.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_sociabilityBest, 10.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_sociabilityMax, 50.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_roadsWeight, 0.5);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_roadsMax, 10.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_altitudeWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_altitudeMax, 0.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_dominationWeight, 0.1);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_dominationR, 1000.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_sunWeight, 0.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_fortificationWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_fortificationRmax, 200.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_militaryWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_militaryRmax, 100.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_waterWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_waterRmax, 6.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_cultWeight, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_cultRmax, 400.0);

    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_spaceMin, 5.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_slopeVarMax, 15.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_slopeMax, 5.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_waterFeasability, 0.0);
    buildingsAtlas1.setParameters(Building::WALL, BuildingsAtlas::_vegetationFeasability, 1.0);

    system->_buildingsAtlasList.insert("B Atlas 1", buildingsAtlas1);

    BuildingsAtlas buildingsAtlas2;

    buildingsAtlas2.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityWeight, 1.0);
    buildingsAtlas2.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityMin, 5.0);
    buildingsAtlas2.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityBest, 15.0);
    buildingsAtlas2.setParameters(Building::HOUSE, BuildingsAtlas::_sociabilityMax, 50.0);
    system->_buildingsAtlasList.insert("B Atlas 2", buildingsAtlas2);
}

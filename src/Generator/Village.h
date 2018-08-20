/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef VILLAGE_H
#define VILLAGE_H

#include "Road.h"
#include "Building.h"
#include "Wall.h"

#include "../Utils/LinkedList.h"

#include <iostream>


class Village{
private :
        LinkedList<Road>     _roads;
        LinkedList<Building> _buildings;
        LinkedList<Wall>     _walls;
public :
        void addRoad(const Road& road);
        void addBuilding(const Building& building);
        void addWall(const Wall& wall);
        inline LinkedList<Road>& getRoads(){ return _roads;}
        inline LinkedList<Building>& getBuildings(){ return _buildings;}
        inline LinkedList<Wall>& getWalls() { return _walls; }
        
        void emptyBuildings();
        void emptyRoads();
        void emptyWalls();

	void save(std::string fileName);

}; // class Village

#endif // VILLAGE_H

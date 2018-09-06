/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Village.h"
#include <fstream>

void Village::addRoad(const Road &road) {
	_roads.pushLast(road);
}

void Village::addBuilding(const Building &building) {
	_buildings.pushLast(building);
}

void Village::emptyBuildings() {
	_buildings.empty();
}

void Village::emptyRoads() {
	_roads.empty();
}
void Village::emptyWalls() {
	_walls.empty();
}

void Village::addWall(const Wall &wall) {
	_walls.pushLast(wall);
}

void Village::save(std::string fileName) {
	std::ofstream file(fileName.c_str());

	if (!file.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	std::cout << "Saving village " << std::endl;

	// Roads
	file << "ROADS NB " << _roads.getSize() << "\n";
	LinkedList<Road>::LinkedListIterator it(&_roads);
	while (it.hasNext()) {
		it.next();
		Road road = it.value();
		file << "TYPE " << road._type << " SIZE " << road._path.getSize() << " PATH ";
		LinkedList<Vector2F>::LinkedListIterator itp(&road._path);
		while (itp.hasNext()) {
			itp.next();
			file << itp.value().x << " " << itp.value().y << " ";
		}
		file << "\n";
	}

	// Buildings
	file << "BUILDINGS NB " << _buildings.getSize() << "\n";
	LinkedList<Building>::LinkedListIterator it2(&_buildings);
	while (it2.hasNext()) {
		it2.next();
		Building building = it2.value();
		file << "TYPE " << building._type << " SPACEMIN " << building._spaceMin;
		file << " POSITION "
			 << building.getNode().getPosition().x
			 << " "
			 << building.getNode().getPosition().y
			 << " "
			 << building.getNode().getPosition().z
			 << " ORIENTATION "
			 << building.getNode().getOrientation().angle
			 << " "
			 << building.getNode().getOrientation().vector.x
			 << " "
			 << building.getNode().getOrientation().vector.y
			 << " "
			 << building.getNode().getOrientation().vector.z;
		file << "\n";
	}

	file.close();

	std::cout << "Saving complete " << std::endl;
}

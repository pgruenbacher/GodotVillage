/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Building.h"
#include "Road.h"

#include <stdio.h>
#include <string.h>

class Sequence {
public:
	enum Type {
		BUILDING_GENERATION,
		WALL_CREATION,
		DIRECT_ROAD_MODIFICATION,
		DIRECT_BUILDING_MODIFICATION,
		ROADS_ATLAS_MODIFICATION,
		BUILDINGS_ATLAS_MODIFICATION,
		BUILDINGS_ATLAS_SELECTION,
		ROADS_ATLAS_SELECTION
	};

	union Data {
		struct {
			Building::Type type;
			int number;
		} generation;

		struct {
			Building::Type type;
			int x;
			int y;
		} creation;

		struct {
			int x;
			int y;
			float radius;
		} wall;

		struct {
			Road::Type type;
			float params[6];
		} roadsAtlas;

		struct {
			Building::Type type;
			float params[24];
		} buildingsAtlas;

		struct {
			char name[256];
		} atlasSelection;
	};

private:
	Type _type;
	Data _data;

public:
	static Sequence Generation(Building::Type type, int number) {
		Sequence result;
		result._type = BUILDING_GENERATION;
		result._data.generation.type = type;
		result._data.generation.number = number;
		return result;
	}

	static Sequence RoadsAtlasSelection(const std::string &name) {
		Sequence result;
		result._type = ROADS_ATLAS_SELECTION;
		strcpy(result._data.atlasSelection.name, name.c_str());
		return result;
	}

	static Sequence BuildingsAtlasSelection(const std::string &name) {
		Sequence result;
		result._type = BUILDINGS_ATLAS_SELECTION;
		strcpy(result._data.atlasSelection.name, name.c_str());
		return result;
	}

	static Sequence BuildingCreation(Building::Type type, int x, int y) {
		Sequence result;
		result._type = DIRECT_BUILDING_MODIFICATION;
		result._data.creation.type = type;
		result._data.creation.x = x;
		result._data.creation.y = y;
		return result;
	}

	static Sequence WallCreation(int x, int y, float radius) {
		Sequence result;
		result._type = WALL_CREATION;
		result._data.wall.x = x;
		result._data.wall.y = y;
		result._data.wall.radius = radius;
		return result;
	}

	Type getType() const { return _type; }

	Building::Type getGenerationType() const { return _data.generation.type; }
	int getGenerationNumber() const { return _data.generation.number; }

	const char *getSelectionName() const { return _data.atlasSelection.name; }

	Building::Type getCreationType() const { return _data.creation.type; }
	int getCreationX() const { return _data.creation.x; }
	int getCreationY() const { return _data.creation.y; }

	int getWallX() const { return _data.wall.x; }
	int getWallY() const { return _data.wall.y; }
	int getWallRadius() const { return _data.wall.radius; }

}; // class Sequence

#endif // SEQUENCE_H

/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef VillageGenerator_H
#define VillageGenerator_H

#include <iostream>

#include <time.h>

#include "Building.h"
#include "BuildingsAtlas.h"
#include "Road.h"
#include "RoadsAtlas.h"
#include "Scenario.h"
#include "Sequence.h"
#include "Village.h"

#include "../Terrain/Terrain.h"

#include "../Utils/Graph.h"
#include "../Utils/ImageLayout.h"
#include "../Utils/ImplicitGridedGraph.h"
#include "../Utils/Table.h"

#include "Tree.h"
#include "Watershed.h"

#include "Cost/CostFunctorGradient.h"

class VillageGenerator {
public:
	static VillageGenerator *_instance;

private:
	/** Width of the grid. */
	float _width;

	/** Height of the grid.*/
	float _height;

	/** Scale used for the generation. */
	unsigned int _scale;
	GridedGraphNeighbourhood _neighbourhood;

public:
	bool _bezier;
	Village _village;
	RoadsAtlas _roadsAtlas;
	BuildingsAtlas _buildingsAtlas;
	Terrain *_terrain;
	Watershed *_watershed;

	HashTable<std::string, RoadsAtlas> _roadsAtlasList;
	HashTable<std::string, BuildingsAtlas> _buildingsAtlasList;

	LinkedList<Tree> _trees;
	LinkedList<Mesh> _wood;
	LinkedList<Mesh> _tiles;
	LinkedList<Mesh> _glass;

	/** The graph used for the shortest path computation.
	    The graph is an implicit grid, only the size of the
	    grid and the connectivity pattern are stored. */
	Graph<float, Vector2I, Vector3F> *_landscapeGraph;

	ImageLayout<float, Vector2I, Vector3F> *_heightmapLayout;
	ImageLayout<float, Vector2I, Vector3F> *_waterLayout;
	ImageLayout<float, Vector2I, Vector3F> *_roadsLayout;
	ImageLayout<float, Vector2I, Vector3F> *_roadsColorLayout;
	ImageLayout<float, Vector2I, Vector3F> *_vegetationLayout;
	ImageLayout<float, Vector2I, Vector3F> *_rockLayout;
	ImageLayout<float, Vector2I, Vector3F> *_buildingsLayout;
	ImageLayout<float, Vector2I, Vector3F> *_wallsLayout;

	CostFunctorGradient<float, Vector2I, Vector3F> *_terrainFunctor;

	/** Points on the edge of the grid which represent the "exterior".*/
	Table<Vector2F> _waysToTheWorld;
	void _initWaysToTheWorld();

public:
	// CONSTRUCTOR
	VillageGenerator(Terrain *terrain);
	~VillageGenerator();

	// GENERATOR
	void setScale(unsigned int scale);
	void setNeighbourhood(GridedGraphNeighbourhood neighbourhood);

	unsigned int getScale() { return _scale; }
	float getWidth() { return _width; }
	float getHeight() { return _height; }

	// WALLS
	void clearWalls();
	void addWall(Wall &wall);
	void wallCorrection(Wall &wall);
	LinkedList<Vector2F> createVertexListForJarvis(const Vector2F &center, float radius);

	// ROADS
	void addRoad(Road &rroad, bool bezier = true, bool draw = true);
	void clearRoads();
	void drawRoads();

	// BUILDINGS
	void addBuilding(Building &building);
	void clearBuildings();

	// COMPUTATION
	float roadDistance(Building &building);
	float _computeDomination(float radius, unsigned int nbEchantillons,
			unsigned int i, unsigned int j);
	float _computeWater(float radius, unsigned int nbEchantillons,
			unsigned int i, unsigned int j);
	Vector2F correctWithScale(Vector2F vector);
	Vector3F correctWithScale(Vector3F vector);
	bool randomTest(float probability);
	bool validPosition(const Vector2F &pos);
	bool validPosition(const Vector3F &pos);

	// INTERESTS
	float sociabilityInterest(Building &building);
	float roadsInterest(Building &building);
	float altitudeInterest(Building &building);
	float dominationInterest(Building &building);
	float sunInterest(Building &building);
	float wallInterest(Building &building);
	float militaryInterest(Building &building);
	float waterInterest(Building &building);
	float cultInterest(Building &building);

	float interest(Building &building);

	// FEASABILITY
	float slopeFeasability(Building &building);
	float spaceFeasability(Building &building);
	float vegetationFeasability(Building &building);
	float waterFeasability(Building &building);
	float unbuildableFeasability(Building &building);

	float feasability(Building &building);

	// GENERATION
	void playAScenario(const Scenario &scenario);
	void playASequence(const Sequence &sequence);
	void generateGeometry();
	void exportGeometry(const std::string &filename);

	// POSITIONING BUILDING
	float aggregationProba(Building &building);
	void initialPosition(Building &building);
	bool feasabilityTest(Building &building);
	bool aggregationTest(Building &building);

	void positionCluster(Building &building, int nb);

	Vector3F getNeighbour(unsigned int nb);
	void stochPos(Building &building);
	void stochasticPositioning(Building &building);

	void positioningBuilding(Building &building);

	// CONNEXION TO ROAD NETWORK
	bool roadExistenceTest(Building &building);
	Vector2F findClosestRoadNode(Building &building, Vector2F *normalResult = 0);
	Vector2F findClosestRoadNormal(Building &building);
	void generateBuildingRoad(Building &building, const Vector2F &near);
	void shortcuts();
	void betaSkeleton();

	void networkConnexion(Building &building);
	void watershed();

	LinkedList<Vector2I> generatePath(Vector2F source,
			Vector2F dest,
			Road::Type roadType);

	void save(std::string file);
	void load(std::string file);

}; // class VillageGenerator

#endif // VillageGenerator_H

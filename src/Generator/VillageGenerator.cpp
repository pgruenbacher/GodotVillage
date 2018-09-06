/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "VillageGenerator.h"

#include "../Image/Painter.h"
#include "../Maths/Calcul.h"
#include "../Maths/DeCasteljau.h"
#include "../PathFinding/PathFinding.h"
#include <fstream>
#include <sstream>

VillageGenerator *VillageGenerator::_instance = NULL;

VillageGenerator::VillageGenerator(Terrain *terrain)
		: _terrain(terrain), _width(terrain->getWidth()), _height(terrain->getHeight()), _landscapeGraph(NULL), _scale(4), _neighbourhood(THIRTY_TWO), _bezier(false) {
	std::cout << "NEW VILLAGE" << std::endl;
	srand(time(NULL));

	std::cout << "LAYOUTS" << std::endl;
	_heightmapLayout = new ImageLayout<float, Vector2I, Vector3F>(_terrain->getHeightMap());
	_waterLayout = new ImageLayout<float, Vector2I, Vector3F>(_terrain->getWater());
	_vegetationLayout = new ImageLayout<float, Vector2I, Vector3F>(_terrain->getVegetation());
	_rockLayout = new ImageLayout<float, Vector2I, Vector3F>(_terrain->getRock());

	setNeighbourhood(THIRTY_TWO);
	setScale(4);

	_terrainFunctor = new CostFunctorGradient<float, Vector2I, Vector3F>;
	std::cout << "LAYOUTS WITH NEW 1 " << std::endl;
	_roadsLayout = new ImageLayout<float, Vector2I, Vector3F>(new Image(_width, _height, Color::RGB_8), true);
	std::cout << "LAYOUTS WITH NEW 2" << std::endl;
	_roadsColorLayout = new ImageLayout<float, Vector2I, Vector3F>(_terrain->getRoads());
	std::cout << "LAYOUTS WITH NEW 3" << std::endl;
	_buildingsLayout = new ImageLayout<float, Vector2I, Vector3F>(new Image(_width, _height, Color::RGB_8), true);
	std::cout << "LAYOUTS WITH NEW 4" << std::endl;
	_wallsLayout = new ImageLayout<float, Vector2I, Vector3F>(new Image(_width, _height, Color::RGB_8), true);
	std::cout << "LAYOUTS WITH NEW 5" << std::endl;

	_initWaysToTheWorld();

	std::cout << "WATERSHED" << std::endl;
	_watershed = new Watershed(_width, _height);
}

VillageGenerator::~VillageGenerator() {
	delete _landscapeGraph;
	delete _heightmapLayout;
	delete _waterLayout;
	delete _roadsLayout;
	delete _roadsColorLayout;
	delete _vegetationLayout;
	delete _rockLayout;
	delete _buildingsLayout;
	delete _wallsLayout;
	delete _terrainFunctor;
	delete _watershed;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              GENERATOR
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void VillageGenerator::setScale(unsigned int scale) {
	if (_landscapeGraph)
		delete _landscapeGraph;

	_scale = scale;
	_landscapeGraph = new ImplicitGridedGraph<float, Vector3F>(_width,
			_height,
			_neighbourhood,
			_scale);
}

void VillageGenerator::setNeighbourhood(GridedGraphNeighbourhood neighbourhood) {
	if (_landscapeGraph)
		delete _landscapeGraph;

	_neighbourhood = neighbourhood;
	_landscapeGraph = new ImplicitGridedGraph<float, Vector3F>(_width,
			_height,
			_neighbourhood,
			_scale);
}

void VillageGenerator::generateGeometry() {
	_wood.empty();
	_tiles.empty();
	_glass.empty();
	_trees.empty();

	//BUILDING
	LinkedList<Building> &buildings = _village.getBuildings();
	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		it.value().generateGeometry();
	}

	// TREES
	for (unsigned int i = 0; i < _height; i += 2) {
		for (unsigned int j = 0; j < _width; j += 2) {
			if (_vegetationLayout->getImage()->getAt(j, i).r() > 0.96 && _waterLayout->getImage()->getAt(j, i).r() <= 0.5) {
				float random = float(rand() % 10000) / 10000.0;
				if (random > 0.98) {
					float height = _terrain->getHeightMap()->getAt(j, i).rf() * _terrain->getMaxElevation();
					_trees.pushLast(Tree(Vector3F(j, height, -float(i))));
				}
			}
		}
	}
}

void VillageGenerator::exportGeometry(const std::string &filename) {
	// BUILDING
	LinkedList<Building> &buildings = _village.getBuildings();
	LinkedList<Building>::LinkedListIterator it(&buildings);
	int building = 0;
	while (it.hasNext()) {
		it.next();
		Mesh result = it.value()._mesh;
		std::stringstream st;
		st << filename << "_Building_" << building << ".obj";
		result.exportToObj(st.str());
		++building;
	}

	// TERRAIN
	Mesh *result = _terrain->generateGeometry();
	std::stringstream st;
	st << filename << "_Terrain_"
	   << ".obj";
	result->exportToObj(st.str());
	delete result;

	// TREE
	int tree = 0;
	LinkedList<Tree>::LinkedListIterator it2(&_trees);
	while (it2.hasNext()) {
		it2.next();
		std::stringstream st;
		st << filename << "_Tree_Leaves_" << tree << ".obj";
		it2.value()._mesh1.exportToObj(st.str());
		std::stringstream st2;
		st2 << filename << "_Tree_Body_" << tree << ".obj";
		it2.value()._mesh.exportToObj(st2.str());
	}
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              WALLS
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void VillageGenerator::clearWalls() {
	_village.emptyWalls();
	_wallsLayout->getImage()->fill(Color::RGBX(0, 0, 0));
}

void VillageGenerator::wallCorrection(Wall &wall) {
	LinkedList<Vector2F> result;
	LinkedList<Vector2F> resultDoors;

	LinkedList<Vector2F>::LinkedListIterator it(&wall._nodes);
	Vector2F prev, current;
	if (it.hasNext()) {
		it.next();
		prev = it.value();
		result.pushLast(prev);
		if (Painter::getColor(prev, _terrain->getRoads()).x > 0.0) {
			resultDoors.pushLast(prev);
		}
	}
	while (it.hasNext()) {
		it.next();
		current = it.value();
		Table<Vector2F> points = PainterAlgorithms::findBresenhamLinePoints(prev, current);
		for (unsigned int i = 1; i < points.getSize() - 1; ++i) {
			if (Painter::getColor(points[i], _terrain->getRoads()).x > 0.0) {
				result.pushLast(points[i]);
				resultDoors.pushLast(points[i]);
			}
		}
		result.pushLast(current);
		if (Painter::getColor(current, _terrain->getRoads()).x > 0.0) {
			resultDoors.pushLast(current);
		}
		prev = current;
	}

	wall._nodes = result;
	wall._doors = resultDoors;
}

void VillageGenerator::addWall(Wall &wall) {
	wallCorrection(wall);
	_village.addWall(wall);

	// Update textures
	LinkedList<Vector2F>::LinkedListIterator it(&wall._nodes);
	Vector2F prev, current;
	if (it.hasNext()) {
		it.next();
		prev = it.value();
	}
	while (it.hasNext()) {
		it.next();
		current = it.value();

		Painter::drawBresenhamLine(prev,
				current,
				_wallsLayout->getImage(),
				Vector3I(255, 255, 255),
				3.0);

		Table<Vector2F> points = PainterAlgorithms::findBresenhamLinePoints(prev, current);
		for (unsigned int i = 0; i < points.getSize(); ++i) {
			if (wall._doors.contains(points[i])) {
				Painter::drawCircle(points[i],
						4.0,
						_wallsLayout->getImage(),
						Vector3I(20, 20, 20));

				Painter::drawCircle(points[i],
						15,
						_terrain->getVegetation(),
						Vector3I(0, 0, 0));
			}
		}

		prev = current;
	}
}

LinkedList<Vector2F> VillageGenerator::createVertexListForJarvis(const Vector2F &center, float radius) {
	LinkedList<Vector2F> result;
	Vector3F currentPos;

	// ADD BUILDINGS
	LinkedList<Building> buildings = _village.getBuildings();
	Vector2F pos;
	float buildingRadius;

	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		if (it.value()._type == Building::FIELD)
			continue;
		pos.x = it.value().getNode().getPosition().x;
		pos.y = it.value().getNode().getPosition().z;
		if (norm(center - pos) < radius) {
			buildingRadius = it.value()._spaceMin;
			result.pushLast(pos + Vector2F(3.0 * buildingRadius + 10, 3.0 * buildingRadius + 10));
			result.pushLast(pos + Vector2F(-3.0 * buildingRadius - 10, 3.0 * buildingRadius + 10));
			result.pushLast(pos + Vector2F(-3.0 * buildingRadius - 10, -3.0 * buildingRadius - 10));
			result.pushLast(pos + Vector2F(3.0 * buildingRadius + 10, -3.0 * buildingRadius - 10));
		}
	}

	// ADD NATURAL PROTECTIONS
	float currentHeight;
	float gradX, gradY;
	Vector2F gradV;
	float grad;
	for (int h = -radius; h <= radius; ++h) {
		for (int w = -radius; w <= radius; ++w) {
			if ((w * w + h * h) <= radius * radius) {
				pos = center + Vector2F(w, h);
				if (!validPosition(pos))
					continue;
				// HEIGHT GRADIENT
				currentHeight = _heightmapLayout->getValueAt(pos);
				gradX = _heightmapLayout->getValueAt(pos + Vector2F(_scale, 0.0)) - currentHeight;
				gradY = _heightmapLayout->getValueAt(pos + Vector2F(0.0, _scale)) - currentHeight;
				gradV = Vector2F(gradX, gradY);
				grad = norm(gradV);
				if (grad > 0.01 && dotProduct(gradV, Vector2F(w, h)) >= 0.0) {
					result.pushLast(pos);
				}
				// WATER GRADIENT
				currentHeight = _waterLayout->getValueAt(pos);
				gradX = _waterLayout->getValueAt(pos + Vector2F(_scale, 0.0)) - currentHeight;
				gradY = _waterLayout->getValueAt(pos + Vector2F(0.0, _scale)) - currentHeight;
				gradV = Vector2F(gradX, gradY);
				grad = norm(gradV);
				if (grad > 10.0 && dotProduct(gradV, Vector2F(w, h)) >= 0.0) {
					result.pushLast(pos);
				}
			}
		}
	}

	return result;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              ROADS
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void VillageGenerator::drawRoads() {
	if (_village.getRoads().getSize() <= 0)
		return;

	// BLACK
	LinkedList<Road> &roads = _village.getRoads();
	LinkedList<Road>::LinkedListIterator it3(&roads);
	while (it3.hasNext()) {
		it3.next();

		Vector3I color;
		int width;
		switch (it3.value()._type) {
			case Road::TRAIL:
				color = Vector3I(255, 0, 0);
				width = 1;
				break;
			case Road::DIRT_ROAD:
				color = Vector3I(255, 50, 0);
				width = 2;
				break;
			case Road::ROAD:
				color = Vector3I(255, 100, 0);
				width = 3;
				break;
			case Road::STREET:
				color = Vector3I(255, 150, 0);
				width = 2;
				break;
		}

		LinkedList<Vector2F> nodes(it3.value()._path);
		LinkedList<Vector2F>::LinkedListIterator it2(&nodes);
		if (!it2.hasNext())
			continue;
		Vector2I start, end;
		start = nodes.getFirst();
		it2.next();
		while (it2.hasNext()) {
			it2.next();
			end = it2.value();

			Table<Vector2I> points = PainterAlgorithms::findBresenhamLinePoints(start, end);

			for (unsigned int i = 0; i < points.getSize(); ++i) {

				Painter::drawCircle(points[i],
						width + 1,
						_terrain->getRoads(),
						Vector3I(10, 10, 10));

				Painter::drawCircle(points[i],
						width + 5,
						_terrain->getVegetation(),
						Vector3I(0, 0, 0));
			}
			start = end;
		}
	}

	// COLOR
	LinkedList<Road>::LinkedListIterator it(&roads);
	while (it.hasNext()) {
		it.next();

		Vector3I color;
		int width;
		switch (it.value()._type) {
			case Road::TRAIL:
				color = Vector3I(255, 0, 0);
				width = 1;
				break;
			case Road::DIRT_ROAD:
				color = Vector3I(255, 50, 0);
				width = 2;
				break;
			case Road::ROAD:
				color = Vector3I(255, 100, 0);
				width = 3;
				break;
			case Road::STREET:
				color = Vector3I(255, 150, 0);
				width = 2;
				break;
		}

		LinkedList<Vector2F> nodes(it.value()._path);
		LinkedList<Vector2F>::LinkedListIterator it2(&nodes);
		if (!it2.hasNext())
			continue;
		Vector2I start, end;
		start = nodes.getFirst();
		it2.next();
		while (it2.hasNext()) {
			it2.next();
			end = it2.value();

			Table<Vector2I> points = PainterAlgorithms::findBresenhamLinePoints(start, end);

			for (unsigned int i = 0; i < points.getSize(); ++i) {

				Painter::drawCircle(points[i],
						width,
						_terrain->getRoads(),
						color);
			}
			start = end;
		}
	}
}

void VillageGenerator::addRoad(Road &rroad, bool bezier, bool draw) {
	float step = 1.0 / (float(rroad._path.getSize()) * 8.0);

	// Compute Bezier Curve
	LinkedList<Vector2F> road;
	if (bezier && _bezier)
		road = DeCasteljau::bezierCurve(rroad._path, step);
	else
		road = rroad._path;

	// Add the road
	_village.addRoad(Road(rroad._type, road));

	Vector3I color;
	int width;
	switch (rroad._type) {
		case Road::TRAIL:
			color = Vector3I(255, 0, 0);
			width = 1;
			break;
		case Road::DIRT_ROAD:
			color = Vector3I(255, 50, 0);
			width = 2;
			break;
		case Road::ROAD:
			color = Vector3I(255, 100, 0);
			width = 4;
			break;
		case Road::STREET:
			color = Vector3I(255, 150, 0);
			width = 3;
			break;
	}

	// Update textures
	LinkedList<Vector2F>::LinkedListIterator it2(&road);
	Vector2F prev, current;
	if (it2.hasNext()) {
		it2.next();
		prev = it2.value();
		// Draw the node
		Painter::drawPoint(prev,
				_roadsLayout->getImage(),
				color);
	}
	while (it2.hasNext()) {
		it2.next();
		current = it2.value();

		// Draw the node
		Painter::drawPoint(current,
				_roadsLayout->getImage(),
				color);

		prev = current;
	}

	if (draw)
		drawRoads();
}

void VillageGenerator::clearRoads() {
	_village.emptyRoads();
	_roadsLayout->getImage()->fill(Color::RGBX(0, 0, 0));
	_terrain->getRoads()->fill(Color::RGBX(0, 0, 0));
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              BUILDINGS
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void VillageGenerator::addBuilding(Building &building) {
	Building p(building);

	float max = float(building._spaceMin) * 2.0;
	Vector3I color;
	switch (building._type) {
		case Building::HOUSE:
			max = float(building._spaceMin) * 3.0 / 2.0;
			color = Vector3I(255, 0, 255);
			break;
		case Building::FIELD:
			max = float(building._spaceMin) * 3.0;
			color = Vector3I(255, 10, 255);
			break;
		case Building::CHURCH:
			color = Vector3I(255, 20, 255);
			break;
		case Building::VILLA:
			color = Vector3I(255, 30, 255);
			break;
	}

	_village.addBuilding(building);

	Vector2F pos;
	pos.x = building.getNode().getPosition().x;
	pos.y = building.getNode().getPosition().z;

	Painter::drawCircle(pos,
			p._spaceMin,
			_buildingsLayout->getImage(),
			color);

	Painter::drawCircle(pos,
			p._spaceMin * 2 + 10,
			_terrain->getVegetation(),
			Vector3I(0, 0, 0));
}

void VillageGenerator::clearBuildings() {
	_village.emptyBuildings();
	_buildingsLayout->getImage()->fill(Color::RGBX(0, 0, 0));
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              COMPUTATION
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

float VillageGenerator::roadDistance(Building &building) {
	int edge = 5;
	Vector3F position = building.getNode().getPosition();
	Vector2F pos = Vector2F(position.x, position.z);
	float dist = 100000.0 * float(edge);
	for (int i = -edge; i < edge; ++i) {
		for (int j = -edge; j < edge; ++j) {
			if (j == 0 && i == 0)
				continue;
			if (_roadsLayout->getValueAt(pos + Vector2F(i * _scale, j * _scale)) > 0.0) {
				dist = MIN(dist / 6.0, norm(Vector2F(i * _scale, j * _scale)));
			}
		}
	}
	return dist;
}

Vector2F VillageGenerator::correctWithScale(Vector2F vector) {
	return Vector2F(int(vector.x) - int(vector.x) % _scale,
			int(vector.y) - int(vector.y) % _scale);
}

Vector3F VillageGenerator::correctWithScale(Vector3F vector) {
	return Vector3F(int(vector.x) - int(vector.x) % _scale,
			int(vector.y) - int(vector.y) % _scale,
			int(vector.z) - int(vector.z) % _scale);
}

bool VillageGenerator::randomTest(float probability) {
	unsigned int accuracy = 100000;
	float random = float(rand() % accuracy) / float(accuracy - 1);
	return (probability > random);
}

bool VillageGenerator::validPosition(const Vector2F &pos) {
	if (pos.x < 0 || pos.x >= _width)
		return false;

	if (pos.y < 0 || pos.y >= _height)
		return false;

	return true;
}

bool VillageGenerator::validPosition(const Vector3F &pos) {
	return validPosition(Vector2F(pos.x, pos.z));
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              INTERESTS
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

float VillageGenerator::wallInterest(Building &building) {
	if (_village.getWalls().getSize() == 0) {
		return 0.2;
	}

	float result = 100000.0;

	Vector2F position;
	position.x = building.getNode().getPosition().x;
	position.y = building.getNode().getPosition().z;

	LinkedList<Wall> &walls = _village.getWalls();
	LinkedList<Wall>::LinkedListIterator it(&walls);
	while (it.hasNext()) {
		it.next();
		result = MIN(result, distanceToAPolygon(position, it.value()._nodes));
	}

	// INSIDE
	if (result < 0.0)
		return 1.0;

	// OUTSIDE
	else if (result > building._fortificationRmax)
		return 0.0;
	else
		return 0.5 - 0.5 * (result / building._fortificationRmax) * (result / building._fortificationRmax);
}

float VillageGenerator::waterInterest(Building &building) {
	return _computeWater(building._waterRmax,
			50,
			building.getNode().getPosition().x,
			building.getNode().getPosition().z);
}

float VillageGenerator::_computeWater(float radius,
		unsigned int nbEchantillons,
		unsigned int i,
		unsigned int j) {
	if (radius < 2.0)
		return 0.0;

	Vector2F minDist(999999, 999999);
	int w, h;
	float presence;
	if (_waterLayout->getValueAt(Vector2F(i + h, j + w)) > 128) {
		return 1.0;
	}

	if (!validPosition(Vector2F(i, j)))
		return 0.0;

	for (unsigned int n = 0; n < nbEchantillons;) {
		w = rand() % int(2.0 * radius) - radius;
		h = rand() % int(2.0 * radius) - radius;

		if (j + w < 0 || j + w >= _width)
			continue;

		if (i + h < 0 || i + h >= _height)
			continue;

		if (_waterLayout->getValueAt(Vector2F(i + h, j + w)) > 128) {
			if (norm(minDist) > norm(Vector2F(h, w))) {
				minDist = Vector2F(h, w);
			}
		}
		++n;
	}

	float r = norm(minDist);
	if (r > radius) {
		return 0.0;
	} else {
		return 1.0 - (r / radius) * (r / radius);
	}
}

float VillageGenerator::militaryInterest(Building &building) {
	LinkedList<Building> buildings = _village.getBuildings();
	int nb = 0;

	float result = 0.0;
	Vector3F position = building.getNode().getPosition();
	float dist = 999999.0;

	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		if ((it.value()._type == Building::TOWER_DEFENSE) || (it.value()._type == Building::CASTLE)) {
			dist = MIN(norm(position - it.value().getNode().getPosition()) - it.value()._spaceMin - building._spaceMin, dist);
			++nb;
		}
	}

	if (nb == 0) {
		return 0.2;
	}

	float sociab;
	if (dist > building._militaryRmax) {
		return 0.0;
	} else {
		return 1.0 - (dist / building._militaryRmax) * (dist / building._militaryRmax);
	}
}

float VillageGenerator::cultInterest(Building &building) {
	LinkedList<Building> buildings = _village.getBuildings();
	int nb = 0;

	float result = 0.0;
	Vector3F position = building.getNode().getPosition();
	float dist = 999999.0;

	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		if ((it.value()._type == Building::CHURCH) || (it.value()._type == Building::STATUE)) {
			dist = MIN(norm(position - it.value().getNode().getPosition()) - it.value()._spaceMin - building._spaceMin, dist);
			++nb;
		}
	}

	if (nb == 0) {
		return 0.2;
	}

	float sociab;
	if (dist > building._cultRmax) {
		return 0.0;
	} else {
		return 1.0 - (dist / building._cultRmax) * (dist / building._cultRmax);
	}
}

float VillageGenerator::altitudeInterest(Building &building) {
	Vector3F position = building.getNode().getPosition();
	float result = (_heightmapLayout->getValueAt(Vector2F(position.x, position.z)));
	return result;
}

float VillageGenerator::_computeDomination(float radius,
		unsigned int nbEchantillons,
		unsigned int i,
		unsigned int j) {
	float domination = 0.0;

	if (radius < 2.0)
		return 0.0;

	Vector3F normal(0, 1, 0);
	Vector3F dir;
	float dist;
	float heightIJ;
	float heightWH;
	int w, h;

	if (!validPosition(Vector2F(i, j)))
		return 0.0;

	heightIJ = _heightmapLayout->getValueAt(Vector2F(i, j));
	for (unsigned int n = 0; n < nbEchantillons;) {
		w = rand() % int(2.0 * radius) - radius;
		h = rand() % int(2.0 * radius) - radius;

		if (j + w < 0 || j + w >= _width)
			continue;

		if (i + h < 0 || i + h >= _height)
			continue;

		if ((w == h) && (h == 0))
			continue;

		heightWH = _heightmapLayout->getValueAt(Vector2F(i + h, j + w));
		domination += (heightIJ - heightWH) / (1.0 + dist * dist);
		++n;
	}

	domination = domination * 6;
	return domination;
}

float VillageGenerator::dominationInterest(Building &building) {
	float radius = building._dominationR;
	float nbEchantillons = 70.0;
	float domination;
	unsigned int i = building.getNode().getPosition().x;
	unsigned int j = building.getNode().getPosition().z;
	domination = _computeDomination(radius / 8.0, nbEchantillons / 4.0, i, j);
	domination += _computeDomination(radius / 4.0, nbEchantillons / 4.0, i, j);
	domination += _computeDomination(radius / 2.0, nbEchantillons / 4.0, i, j);
	domination += _computeDomination(radius, nbEchantillons / 4.0, i, j);
	domination /= nbEchantillons;
	domination = MIN(domination, 1.0);
	//domination = 0.5 + 0.5 * domination;
	domination = MAX(0.0, domination);
	return domination * domination;
}

float VillageGenerator::sunInterest(Building &building) {
	return 0.0;
}

float VillageGenerator::roadsInterest(Building &building) {
	if (_village.getRoads().getSize() == 0) {
		return 0.2;
	}

	float result = 100000.0;

	Vector2F position;
	position.x = building.getNode().getPosition().x;
	position.y = building.getNode().getPosition().z;

	LinkedList<Road> &roads = _village.getRoads();
	LinkedList<Road>::LinkedListIterator it(&roads);
	while (it.hasNext()) {
		it.next();
		result = MIN(result, ABS(distanceToACurve(position, it.value()._path)));
	}

	if (result > building._roadsMax)
		return 0.0;
	else
		return 1.0 - (result / building._roadsMax) * (result / building._roadsMax);
}

float VillageGenerator::sociabilityInterest(Building &building) {
	LinkedList<Building> buildings = _village.getBuildings();
	int nb = 0;

	float result = 0.0;
	Vector3F position = building.getNode().getPosition();
	float dist = 999999.0;

	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		if ((it.value()._type == Building::HOUSE) || (it.value()._type == Building::VILLA) || (it.value()._type == Building::TRADE) || (it.value()._type == Building::WATER_MILL) || (it.value()._type == Building::WIND_MILL)) {
			dist = MIN(norm(position - it.value().getNode().getPosition()) - it.value()._spaceMin - building._spaceMin, dist);
			++nb;
		}
	}

	float sociab;
	if (dist <= building._sociabilityMin) {
		result = 0.0;
	} else if (dist <= building._sociabilityBest) {
		result = (dist - building._sociabilityMin) / (building._sociabilityBest - building._sociabilityMin);
	} else if (dist <= building._sociabilityMax) {
		result = 1.0 - (dist - building._sociabilityBest) / (building._sociabilityMax - building._sociabilityBest);
	} else {
		result = 0.0;
	}

	if (nb == 0) {
		return 0.2;
	} else {
		return result;
	}
}

float VillageGenerator::interest(Building &building) {
	float result = 0.0;
	float total = 0.0;
	float totalWeight = 0.0;
	float minresult = -0.1;

	if (!validPosition(building.getNode().getPosition())) {
		std::cout << "interest with invalid position ! " << building.getNode().getPosition() << std::endl;
		return 0.0;
	}

	if (building._sociabilityWeight) {
		result = sociabilityInterest(building);
		//if (result < minresult) return 0.0;
		total += building._sociabilityWeight * result;
		totalWeight += building._sociabilityWeight;
	}
	if (building._roadsWeight) {
		result = roadsInterest(building);
		//if (result < minresult) return 0.0;
		total += building._roadsWeight * result;
		totalWeight += building._roadsWeight;
	}
	if (building._dominationWeight) {
		result = dominationInterest(building);
		//if (result < minresult) return 0.0;
		total += building._dominationWeight * result;
		totalWeight += building._dominationWeight;
	}
	if (building._altitudeWeight) {
		result = altitudeInterest(building);
		//if (result < minresult) return 0.0;
		total += building._altitudeWeight * result;
		totalWeight += building._altitudeWeight;
	}
	if (building._sunWeight) {
		result = sunInterest(building);
		if (result < minresult) return 0.0;
		total += building._sunWeight * result;
		totalWeight += building._sunWeight;
	}
	if (building._fortificationWeight) {
		result = wallInterest(building);
		//if (result < minresult) return 0.0;
		total += building._fortificationWeight * result;
		totalWeight += building._fortificationWeight;
	}
	if (building._waterWeight) {
		result = waterInterest(building);
		//if (result < minresult) return 0.0;
		total += building._waterWeight * result;
		totalWeight += building._waterWeight;
	}
	if (building._cultWeight) {
		result = cultInterest(building);
		//if (result < minresult) return 0.0;
		total += building._cultWeight * result;
		totalWeight += building._cultWeight;
	}

	//BLIBLIBLI

	result = total; // / totalWeight;

	// Force the result in [0,1]
	result = MIN(1.0, result);
	result = MAX(0.0, result);

	// Mapping
	//result = sin(result*M_PI/2.0);
	//result = sin(result*result*M_PI/2.0);
	//result = result*result*result*result*result*result;
	result = result * result;

	// Threshold
	if (result < 0.05) {
		//result = 0.0;
	} else if (result > 0.9) {
		//result = 1.0;
	}

	return result;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              FEASABILITY
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

float VillageGenerator::feasability(Building &building) {
	float result = 1.0;
	result *= slopeFeasability(building);
	result *= spaceFeasability(building);
	result *= waterFeasability(building);
	result *= vegetationFeasability(building);
	result *= unbuildableFeasability(building);
	return result;
}

float VillageGenerator::slopeFeasability(Building &building) {
	float minHeight, maxHeight, total = 0.0;
	float currentHeight;
	float height;
	int nb = 0;

	float i = building.getNode().getPosition().x;
	float j = building.getNode().getPosition().z;

	height = maxHeight = minHeight = _heightmapLayout->getValueAt(Vector2F(i, j));
	for (int h = -building._spaceMin; h <= building._spaceMin; ++h) {
		for (int w = -building._spaceMin; w <= building._spaceMin; ++w) {
			if ((h == w) && (h == 0))
				continue;
			if (!_heightmapLayout->getImage()->validCoords(i + w, j + h))
				continue;
			if ((w * w + h * h) < building._spaceMin * building._spaceMin) {
				currentHeight = _heightmapLayout->getValueAt(Vector2F(i + w, j + h));
				minHeight = MIN(minHeight, currentHeight);
				maxHeight = MAX(maxHeight, currentHeight);
				total += currentHeight;

				if ((maxHeight - minHeight) > building._slopeVarMax)
					return 0.0;
			}
		}
	}

	return 1.0;
}

float VillageGenerator::spaceFeasability(Building &building) {
	LinkedList<Building> &buildings = _village.getBuildings();
	LinkedList<Road> &roads = _village.getRoads();
	LinkedList<Wall> &walls = _village.getWalls();
	if (buildings.getSize() == 0 && roads.getSize() == 0 && walls.getSize() == 0) {
		return 1.0;
	}

	float result = 0.0;
	Vector3F position = building.getNode().getPosition();
	float dist = 999999.0;

	// Buildings
	LinkedList<Building>::LinkedListIterator it(&buildings);
	while (it.hasNext()) {
		it.next();
		dist = MIN(norm(position - it.value().getNode().getPosition()) - it.value()._spaceMin, dist);
	}
	// Roads
	Vector2F position2;
	position2.x = building.getNode().getPosition().x;
	position2.y = building.getNode().getPosition().z;
	LinkedList<Road>::LinkedListIterator it2(&roads);
	while (it2.hasNext()) {
		it2.next();
		dist = MIN(dist, ABS(distanceToACurve(position2, it2.value()._path)));
	}

	// Walls
	LinkedList<Wall>::LinkedListIterator it3(&walls);
	while (it3.hasNext()) {
		it3.next();
		dist = MIN(dist, ABS(distanceToAPolygon(position2, it3.value()._nodes)));
	}

	float sociab;
	if (dist <= building._spaceMin) {
		return 0.0;
	} else {
		return 1.0;
	}
}
float VillageGenerator::vegetationFeasability(Building &building) {
	return 1.0;
	float i = building.getNode().getPosition().x;
	float j = building.getNode().getPosition().z;
	for (int h = -building._spaceMin; h <= building._spaceMin; ++h) {
		for (int w = -building._spaceMin; w <= building._spaceMin; ++w) {
			if ((w * w + h * h) < building._spaceMin * building._spaceMin) {
				if (!_vegetationLayout->getImage()->validCoords(i + w, j + h))
					continue;
				if (_vegetationLayout->getValueAt(Vector2F(i + w, j + h)) > 128) {
					return building._vegetationFeasability;
				}
			}
		}
	}
	return 1.0;
}
float VillageGenerator::waterFeasability(Building &building) {
	float i = building.getNode().getPosition().x;
	float j = building.getNode().getPosition().z;
	for (int h = -building._spaceMin; h <= building._spaceMin; ++h) {
		for (int w = -building._spaceMin; w <= building._spaceMin; ++w) {
			if ((w * w + h * h) < building._spaceMin * building._spaceMin) {
				if (!_waterLayout->getImage()->validCoords(i + w, j + h))
					continue;
				if (_waterLayout->getValueAt(Vector2F(i + w, j + h)) > 128) {
					return building._waterFeasability;
				}
			}
		}
	}
	return 1.0;
}
float VillageGenerator::unbuildableFeasability(Building &building) {
	float i = building.getNode().getPosition().x;
	float j = building.getNode().getPosition().z;
	for (int h = -building._spaceMin; h <= building._spaceMin; ++h) {
		for (int w = -building._spaceMin; w <= building._spaceMin; ++w) {
			if ((w * w + h * h) < building._spaceMin * building._spaceMin) {
				if (!_rockLayout->getImage()->validCoords(i + w, j + h))
					continue;
				if (_rockLayout->getValueAt(Vector2F(i + w, j + h)) > 128) {
					return 0.0;
				}
			}
		}
	}
	return 1.0;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              GENERATION
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void VillageGenerator::playAScenario(const Scenario &scenario) {
	//std::cout << "Play A scenario" << std::endl;
	LinkedList<Sequence> squences = scenario.getSequences();
	LinkedList<Sequence>::LinkedListIterator it(&squences);
	while (it.hasNext()) {
		it.next();
		playASequence(it.value());
	}
}

void VillageGenerator::playASequence(const Sequence &sequence) {
	//std::cout << "Play A Sequence" << std::endl;
	switch (sequence.getType()) {
		case Sequence::BUILDING_GENERATION: {
			std::cout << "Generating building" << std::endl;
			for (unsigned int i = 0; i < sequence.getGenerationNumber(); ++i) {
				Building building;
				_buildingsAtlas.updateParameters(building, sequence.getGenerationType());
				positioningBuilding(building);
			}
		} break;
		case Sequence::BUILDINGS_ATLAS_SELECTION: {
			std::cout << "Selection of the Building Atlas : " << sequence.getSelectionName() << std::endl;
			_buildingsAtlas = _buildingsAtlasList[std::string(sequence.getSelectionName())];
		} break;
		case Sequence::ROADS_ATLAS_SELECTION: {
			std::cout << "Selection of the Roads Atlas : " << sequence.getSelectionName() << std::endl;
			_roadsAtlas = _roadsAtlasList[std::string(sequence.getSelectionName())];
		} break;
		case Sequence::DIRECT_BUILDING_MODIFICATION: {
			std::cout << "Direct building creation" << std::endl;
			Building building;
			_buildingsAtlas.updateParameters(building, sequence.getCreationType());
			int x = sequence.getCreationX();
			int y = sequence.getCreationY();
			building.getNode().setPosition(Vector3F(x, 0, y));
			addBuilding(building);
		} break;
		case Sequence::WALL_CREATION: {
			Vector2F center = Vector2F(sequence.getWallX(), sequence.getWallY());
			float r = sequence.getWallRadius();
			LinkedList<Vector2F> points = createVertexListForJarvis(center, r);
			LinkedList<Vector2F> wallNodes = computeJarvis(points);
			Wall wall;
			wall._nodes = wallNodes;
			addWall(wall);
		} break;
	}
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              POSITIONING BUILDING
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void VillageGenerator::positioningBuilding(Building &building) {

	stochasticPositioning(building);
}

void VillageGenerator::initialPosition(Building &building) {
	int posX = rand() % (int)_width;
	int posY = rand() % (int)_height;
	Vector3F position = correctWithScale(Vector3F(posX, 0, posY));
	building.getNode().setPosition(position);
}

bool VillageGenerator::feasabilityTest(Building &building) {
	if (feasability(building) != 1.0) {
		return false;
	}
	return true;
}

bool VillageGenerator::aggregationTest(Building &building) {
	float value = interest(building);
	if (value < 0.5)
		return false;
	else if (value > 0.9)
		return true;
	else if (randomTest(value)) {
		std::cout << " >>>>  >>>>  Aggregated with : " << value << std::endl;
		return true;
	} else
		return false;
}

float VillageGenerator::aggregationProba(Building &building) {
	if (!feasabilityTest(building))
		return 0.0;
	return interest(building);
}

void VillageGenerator::stochPos(Building &building) {
	int nbTest = 0;
	do {
		initialPosition(building);
		++nbTest;
	} while (((!feasabilityTest(building)) || (!aggregationTest(building))) && nbTest < 20000);
	if (nbTest >= 20000) {
		std::cout << "Stochastic positionning failed !!!!!" << std::endl;
		nbTest = 0;
		do {
			initialPosition(building);
			++nbTest;
		} while (!feasabilityTest(building) && nbTest < 20000);
		if (nbTest >= 20000) {
			std::cout << "Stochastic positionning FEASABILITY failed !!!!!" << std::endl;
		}
	}
}

void VillageGenerator::stochasticPositioning(Building &building) {
	std::cout << "Stochastic position" << std::endl;
	stochPos(building);
	std::cout << "networkConnexion" << std::endl;
	networkConnexion(building);
	std::cout << "addBuilding" << std::endl;

	if (building._type == Building::HOUSE)
		positionCluster(building, 8);
	else if (building._type == Building::VILLA)
		positionCluster(building, 8);
	else if (building._type == Building::FIELD)
		positionCluster(building, 8);
	else
		addBuilding(building);
}

Vector3F VillageGenerator::getNeighbour(unsigned int neighbour) {
	Vector3F move;
	switch (neighbour) {
		case 0:
			move = Vector3F(-1, 0, -1);
			break;
		case 1:
			move = Vector3F(-1, 0, 1);
			break;
		case 2:
			move = Vector3F(1, 0, 1);
			break;
		case 3:
			move = Vector3F(1, 0, -1);
			break;
		case 4:
			move = Vector3F(-1, 0, 0);
			break;
		case 5:
			move = Vector3F(1, 0, 0);
			break;
		case 6:
			move = Vector3F(0, 0, -1);
			break;
		case 7:
			move = Vector3F(0, 0, 1);
			break;
	}
	return move;
}

void VillageGenerator::positionCluster(Building &building, int nb) {
	std::cout << "Cluster Positionning ! " << std::endl;
	int i = 0;
	while (i < nb) {
		int nbTestMax = 2000;
		int nbTests = 0;

		Vector2F pos1, pos2;
		Building current = building;

		while (nbTests < nbTestMax) {
			unsigned int neighbour = rand() % 8;
			Vector3F move = getNeighbour(neighbour);
			float coeff = 1.0;
			switch (building._type) {
				case Building::HOUSE:
					coeff = 1.0;
					break;
				case Building::VILLA:
					coeff = 2.0;
					break;
				case Building::FIELD:
					coeff = 4.0;
					break;
			}
			current.getNode().translate(coeff * move);
			nbTests++;

			if (validPosition(current.getNode().getPosition()) && feasabilityTest(current)) {
				if (_village.getRoads().getSize() == 0) {
					std::cout << "Cluster : add building ! " << std::endl;
					addBuilding(current);
					++i;
				} else {
					pos2 = findClosestRoadNode(current);
					pos1.x = current.getNode().getPosition().x;
					pos1.y = current.getNode().getPosition().z;

					//std::cout << "Closest : " << pos2 << std::endl;
					float dist = fabs(norm(pos1 - pos2) - current._spaceMin);
					if (dist > 3 && (dist - 3) <= coeff) {
						std::cout << "Cluster : add building ! " << std::endl;
						current.getNode().getPosition() = pos2 + coeff * normalize(pos1 - pos2);
						addBuilding(current);
						++i;
					}
				}
			}
		}
		++i;
	}
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//                              CONNEXION TO ROAD NETWORK
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
bool VillageGenerator::roadExistenceTest(Building &building) {
	if (_village.getRoads().getSize() == 0)
		return false;

	Vector2F nearest = findClosestRoadNode(building);
	Vector2F position;
	position.x = building.getNode().getPosition().x;
	position.y = building.getNode().getPosition().z;
	if (abs(norm(nearest - position) - building._spaceMin) <= 2.0)
		return true;
	return false;
}

Vector2F VillageGenerator::findClosestRoadNode(Building &building, Vector2F *normalResult) {
	if (_village.getRoads().getSize() == 0)
		return _waysToTheWorld[0];

	Vector2F position;
	position.x = building.getNode().getPosition().x;
	position.y = building.getNode().getPosition().z;

	LinkedList<Road> &roads = _village.getRoads();
	Vector2F near = roads.getFirst()._path.getFirst();
	Vector2F last = near;
	LinkedList<Road>::LinkedListIterator it(&roads);
	while (it.hasNext()) {
		it.next();
		LinkedList<Vector2F>::LinkedListIterator it2(&it.value()._path);
		Vector2F prev = it.value()._path.getFirst();
		while (it2.hasNext()) {
			it2.next();
			if (norm(it2.value() - position) < norm(near - position)) {
				near = it2.value();
				last = prev;
			}
			prev = it2.value();
		}
	}

	if (normalResult != 0) {
		//Vector3F cross = crossProduct(Vector3F(0,1,0), normalize(Vector3F(near.x - last.x, 0, near.y - last.y)));
		Vector2F cross = normalize(Vector2F(near.x - position.x, near.y - position.y));
		normalResult->x = cross.x;
		normalResult->y = cross.y;
	}

	return near;
}

Vector2F VillageGenerator::findClosestRoadNormal(Building &building) {
	if (_village.getRoads().getSize() == 0)
		return _waysToTheWorld[0];

	Vector2F position;
	position.x = building.getNode().getPosition().x;
	position.y = building.getNode().getPosition().z;

	LinkedList<Road> &roads = _village.getRoads();
	Vector2F near = roads.getFirst()._path.getFirst();
	Road road = roads.getFirst();
	LinkedList<Vector2F>::LinkedListIterator nearIt(&(road._path));
	LinkedList<Road>::LinkedListIterator it(&roads);
	while (it.hasNext()) {
		it.next();
		LinkedList<Vector2F>::LinkedListIterator it2(&it.value()._path);
		Vector2F prev = it.value()._path.getFirst();
		while (it2.hasNext()) {
			it2.next();
			if (norm(it2.value() - position) < norm(near - position)) {
				near = it2.value();
				nearIt = it2;
			}
		}
	}

	if (norm(near - position) > building._spaceMin * 5) {
		// cas 0
		std::cout << "cas 0" << std::endl;
		return Vector2F(0, 0);
	} else if (!nearIt.hasPrev() && nearIt.hasNext()) {
		// cas 1
		Vector2F next = nearIt.next();
		if (dotProduct(position - near, next - near) < 0) {
			// cas 1a
			std::cout << "cas 1a" << std::endl;
			return normalize(near - position);
		} else {
			// cas 1b
			std::cout << "cas 1b" << std::endl;
			Vector2F tangent = next - near;
			return normalize(Vector2F(-tangent.y, tangent.x));
		}
	} else if (!nearIt.hasNext() && nearIt.hasPrev()) {
		// cas 3
		Vector2F prev = nearIt.prev();
		if (dotProduct(position - near, near - prev) > 0) {
			// cas 3a
			std::cout << "cas 3a" << std::endl;
			return normalize(near - position);
		} else {
			// cas 3b
			std::cout << "cas 3b" << std::endl;
			Vector2F tangent = near - prev;
			return normalize(Vector2F(-tangent.y, tangent.x));
		}
	} else if (nearIt.hasNext() && nearIt.hasPrev()) {
		// cas 2
		nearIt.prev();
		Vector2F prev = nearIt.value();
		nearIt.next();
		nearIt.next();
		Vector2F next = nearIt.value();
		Vector2F tangent = normalize(prev - next);
		std::cout << "cas 2" << std::endl;
		return normalize(Vector2F(-tangent.y, tangent.x));
	} else {
		// cas 0 bis
		std::cout << "cas 0bis" << std::endl;
		return normalize(near - position);
	}

	return near;
}

void VillageGenerator::generateBuildingRoad(Building &building, const Vector2F &near) {
	std::cout << "generateBuildingRoad" << std::endl;
	Vector2F end = correctWithScale(near);
	Vector2F start;
	start.x = building.getNode().getPosition().x;
	start.y = building.getNode().getPosition().z;
	start = correctWithScale(start);

	if (building._type != Building::HOUSE) {
		start = start + (building._spaceMin + 1.f) * normalize(near - start);
		int nbTests = 0;
		while (nbTests < 500) {
			if (validPosition(start) && (_buildingsLayout->getValueAt(start) == 0))
				exit;
			else {
				start.x += (float(rand() % 1000) - 500.0) / 500.0;
				start.y += (float(rand() % 1000) - 500.0) / 500.0;
			}
			nbTests++;
		}
	}

	Road::Type type;
	switch (building._type) {
		case Building::HOUSE:
			type = Road::ROAD;
			break;
		case Building::VILLA:
			type = Road::ROAD;
			break;
		case Building::GARDEN:
			type = Road::DIRT_ROAD;
			break;

		case Building::TRADE:
			type = Road::STREET;
			break;

		case Building::CHURCH:
			type = Road::ROAD;
			break;
		case Building::STATUE:
			type = Road::TRAIL;
			break;

		case Building::FIELD:
			type = Road::DIRT_ROAD;
			break;
		case Building::WATER_MILL:
			type = Road::DIRT_ROAD;
			break;
		case Building::WIND_MILL:
			type = Road::DIRT_ROAD;
			break;

		case Building::TOWER_DEFENSE:
			type = Road::ROAD;
			break;
		case Building::CASTLE:
			type = Road::ROAD;
			break;
		default:
			type = Road::ROAD;
			break;
	}

	// SCALE !!
	unsigned int oldScale = _scale;
	GridedGraphNeighbourhood oldNeigh = _neighbourhood;
	switch (type) {
		case Road::ROAD:
			setScale(4);
			setNeighbourhood(THIRTY_TWO);
			break;
		case Road::DIRT_ROAD:
			setScale(4);
			setNeighbourhood(THIRTY_TWO);
			break;
		case Road::STREET:
			setScale(2);
			setNeighbourhood(THIRTY_TWO);
			break;
		case Road::TRAIL:
			setScale(2);
			setNeighbourhood(THIRTY_TWO);
			break;
	}

	LinkedList<Vector2F> path = generatePath(start, end, type);

	// SCALE BIS
	setScale(oldScale);
	setNeighbourhood(oldNeigh);

	std::cout << "path found" << std::endl;
	Road road(type, path);
	addRoad(road);
	std::cout << "road added" << std::endl;
}

void VillageGenerator::networkConnexion(Building &building) {
	if (roadExistenceTest(building))
		return;

	Vector2F near = findClosestRoadNode(building);

	generateBuildingRoad(building, near);
}

void VillageGenerator::betaSkeleton() {
	int n = _village.getBuildings().getSize();
	float e = 1.001;
	//float e = 19.15;

	if (n == 0)
		return;

	LinkedList<Building>::LinkedListIterator it(&_village.getBuildings());
	int i = 0;
	while (it.hasNext()) {
		it.next();
		Vector3F posI = it.value().getNode().getPosition();

		LinkedList<Building>::LinkedListIterator it2(&_village.getBuildings());
		int j = 0;
		while (it2.hasNext()) {
			it2.next();
			Vector3F posJ = it2.value().getNode().getPosition();

			if (i == j) {
				++j;
				continue;
			}

			float distIJ = norm(posJ - posI);

			LinkedList<Building>::LinkedListIterator it3(&_village.getBuildings());
			int k = 0;
			while (it3.hasNext()) {
				it3.next();
				Vector3F posK = it3.value().getNode().getPosition();

				if ((k == i) || (k == j)) {
					++k;
					continue;
				}

				float distIK = norm(posK - posI);
				float distJK = norm(posK - posJ);

				//if (exp(log(distIJ)*e) < exp(log(distIK)*e)+ exp(log(distJK)*e) )
				if (exp(log(distIJ) * e) > exp(log(distIK) * e) + exp(log(distJK) * e)) {
					Vector2I start;
					start.x = posI.x;
					start.y = posI.z;
					Vector2I end;
					end.x = posJ.x;
					end.y = posJ.z;

					Painter::drawBresenhamLine(start, end,
							_terrain->getRoads(),
							Vector3I(255, 255, 255),
							3.0);
					Painter::drawBresenhamLine(start, end,
							_roadsLayout->getImage(),
							Vector3I(255, 255, 255),
							3.0);
					std::cout << "i,j,k"
							  << i
							  << " "
							  << j
							  << " "
							  << k
							  << std::endl;
					std::cout << "TRUE" << std::endl;
					std::cout << "start " << start << std::endl;
					std::cout << "end " << end << std::endl;
				}
				++k;
			}
			++j;
		}
		++i;
	}
}

void VillageGenerator::watershed() {
	_watershed->perform();
}

void VillageGenerator::shortcuts() {
	if (_village.getRoads().getSize() == 0)
		return;

	float maxDist = 20;
	//std::cout << "Shortcut ?"<< std::endl;

	Vector2F start1, end1, start2, end2;
	LinkedList<Road> &roads = _village.getRoads();
	LinkedList<Road> roadsToAdd;
	LinkedList<Road>::LinkedListIterator it(&roads);
	int nb1 = 0;
	while (it.hasNext()) {
		it.next();
		start1 = it.value()._path.getFirst();
		end1 = it.value()._path.getLast();

		LinkedList<Road>::LinkedListIterator itBIS(&roads);
		int nb2 = 0;
		while (itBIS.hasNext()) {
			itBIS.next();
			if (nb1 == nb2)
				continue;

			start2 = itBIS.value()._path.getFirst();
			end2 = itBIS.value()._path.getLast();

			if (norm(start1 - start2) < maxDist && norm(start1 - start2) > 0) {
				std::cout << "Shortcut between " << start1 << " and " << start2 << std::endl;
				LinkedList<Vector2F> path = generatePath(start1, start2, it.value()._type);
				Road road(it.value()._type, path);
				roadsToAdd.pushLast(road);
			}

			if (norm(start1 - end2) < maxDist && norm(start1 - end2) > 0) {
				std::cout << "Shortcut between " << start1 << " and " << end2 << std::endl;
				LinkedList<Vector2F> path = generatePath(start1, end2, it.value()._type);
				Road road(it.value()._type, path);
				roadsToAdd.pushLast(road);
			}

			if (norm(end1 - start2) < maxDist && norm(end1 - start2) > 0) {
				std::cout << "Shortcut between " << end1 << " and " << start2 << std::endl;
				LinkedList<Vector2F> path = generatePath(end1, start2, it.value()._type);
				Road road(it.value()._type, path);
				roadsToAdd.pushLast(road);
			}

			if (norm(end1 - end2) < maxDist && norm(end1 - end2) > 0) {
				std::cout << "Shortcut between " << end1 << " and " << end2 << std::endl;
				LinkedList<Vector2F> path = generatePath(end1, end2, it.value()._type);
				Road road(it.value()._type, path);
				roadsToAdd.pushLast(road);
			}

			++nb2;
		}
		++nb1;
	}
	LinkedList<Road>::LinkedListIterator it2(&roadsToAdd);
	while (it2.hasNext()) {
		it2.next();
		addRoad(it2.value());
	}
}

LinkedList<Vector2I> VillageGenerator::generatePath(Vector2F source,
		Vector2F dest,
		Road::Type roadType) {

	// INITIALIZE
	LinkedList<Pair<CostFunctor<float, Vector2I, Vector3F> *, Layout<float, Vector2I, Vector3F> *> > costLayouts;

	// GRADIENT
	Pair<CostFunctor<float, Vector2I, Vector3F> *, Layout<float, Vector2I, Vector3F> *> gradient(_terrainFunctor, _heightmapLayout);
	costLayouts.pushLast(gradient);

	std::cout << "Road(roadType); " << roadType << " = " << Road::ROAD << " val : " << _roadsAtlas.getParameter(roadType, RoadsAtlas::_curvatureWeight) << std::endl;
	Road road = Road(roadType);
	_roadsAtlas.updateParameters(road, roadType);
	_terrainFunctor->setRoad(road);

	//LinkedList<Building>& buildings = _village.getBuildings();
	//_terrainFunctor->setBuildings(&buildings);

	_terrainFunctor->setRoadsLayout(_roadsLayout);
	_terrainFunctor->setWaterLayout(_waterLayout);
	_terrainFunctor->setRockLayout(_rockLayout);
	_terrainFunctor->setVegetationLayout(_vegetationLayout);
	_terrainFunctor->setBuildingsLayout(_buildingsLayout);
	_terrainFunctor->setWallsLayout(_wallsLayout);

	source = correctWithScale(source);
	dest = correctWithScale(dest);

	// GENERATE THE PATH
	return PathFinding<float, Vector2I, Vector3F>::aStar(source, dest, _landscapeGraph, costLayouts);
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void VillageGenerator::_initWaysToTheWorld() {
	_waysToTheWorld.empty();

	if (!_terrain)
		return;

	unsigned int nb = (rand() % 2) + 1;

	std::cout << "Random number of WaysToTheWorld: " << nb << std::endl;

	for (unsigned int i = 0; i < nb; ++i) {
		// Which edge of the map
		int edge = (rand() % 4);
		int height, width;
		switch (edge) {
			case 0:
				width = 0;
				height = rand() % int(_height);
				break;
			case 1:
				width = _width - 1;
				height = rand() % int(_height);
				break;
			case 2:
				width = rand() % int(_width);
				height = 0;
				break;
			case 3:
				width = rand() % int(_width);
				height = _height - 1;
				break;
		}
		_waysToTheWorld.pushLast(correctWithScale(Vector2F(width, height)));
	}
}

void VillageGenerator::save(std::string file) {
	_village.save(file);
}

void VillageGenerator::load(std::string fileName) {
	std::ifstream file(fileName.c_str());

	if (!file.is_open()) {
		std::cout << "Error opening file." << std::endl;
	}

	std::string buffer;

	std::cout << "Loading village " << std::endl;

	// ROADS
	file >> buffer;
	file >> buffer;
	int nbRoads;
	file >> nbRoads;
	std::cout << "nbRoads " << nbRoads << std::endl;
	Vector2F node;
	for (unsigned int i = 0; i < nbRoads; ++i) {
		Road road;
		std::cout << "Road i : " << i << std::endl;
		file >> buffer;
		int type;
		file >> type;
		road._type = (Road::Type)type;
		file >> buffer;
		int nbNodes;
		file >> nbNodes;
		file >> buffer;
		for (unsigned int n = 0; n < nbNodes; ++n) {
			file >> node.x;
			file >> node.y;
			road._path.pushLast(node);
		}
		addRoad(road, false, false);
	}

	// BUILDINGS
	file >> buffer;
	file >> buffer;
	int nbBuildings;
	file >> nbBuildings;
	std::cout << "nbBuildings" << nbBuildings << std::endl;
	Vector3F pos;
	Quaternion rot;
	for (unsigned int i = 0; i < nbBuildings; ++i) {
		Building building;
		std::cout << "Building i : " << i << std::endl;
		file >> buffer;
		int type;
		file >> type;
		file >> buffer;
		file >> building._spaceMin;
		building._type = (Building::Type)type;
		file >> buffer;
		file >> pos.x;
		file >> pos.y;
		file >> pos.z;
		building.getNode().setPosition(pos);
		file >> buffer;
		file >> rot.angle;
		file >> rot.vector.x;
		file >> rot.vector.y;
		file >> rot.vector.z;
		building.getNode().setOrientation(rot);

		addBuilding(building);
	}

	file.close();

	drawRoads();

	std::cout << "Loading complete " << std::endl;
}

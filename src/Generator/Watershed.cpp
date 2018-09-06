/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Watershed.h"
#include "../Image/Painter.h"
#include "../Maths/Calcul.h"
#include "../Maths/Quaternion.h"
#include "../Maths/TransformationMatrix.h"
#include "../Utils/Heap.h"
#include "VillageGenerator.h"

Watershed::Watershed(unsigned int width,
		unsigned int height)
		: _width(width), _height(height) {
	_image = new Image(_width, _height, Color::RGB_32);
	_image->fill(Color::RGBF(0.0, 0.0, 0.0));
}

void complete(LinkedList<Vector2F> &points) {
	LinkedList<Vector2F>::LinkedListIterator it(&points);
	while (it.hasNext()) {
		it.next();
		Vector2F current = it.value();
		points.pushFirst(current + Vector2F(0.5, 0.5));
		points.pushFirst(current + Vector2F(0.5, -0.5));
		points.pushFirst(current + Vector2F(-0.5, 0.5));
		points.pushFirst(current + Vector2F(-0.5, -0.5));
	}
}

void Watershed::addSource(LinkedList<Building>::LinkedListIterator building,
		Vector2F pos,
		Vector2F normal,
		float max) {
	Source source;
	source.building = building;
	source.id = _sources.getSize() + 1;
	source.pos = pos;
	source.max = max;
	source.origin = pos;
	source.distToOrigin = 0;

	std::cout << "add source : normal : " << normal << std::endl;

	std::cout << "id "
			  << source.id
			  << " pos "
			  << source.pos
			  << " max "
			  << source.max
			  << " origin "
			  << source.origin
			  << std::endl;

	//normal = Vector2I(1.0, 0.0);

	source.normal = normalize(normal);

	source.tangent.x = -normal.y;
	source.tangent.y = normal.x;

	_sources.pushLast(source);
}

void Watershed::addRoad(Road road) {
	_roads.pushLast(road);
}

Watershed::Source *Watershed::neighbour(Watershed::Source *source, float dx, float dy) {
	Source *neigh = new Source(*source);
	neigh->pos = source->pos + Vector2F(dx * source->normal + dy * source->tangent);
	neigh->distToOrigin = norm(neigh->pos - neigh->origin);
	return neigh;
}

bool Watershed::validNeigh(Watershed::Source *neigh) {
	if (!_image->validCoords(neigh->pos))
		return false;
	else if (_image->getAt(neigh->pos).rf() == 0.0)
		return true;
	else
		return false;
}

void Watershed::addNeigh(Watershed::Source *source, float dx, float dy, std::priority_queue<Source *, vector<Source *>, Comparator> &result) {
	Source *neigh = neighbour(source, dx, dy);
	if (validNeigh(neigh)) {
		_image->setAt(neigh->pos, Color::RGBF(1.0, 0.0, float(neigh->id) / float(_sources.getSize() + 1)));
		result.push(neigh);
	}
}

void Watershed::neighbours(Watershed::Source *source, std::priority_queue<Source *, vector<Source *>, Comparator> &result) {
	if ((fabs(dotProduct(Vector2F(source->pos - source->origin), source->normal)) > source->max) || (fabs(dotProduct(Vector2F(source->pos - source->origin), source->tangent)) > source->max))
		return;

	addNeigh(source, +1.0, +0.0, result);
	addNeigh(source, -1.0, +0.0, result);
	addNeigh(source, +0.0, +1.0, result);
	addNeigh(source, +0.0, -1.0, result);

	addNeigh(source, +1.0, +1.0, result);
	addNeigh(source, -1.0, -1.0, result);
	addNeigh(source, -1.0, +1.0, result);
	addNeigh(source, +1.0, -1.0, result);

	addNeigh(source, +0.75, +0.0, result);
	addNeigh(source, -0.75, +0.0, result);
	addNeigh(source, +0.0, +0.75, result);
	addNeigh(source, +0.0, -0.75, result);
}

void Watershed::paintRoad(Road &road) {
	int width;
	switch (road._type) {
		case Road::TRAIL:
			width = 1;
			break;
		case Road::DIRT_ROAD:
			width = 1;
			break;
		case Road::ROAD:
			width = 2;
			break;
		case Road::STREET:
			width = 1;
			break;
	}

	LinkedList<Vector2F> nodes(road._path);
	LinkedList<Vector2F>::LinkedListIterator it2(&nodes);
	if (!it2.hasNext())
		return;
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
					_image,
					Vector3I(1, 0, 0));
		}
		start = end;
	}
}

void Watershed::getQuadIn(unsigned int id) {
	std::cout << "getQuadIn " << id << std::endl;
	LinkedList<Vector2F> points = findPoints(id);
	LinkedList<Vector2F> polygon = computeJarvis(points);

	Vector3F pos = _sources[id - 1].building.value().getNode().getPosition();
	Vector2F HousePos;
	HousePos.x = pos.x;
	HousePos.y = pos.z;
	float size = 6;

	std::cout << " HousePos " << HousePos << std::endl;

	Vector2F n = _sources[id - 1].normal;
	Vector2F t = _sources[id - 1].tangent;

	bool ok = true;
	while (size > 3 && ok) {

		if ((distanceToAPolygon(HousePos + size * (n + t), polygon) < 0.0) && (distanceToAPolygon(HousePos + size * (n - 1.f * t), polygon) < 0.0) && (distanceToAPolygon(HousePos + size * (-1.f * n + t), polygon) < 0.0) && (distanceToAPolygon(HousePos + size * (-1.f * n - 1.f * t), polygon) < 0.0)) {
			ok = false;
		}

		--size;
	}

	_sources[id - 1].building.value()._2DHousePrint.empty();
	if (size <= 3)
		return;

	std::cout << "Add house print" << std::endl;

	_sources[id - 1].building.value()._2DHousePrint.pushLast(Vector2F(HousePos + size * (-1.f * n + t)));
	_sources[id - 1].building.value()._2DHousePrint.pushLast(Vector2F(HousePos + size * (-1.f * n - 1.f * t)));
	_sources[id - 1].building.value()._2DHousePrint.pushLast(Vector2F(HousePos + size * (n - 1.f * t)));
	_sources[id - 1].building.value()._2DHousePrint.pushLast(Vector2F(HousePos + size * (n + t)));
}

void Watershed::getPolygon(unsigned int id) {
	std::cout << "getPolygon " << id << std::endl;
	_sources[id - 1].building.value()._2DFootPrint.empty();

	LinkedList<Vector2F> points = findPoints(id);
	//simplify(points);
	if (points.getSize() == 0)
		std::cout << "No points found for id = " << id << std::endl;

	if (points.getSize() == 0)
		return;

	LinkedList<Vector2F> toadd = points;
	Vector2F to;
	while (toadd.getSize() > 0) {
		to = toadd.popLast();
		//std::cout << "to : " << to << std::endl;
		_sources[id - 1].building.value()._2DFootPrint.pushLast(to);
	}
}

void Watershed::quadrify(unsigned int id) {
	std::cout << "Quadrify " << id << std::endl;
	_sources[id - 1].building.value()._2DFootPrint.empty();

	LinkedList<Vector2F> points = findPoints(id);
	if (points.getSize() == 0)
		std::cout << "No points found for id = " << id << std::endl;

	// ADD points
	LinkedList<Vector2F> polygon = computeJarvis(points);

	if (points.getSize() == 0)
		return;

	LinkedList<Vector2F> toadd = findQuad2(polygon);

	Vector2F to;
	while (toadd.getSize() > 0) {
		to = toadd.popLast();
		std::cout << "to : " << to << std::endl;
		_sources[id - 1].building.value()._2DFootPrint.pushLast(to);
	}
}

void Watershed::perform() {
	// Init
	std::cout << "perform" << std::endl;

	_sources.empty();
	_roads.empty();

	LinkedList<Road> roads = VillageGenerator::_instance->_village.getRoads();
	LinkedList<Road>::LinkedListIterator it(&roads);
	while (it.hasNext()) {
		it.next();
		addRoad(it.value());
	}

	LinkedList<Building> &buildings = VillageGenerator::_instance->_village.getBuildings();
	LinkedList<Building>::LinkedListIterator it2(&buildings);
	while (it2.hasNext()) {
		it2.next();
		Building building = it2.value();

		float max = float(building._spaceMin) * 2.0;
		switch (building._type) {
			case Building::HOUSE:
				max = float(building._spaceMin) * 2.0;
				break;
			case Building::FIELD:
				max = float(building._spaceMin) * 3.0;
				break;
		}

		building.faceToRoad();

		Vector2F pos;
		pos.x = building.getNode().getPosition().x;
		pos.y = building.getNode().getPosition().z;
		Vector2F forward2D;
		forward2D.x = building.getNode().getForward().x;
		forward2D.y = -building.getNode().getForward().z;
		addSource(it2, pos, forward2D, max);
	}

	/*
	*/

	//LinkedList<Source> toDo;
	std::priority_queue<Source *, vector<Source *>, Comparator> toDo;
	for (unsigned int i = 0; i < _sources.getSize(); ++i) {
		//toDo.pushLast(_sources[i]);
		toDo.push(new Source(_sources[i]));
	}

	// Clear
	_image->fill(Color::RGBF(0.0, 0.0, 0.0));

	// Paint roads
	for (unsigned int i = 0; i < _roads.getSize(); ++i) {
		paintRoad(_roads[i]);
	}

	// Algo
	//while (toDo.getSize() > 0)
	std::cout << "algo" << std::endl;
	while (toDo.size() > 0) {
		Source *current = toDo.top();
		toDo.pop();

		neighbours(current, toDo);

		delete current;
	}

	// Convex areas
	std::cout << "quadrify 1" << std::endl;
	for (unsigned int i = 1; i <= _sources.getSize(); ++i) {
		if (_sources[i - 1].building.value()._type == Building::HOUSE) {
			//getPolygon(i);
			convexify(i);
			quadrify(i);
		} else if (_sources[i - 1].building.value()._type == Building::CHURCH || _sources[i - 1].building.value()._type == Building::VILLA) {
			getQuadIn(i);
			getPolygon(i);
			//convexify(i);
			//quadrify(i);
		} else {
			getPolygon(i);
		}
	}
}

struct toRemoveee {
	LinkedList<Vector2F>::LinkedListIterator it;
	float angle;
	toRemoveee()
			: it(NULL) {}
};

bool operator>(const toRemoveee &a, const toRemoveee &b) {
	return fabs(a.angle) < fabs(b.angle);
}

bool operator<(const toRemoveee &a, const toRemoveee &b) {
	return fabs(a.angle) > fabs(b.angle);
}

void Watershed::simplify(LinkedList<Vector2F> &points) {
	bool loop1 = true;
	while (loop1) {
		loop1 = false;
		bool loop2 = true;

		LinkedList<Vector2F>::LinkedListIterator it(&points);
		while (it.hasNext() && loop2) {
			it.next();

			Vector2F prev;
			Vector2F current;
			Vector2F next;

			if (it.hasPrev() && it.hasNext()) {
				it.prev();
				prev = it.value();
				it.next();
				current = it.value();
				it.next();
				next = it.value();
				it.prev();
			} else if (it.hasPrev()) {
				it.prev();
				prev = it.value();
				it.next();
				current = it.value();

				next = points.getFirst();
			} else if (it.hasNext()) {
				it.next();
				next = it.value();
				it.prev();
				current = it.value();

				prev = points.getLast();
			}

			float angle = fabs(orientedAngle(normalize(current - prev), normalize(next - current)));
			if (angle < 1.0) {
				loop1 = true;
				loop2 = false;
				points.removeAt(it);
			}
		}
	}
}

LinkedList<Vector2F> Watershed::findQuad(LinkedList<Vector2F> points) {
	// REMOVE DOUBLON
	points.popFirst();

	LinkedList<Vector2F> result;
	if (points.getSize() < 4) {
		return result;
	} else if (points.getSize() == 4) {
		return points;
	}
	// INIT
	while (points.getSize() > 4) {
		Heap<toRemoveee> heap;
		LinkedList<Vector2F>::LinkedListIterator it(&points);
		while (it.hasNext()) {
			it.next();

			Vector2F prev;
			Vector2F current;
			Vector2F next;
			toRemoveee to;

			if (it.hasPrev() && it.hasNext()) {
				to.it = it;

				it.prev();
				prev = it.value();
				it.next();
				current = it.value();
				it.next();
				next = it.value();
				it.prev();
			} else if (it.hasPrev()) {
				to.it = it;

				it.prev();
				prev = it.value();
				it.next();
				current = it.value();

				next = points.getFirst();
			} else if (it.hasNext()) {
				to.it = it;

				it.next();
				next = it.value();
				it.prev();
				current = it.value();

				prev = points.getLast();
			}

			to.angle = fabs(orientedAngle(normalize(current - prev), normalize(next - current)));
			if (to.angle != to.angle) {
				to.angle = 0;
			}
			heap.insert(to);
		}
		toRemoveee to = heap.popFirst();
		LinkedList<Vector2F>::LinkedListIterator it2 = to.it;
		std::cout << "remove angle : " << to.angle << std::endl;
		points.removeAt(it2);
	}

	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	return result;
}

LinkedList<Vector2F> Watershed::findQuad2(LinkedList<Vector2F> points) {
	// REMOVE DOUBLON
	points.popFirst();

	LinkedList<Vector2F> result;
	if (points.getSize() < 4) {
		return result;
	} else if (points.getSize() == 4) {
		return points;
	}
	// INIT
	while (points.getSize() > 4) {
		Heap<toRemoveee> heap;
		LinkedList<Vector2F>::LinkedListIterator it(&points);
		while (it.hasNext()) {
			it.next();

			Vector2F prev;
			Vector2F current;
			Vector2F next;
			toRemoveee to;

			if (it.hasPrev() && it.hasNext()) {
				to.it = it;

				it.prev();
				prev = it.value();
				it.next();
				current = it.value();
				it.next();
				next = it.value();
				it.prev();
			} else if (it.hasPrev()) {
				to.it = it;

				it.prev();
				prev = it.value();
				it.next();
				current = it.value();

				next = points.getFirst();
			} else if (it.hasNext()) {
				to.it = it;

				it.next();
				next = it.value();
				it.prev();
				current = it.value();

				prev = points.getLast();
			}

			// calcul du volume perdu
			// HERON
			float a = norm(current - prev);
			float b = norm(current - next);
			float c = norm(next - prev);

			float s = 0.5 * (a + b + c);
			float aire = sqrt(s * (s - a) * (s - b) * (s - c));
			to.angle = aire;

			if (to.angle != to.angle) {
				to.angle = 0;
			}
			heap.insert(to);
		}
		toRemoveee to = heap.popFirst();
		LinkedList<Vector2F>::LinkedListIterator it2 = to.it;
		std::cout << "remove angle : " << to.angle << std::endl;
		points.removeAt(it2);
	}

	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	result.pushLast(points.popFirst());
	return result;
}

LinkedList<Vector2F> Watershed::findPoints(unsigned int id) {
	float idvalue = float(id) / float(_sources.getSize() + 1);
	float inf = 1.0 / float(_sources.getSize() + 2);

	//std::cout << "Finding index with  id : " << id << " as : "  << float(id) / float(_sources.getSize() + 2) << std::endl;
	LinkedList<Vector2F> result;
	for (unsigned int w = 0; w < _image->getWidth(); ++w) {
		for (unsigned int h = 0; h < _image->getHeight(); ++h) {
			if (fabs(_image->getAt(w, h).bf() - (idvalue)) < inf) {
				if ((fabs(_image->getAt(w + 1, h).bf() - (idvalue)) < inf) && (fabs(_image->getAt(w - 1, h).bf() - (idvalue)) < inf) && (fabs(_image->getAt(w, h + 1).bf() - (idvalue)) < inf) && (fabs(_image->getAt(w, h - 1).bf() - (idvalue)) < inf)) {
				} else {
					int neigh = 0;
					float offx = 0;
					float offy = 0;
					if (_sources[id - 1].building.value()._type != Building::HOUSE) {

						if (fabs(_image->getAt(w + 1, h).bf() - (idvalue)) > inf && (fabs(_image->getAt(w - 1, h).bf() - (idvalue)) < inf)) {
							offx = 0.5;
							neigh++;
						}
						if (fabs(_image->getAt(w + 1, h).bf() - (idvalue)) < inf && (fabs(_image->getAt(w - 1, h).bf() - (idvalue)) > inf)) {
							offx = -0.5;
							neigh++;
						}
						if (fabs(_image->getAt(w, h + 1).bf() - (idvalue)) < inf && (fabs(_image->getAt(w, h - 1).bf() - (idvalue)) > inf)) {
							offy = -0.5;
							neigh++;
						}
						if (fabs(_image->getAt(w, h + 1).bf() - (idvalue)) > inf && (fabs(_image->getAt(w, h - 1).bf() - (idvalue)) < inf)) {
							offy = 0.5;
							neigh++;
						}
						//if (neigh < 2)
						result.pushLast(Vector2F(float(w) + offx, float(h) + offy));
						//result.sortedInsert(Vector2F(w,h));
					} else {
						result.pushLast(Vector2F(float(w), float(h)));
					}
				}
			}
		}
	}

	if (result.getSize() == 0)
		return result;

	// SORT !
	LinkedList<Vector2F> result2;
	Vector2F current = result.popFirst();
	while (result.getSize() > 1) {
		bool found = false;
		LinkedList<Vector2F>::LinkedListIterator minIt(&result);
		LinkedList<Vector2F>::LinkedListIterator min(&result);
		min.next();
		minIt.next();
		LinkedList<LinkedList<Vector2F>::LinkedListIterator> toTest;

		while (minIt.hasNext()) {
			minIt.next();
			if (norm(current - minIt.value()) < norm(current - min.value())) {
				min = minIt;
			}
		}

		if (norm(current - min.value()) > 5.0) {
			result.removeAt(min);
		} else {
			current = min.value();
			result2.pushLast(current);
			result.removeAt(min);
		}
	}

	//
	//result2.pushLast(result.popFirst());
	return result2;
}

void Watershed::convexify(unsigned int id) {
	LinkedList<Vector2F> points = findPoints(id);
	if (points.getSize() == 0)
		std::cout << "No points found for id = " << id << std::endl;

	// ADD points
	//complete(points);
	// JARVIS
	LinkedList<Vector2F> polygon = computeJarvis(points);

	if (polygon.getSize() == 0)
		return;

	int minX, maxX, minY, maxY;
	minX = polygon.getFirst().x;
	minY = polygon.getFirst().y;
	maxX = polygon.getFirst().x;
	maxY = polygon.getFirst().y;

	LinkedList<Vector2F>::LinkedListIterator it(&polygon);
	while (it.hasNext()) {
		it.next();
		std::cout << " id :" << id << " pt " << it.value() << std::endl;
		minX = MIN(minX, it.value().x);
		minY = MIN(minY, it.value().y);
		maxX = MAX(maxX, it.value().x);
		maxY = MAX(maxY, it.value().y);
	}

	for (unsigned int w = minX; w <= maxX; ++w) {
		for (unsigned int h = minY; h <= maxY; ++h) {
			if (distanceToAPolygon(Vector2F(w, h), polygon) <= 0.0) {
				_image->setAt(Vector2F(w, h), Color::RGBF(1.0, 1.0, float(id) / float(_sources.getSize() + 1)));
			}
		}
	}
}

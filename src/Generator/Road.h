/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _ROAD_H
#define _ROAD_H

#include "../Maths/Vector.h"
#include "../Utils/LinkedList.h"

class Road {
public:
	enum Type { TRAIL,
		STAIRS,
		DIRT_ROAD,
		ROAD,
		STREET,
		CUSTOM };

public:
	float _travelWeight;
	float _curvatureWeight;
	float _curvatureMax;
	float _slopeWeight;
	float _slopeMax;
	float _waterWeight;
	float _roadsWeight;

	LinkedList<Vector2F> _path;

	Type _type;

public:
	Road(Type type, LinkedList<Vector2F> path)
			: _path(path), _type(type) {
	}

	Road(Type type = ROAD)
			: _type(type) {
	}

	template <class C, class K, class V>
	friend class RoadCostFunctor;

}; // class Road

#endif // _ROAD_H

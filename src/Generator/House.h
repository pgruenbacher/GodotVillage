/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _HOUSE_H
#define _HOUSE_H

#include "../Engine/BoundingBox2D.h"
#include "../Engine/Object3D.h"

#include <iostream>

struct HouseType {
	double waterInterestWeight;
	double heightInterestWeight;
	double roadsInterestWeight;
	double slopeInterestWeight;
};

class House : public Object3D {
private:
	Table<Vector2I> _buildingBorder;
	Table<Vector2I> _border;
	unsigned int _age;
	BoundingBox2D *_boundingBox;

public:
	House() {
	}

	House(const Table<Vector2I> &border)
			: _border(border) {
		_computeBoundingBox();
	}

	void _computeBoundingBox() {
		Vector2F zero;
		Vector2F max;

		unsigned int size = _border.getSize();
		if (size == 0)
			return;

		zero = _border[0];
		max = _border[0];
		for (unsigned int i = 1; i < size; ++i) {
			if (_border[i].x < zero.x) {
				zero.x = _border[i].x;
			} else if (_border[i].x > max.x) {
				max.x = _border[i].x;
			}
			if (_border[i].y < zero.y) {
				zero.y = _border[i].y;
			} else if (_border[i].y > max.y) {
				max.y = _border[i].y;
			}
		}
		_boundingBox = new BoundingBox2D(zero, max.x - zero.x, max.y - zero.y, &_node);
	}

	Table<Vector2I> getBorder() const {
		return _border;
	}

	BoundingBox2D *getBoundingBox2D() {
		return _boundingBox;
	}

	BoundingBox2D getBoundingBox2D() const {
		return *_boundingBox;
	}

	friend std::ostream &operator<<(std::ostream &o, const House &house);

}; // class House

#endif // _HOUSE_H

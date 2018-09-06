/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _BOUNDING_BOX_2D_H
#define _BOUNDING_BOX_2D_H

#include "Node.h"

class BoundingBox2D {
private:
	Vector2F _zero;
	float _width;
	float _height;
	Node *_node;

public:
	Vector2F getZero() const {
		return _zero;
	}
	float getWidth() const {
		return _width;
	}
	float getHeight() const {
		return _height;
	}
	Node *getNode() {
		return _node;
	}
	Node getNode() const {
		return *_node;
	}
	BoundingBox2D(Vector2F zero, float width, float height, Node *node)
			: _zero(zero), _width(width), _height(height), _node(node) {
	}

	bool contains(Vector2F point) const {
		if ((_zero.x + _node->getPosition().x <= point.x) && (_zero.x + _node->getPosition().x + _width >= point.x) && (_zero.y + _node->getPosition().y <= point.y) && (_zero.y + _node->getPosition().y + _height >= point.y)) {
			return true;
		}

		return false;
	}

	bool collides(const BoundingBox2D &box) {
		// Check if one point of the box is inside the current box
		Vector2F p1, p2, p3, p4;
		p1 = box._zero + box._node->getPosition();
		p2 = box._zero + box._node->getPosition() + Vector2F(0.0, box._height);
		p3 = box._zero + box._node->getPosition() + Vector2F(box._width, 0.0);
		p4 = box._zero + box._node->getPosition() + Vector2F(box._width, box._height);

		if (contains(p1) || contains(p2) || contains(p3) || contains(p4))
			return true;

		// Check if one point of the current box is inside the box
		else {
			p1 = _zero + _node->getPosition();
			p2 = _zero + _node->getPosition() + Vector2F(0.0, _height);
			p3 = _zero + _node->getPosition() + Vector2F(_width, 0.0);
			p4 = _zero + _node->getPosition() + Vector2F(_width, _height);

			if (box.contains(p1) || box.contains(p2) || box.contains(p3) || box.contains(p4))
				return true;
		}

		return false;
	}

}; // class BoundingBox2D

#endif // _BOUNDING_BOX_2D_H

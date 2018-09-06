/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "HousesLayout.h"
#include "../Engine/BoundingBox2D.h"
#include "../Image/Painter.h"
#include <QRgb>

HousesLayout::HousesLayout(Image *image)
		: ImageLayout<float, Vector2I, Vector3F>(image) {
}

void HousesLayout::_renderHouse(House &house) {
	// Draw a line for each edge of the house.
	Table<Vector2I> polygonToDraw = house.getBorder();
	unsigned int size = house.getBorder().getSize();
	for (unsigned int i = 0; i < size; ++i) {
		polygonToDraw[i] = polygonToDraw[i] + Vector2I(house.getNode().getPosition());
	}
	Vector3I color1(255, 255, 255);
	Painter::drawFilledPolygon(polygonToDraw, _image, color1);
	Vector3I color2(255, 255, 255);
	Painter::drawEmptyPolygon(polygonToDraw, _image, color2);
}

House HousesLayout::_adjustHouse(const House &house) {
	Table<House *> inCollision;
	for (unsigned int i = 0; i < _houses.getSize(); ++i) {
		// If a house is in the box
		if (_houses[i].getBoundingBox2D()->collides(house.getBoundingBox2D())) {
			inCollision.pushLast(&_houses[i]);
		}
	}
	return house;
}

Table<House *> HousesLayout::inCollisionWith(const House &house) {
	Table<House *> inCollision;
	for (unsigned int i = 0; i < _houses.getSize(); ++i) {
		// If a house is in the box
		if (_houses[i].getBoundingBox2D()->collides(house.getBoundingBox2D())) {
			inCollision.pushLast(&_houses[i]);
		}
	}
	return inCollision;
}

void HousesLayout::addHouse(const House &house) {
	House correctHouse = _adjustHouse(house);
	_houses.pushLast(correctHouse);
	updateImage(*(correctHouse.getBoundingBox2D()));
}

void HousesLayout::updateImage(const BoundingBox2D &updateArea) {
	// Clear the image
	Vector3F bottomLeft;
	Vector3F topRight;

	bottomLeft = updateArea.getZero() + updateArea.getNode().getPosition();
	topRight = updateArea.getZero() + updateArea.getNode().getPosition() + Vector2F(updateArea.getWidth(), updateArea.getHeight());

	Vector3I color(0, 0, 0);
	Painter::drawRectangle(bottomLeft, topRight, _image, color);

	// Draw houses
	unsigned int size = _houses.getSize();
	for (unsigned int i = 0; i < size; ++i) {
		// If a house is in the box
		if (_houses[i].getBoundingBox2D()->collides(updateArea)) {
			// Draw the house
			_renderHouse(_houses[i]);
		}
	}
}

void HousesLayout::updateImage(unsigned int x0,
		unsigned int y0,
		unsigned int width,
		unsigned int height) {
	// Check
	if (x0 > _image->getWidth() || y0 > _image->getHeight()) {
		return;
	}

	// Correct the maximum width
	if (x0 + width > _image->getWidth()) {
		width = _image->getWidth() - x0;
	}

	// Correct the maximum height
	if (y0 + height > _image->getHeight()) {
		height = _image->getHeight() - y0;
	}

	// Create a boundingbox for the update zone
	Node zeroNode;
	BoundingBox2D updateArea(Vector2F(x0, y0), width, height, &zeroNode);
	updateImage(updateArea);
}

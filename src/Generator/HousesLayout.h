#ifndef _HOUSES_LAYOUT_H
#define _HOUSES_LAYOUT_H

#include "../Utils/ImageLayout.h"
#include "../Utils/Table.h"
#include "House.h"

class HousesLayout : public ImageLayout<float, Vector2I, Vector3F> {
private:
	Table<House> _houses;
	void _renderHouse(House &house);
	House _adjustHouse(const House &house);

public:
	HousesLayout(Image *image);
	Table<House *> inCollisionWith(const House &house);
	void addHouse(const House &house);
	void updateImage(const BoundingBox2D &updateArea);
	void updateImage(unsigned int x0,
			unsigned int y0,
			unsigned int width,
			unsigned int height);

}; // class HousesLayout

#endif // _HOUSES_LAYOUT_H

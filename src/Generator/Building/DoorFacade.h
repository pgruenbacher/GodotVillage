/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef DOOR_FACADE_H
#define DOOR_FACADE_H

#include "Door.h"

class DoorFacade{
public :
	Mesh _mesh;
	Quad _quad;

	DoorFacade(){}

	DoorFacade(Quad quad)
		: _quad(quad)
	{}

	Mesh generate()
	{
		_mesh.empty();

		float doorHeight = 2.2f;
		float doorWidth  = 1.2f;

		
		Table<Quad> temp;
		temp = _quad.rotateToY().cutLeft(0.2f);
		WallE wallBottom = WallE(temp[0]);
		
		temp = temp[1].cutLeft(2.0f);		
		WallE wallTop = WallE(temp[1]);
				
		Door door = Door(temp[0].revertToX());
		
		_mesh += wallBottom.generate();
		_mesh += wallTop.generate();
		_mesh += door.generate();	

		return _mesh;
	}


}; // class DoorFacade

#endif // DOOR_FACADE_H

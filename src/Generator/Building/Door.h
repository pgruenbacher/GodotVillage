/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef DOOR_H
#define DOOR_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "Stair.h"
#include "WallE.h"

class Door {
public:
	Quad _quad;
	Mesh _mesh;

	Door(Quad quad)
			: _quad(quad) {}

	Mesh generate();

}; // class Door

#endif // DOOR_H

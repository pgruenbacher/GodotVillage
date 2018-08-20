/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef DOOR_H
#define DOOR_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "WallE.h"
#include "Stair.h"

class Door{
public :
	Quad _quad;
	Mesh _mesh;

	Door (Quad quad)
		: _quad(quad)
	{}

	Mesh generate();

}; // class Door

#endif // DOOR_H

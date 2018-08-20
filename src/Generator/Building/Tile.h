/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TILE_H
#define TILE_H

#include "../../Engine/Mesh.h"
#include "Quad.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Tile{
public :

	enum Type{ ROUND,
		   FLAT,
		   CHAOS
	};

	Quad _quad;
	Mesh _mesh;
	Type _type;

        Tile(Quad quad, Type type)
		: _quad(quad)
		, _type(type)
	{}

	Mesh generate();

}; // class Tile

#endif // TILE_H

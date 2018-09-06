/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef WALLE_H
#define WALLE_H

#include "../../Engine/Mesh.h"
#include "Quad.h"

class WallE {
public:
	Mesh _mesh;
	Quad _quad;

	WallE() {
	}

	WallE(Quad quad)
			: _quad(quad) {
	}

	Mesh generate() {
		_mesh.empty();

		_mesh += _quad.generate();

		return _mesh;
	}
};

#endif // WALLE_H

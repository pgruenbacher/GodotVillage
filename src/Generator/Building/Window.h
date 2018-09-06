/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "Shutter.h"

class Window {
public:
	enum Type { SQUARE,
		CAVE_SQUARE,
		SMALL,
		KILL,
		KILL2 };

	Mesh _mesh;
	Type _type;
	Quad _quad;

	Window(Quad quad, Type type = SQUARE)
			: _quad(quad), _type(type) {}

	Mesh generate();

}; // class Window

#endif // WINDOW_H

/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TILE_LINE_H
#define TILE_LINE_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "Tile.h"

class TileLine {
public:
	Quad _quad;
	Mesh _mesh;
	Tile::Type _type;

	TileLine(Quad quad, Tile::Type type)
			: _quad(quad), _type(type) {}

	Mesh generate() {
		_mesh.empty();

		Table<Quad> quads = _quad.rotateToY().subdiviseXwidthForced(0.3);

		for (unsigned int i = 0; i < quads.getSize(); ++i) {
			Tile tile(quads[i], _type);
			_mesh += tile.generate();
		}

		return _mesh;
	}

}; // class TileLine

#endif // TILE_LINE_H

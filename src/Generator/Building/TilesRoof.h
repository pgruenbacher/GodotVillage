/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TILES_ROOF_H
#define TILES_ROOF_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "TileLine.h"

class TilesRoof {

public:
	Quad _quad;
	Mesh _mesh;
	Tile::Type _type;

	TilesRoof(Quad quad, Tile::Type type)
			: _quad(quad), _type(type) {}

	Mesh generate() {
		_mesh.empty();

		Table<Quad> tilesLine = _quad.rotateToY().subdiviseXwidthForced(0.2);

		for (unsigned int i = 0; i < tilesLine.getSize(); ++i) {
			TileLine line(tilesLine[i], _type);
			_mesh += line.generate();
		}

		return _mesh;
	}

}; // class TilesRoof

#endif // TILES_ROOF_H

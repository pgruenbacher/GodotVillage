/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef SLOPE_ROOF_H
#define SLOPE_ROOF_H

#include "../../Engine/Mesh.h"
#include "Chimney.h"
#include "Quad.h"
#include "TilesRoof.h"
#include "Triangg.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class SlopeRoof {
public:
	Quad _quad;
	Mesh _mesh;

	SlopeRoof() {}

	SlopeRoof(Quad quad)
			: _quad(quad) {}

	Mesh generate() {
		_mesh.empty();

		float backHeight = 2.0f;
		float frontHeight = 0.1f;
		float frontDepth = 0.2f;
		float frontOffset = (backHeight - frontHeight) * (frontDepth / norm(_quad.p01()));

		Vector3F pp0, pp1, pp2, pp3, pp4, pp5;
		pp0 = _quad.p0() - frontDepth * normalize(_quad.p01()) - frontOffset * _quad.normal();
		pp1 = _quad.p3() - frontDepth * normalize(_quad.p01()) - frontOffset * _quad.normal();
		pp2 = pp0 + _quad.normal() * frontHeight;
		pp3 = pp1 + _quad.normal() * frontHeight;
		pp4 = _quad.p1() + (frontHeight + backHeight) * _quad.normal();
		pp5 = _quad.p2() + (frontHeight + backHeight) * _quad.normal();

		Quad q0(_quad.p0(), pp0, pp1, _quad.p3());
		Quad q1(pp0, pp2, pp3, pp1);
		Quad q2(pp2, pp4, pp5, pp3);
		Quad q3(_quad.p0(), _quad.p1(), pp4, pp2);
		Quad q4(_quad.p3(), pp3, pp5, _quad.p2());
		Quad q5(_quad.p1(), _quad.p2(), pp5, pp4);
		Triangg t0(pp0, _quad.p0(), pp2);
		Triangg t1(pp1, pp3, _quad.p3());

		_mesh += q0.generate();
		_mesh += q1.generate();
		//_mesh += q2.generate();
		_mesh += q3.generate();
		_mesh += q4.generate();
		_mesh += q5.generate();
		_mesh += t0.generate();
		_mesh += t1.generate();

		float random = float(rand() % 1000) / 1000.f;
		Tile::Type type;
		if (random < 0.5) {
			type = Tile::ROUND;
		} else if (random < 0.85) {
			type = Tile::FLAT;
		} else {
			type = Tile::CHAOS;
		}

		TilesRoof tiles(q2, type);
		_mesh += tiles.generate();

		Chimney ch(q2.rotateToY());
		_mesh += ch.generate();

		return _mesh;
	}

}; // class SlopeRoof

#endif // SLOPE_ROOF_H

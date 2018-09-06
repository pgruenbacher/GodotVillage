/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef SQUARE_ROOF_H
#define SQUARE_ROOF_H

#include "../../Engine/Mesh.h"
#include "../../Maths/Calcul.h"
#include "Chimney.h"
#include "Quad.h"
#include "Triangg.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class SquareRoof {
public:
	Quad _quad;
	Mesh _mesh;

	SquareRoof(Quad quad)
			: _quad(quad) {}

	Mesh generate() {
		_mesh.empty();

		Vector3F pp0, pp1;

		float coeff;
		coeff = MIN(norm(_quad.p03()) / 3.0, 2.0);

		pp0 = _quad.p0() + _quad.p01() / 2.f + normalize(_quad.p03()) * coeff;
		pp1 = _quad.p3() + _quad.p32() / 2.f - normalize(_quad.p03()) * coeff;

		pp0.y += 2.f;
		pp1.y += 2.f;

		Quad q1(_quad.p0(), pp0, pp1, _quad.p3());
		Quad q2(pp1, pp0, _quad.p1(), _quad.p2());
		Triangg t1(_quad.p0(), _quad.p1(), pp0);
		Triangg t2(pp1, _quad.p2(), _quad.p3());

		_mesh += q1.generate();
		_mesh += q2.generate();
		_mesh += t1.generate();
		_mesh += t2.generate();

		Chimney ch(q2.rotateToY());
		_mesh += ch.generate();

		return _mesh;
	}

}; // class SquareRoof

#endif // SQUARE_ROOF_H

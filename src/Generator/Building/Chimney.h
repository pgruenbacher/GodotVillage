/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef CHIMNEY_H
#define CHIMNEY_H

#include "../../Engine/Mesh.h"
#include "Quad.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Chimney {
public:
	Quad _quad;
	Mesh _mesh;

	Chimney(Quad quad)
			: _quad(quad) {}

	Mesh generate() {
		_mesh.empty();

		float random = float(rand() % 1000) / 1000.f;

		Vector3F pos = _quad.p0() + 0.2f * _quad.p01() + 0.2f * _quad.p03() + Vector3F(0, -0.1, 0);

		if (random < 0.5) {
			_mesh += Mesh::cube(pos, pos + Vector3F(0.01, 1.0f, 0), 0.4);
			_mesh += Mesh::cube(pos, pos + Vector3F(0.01, 1.2f, 0), 0.2);
		} else {
			Vector3F pos1 = _quad.p0() + 0.22f * _quad.p01() + 0.22f * _quad.p03() + Vector3F(0, -0.1, 0);
			Vector3F pos2 = _quad.p0() + 0.18f * _quad.p01() + 0.22f * _quad.p03() + Vector3F(0, -0.1, 0);
			Vector3F pos3 = _quad.p0() + 0.22f * _quad.p01() + 0.18f * _quad.p03() + Vector3F(0, -0.1, 0);
			Vector3F pos4 = _quad.p0() + 0.18f * _quad.p01() + 0.18f * _quad.p03() + Vector3F(0, -0.1, 0);

			_mesh += Mesh::cube(pos, pos + Vector3F(0.01, 1.0f, 0), 0.4);
			_mesh += Mesh::cube(pos + Vector3F(0.01, 1.2f, 0), pos + Vector3F(0.01, 1.3f, 0), 0.45);

			_mesh += Mesh::cube(pos1, pos1 + Vector3F(0.01, 1.22f, 0), 0.05);
			_mesh += Mesh::cube(pos2, pos2 + Vector3F(0.01, 1.22f, 0), 0.05);
			_mesh += Mesh::cube(pos3, pos3 + Vector3F(0.01, 1.22f, 0), 0.05);
			_mesh += Mesh::cube(pos4, pos4 + Vector3F(0.01, 1.22f, 0), 0.05);
		}

		return _mesh;
	}

}; // class Chimney

#endif // CHIMNEY_H

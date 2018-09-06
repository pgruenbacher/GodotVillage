/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _ROOF_H
#define _ROOF_H

#include "../../Engine/Mesh.h"

class Roof {
public:
	Quad _base;
	float _height;
	Mesh _mesh;

	Roof(Quad base, float height)
			: _base(base), _height(height) {}

	Mesh generate() {
		_mesh.empty();

		_mesh.getVerticesArray().pushLast(_base._points[0]);
		_mesh.getVerticesArray().pushLast(_base._points[1]);
		_mesh.getVerticesArray().pushLast(_base._points[2]);
		_mesh.getVerticesArray().pushLast(_base._points[3]);

		_mesh.getVerticesArray().pushLast(_base._points[0] + Vector3F(0, _height, 0));
		_mesh.getVerticesArray().pushLast(_base._points[1] + Vector3F(0, _height, 0));
		_mesh.getVerticesArray().pushLast(_base._points[2] + Vector3F(0, _height, 0));
		_mesh.getVerticesArray().pushLast(_base._points[3] + Vector3F(0, _height, 0));

		_mesh.getTrianglesArray().pushLast(Vector3I(0, 1, 5));
		_mesh.getTrianglesArray().pushLast(Vector3I(5, 4, 0));

		_mesh.getTrianglesArray().pushLast(Vector3I(1, 2, 6));
		_mesh.getTrianglesArray().pushLast(Vector3I(6, 5, 1));

		_mesh.getTrianglesArray().pushLast(Vector3I(2, 3, 7));
		_mesh.getTrianglesArray().pushLast(Vector3I(7, 6, 2));

		_mesh.getTrianglesArray().pushLast(Vector3I(3, 0, 4));
		_mesh.getTrianglesArray().pushLast(Vector3I(4, 7, 3));

		_mesh.getTrianglesArray().pushLast(Vector3I(5, 6, 7));
		_mesh.getTrianglesArray().pushLast(Vector3I(7, 4, 5));

		return _mesh;
	}

}; // class Roof

#endif // _ROOF_H

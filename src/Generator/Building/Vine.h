/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef VINE_H
#define VINE_H


#include "../../Engine/Mesh.h"
#include "../../Maths/Vector.h"

class Vine{
public :
	Mesh _mesh;
	Vector3F _pos;

	Vine(const Vector3F& pos)
		: _pos(pos)
	{}

	Mesh generate()
	{
		_mesh.empty();

		float height = 0.5;

		_mesh = (Mesh::cube(_pos + Vector3F(0.1, 0.0, 0.0),
				    _pos + Vector3F(0.1, 0.0, 0.1),
				    _pos + Vector3F(0.0, 0.0, 0.1),
				    _pos + Vector3F(0.0, 0.0, 0.0),
				    0.5));

		
		return _mesh;
	}

}; // class Vine

#endif // VINE_H

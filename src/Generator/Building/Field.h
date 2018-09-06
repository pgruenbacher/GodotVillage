/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef FIELD_H
#define FIELD_H

#include "../../Engine/Mesh.h"
#include "../../Maths/Calcul.h"
#include "../../Utils/Table.h"
#include "Quad.h"
#include "Vine.h"

class Field {
public:
	Mesh _mesh;
	Table<Vine *> _vines;
	Table<Vector2F> _foot;

	Field(Table<Vector2F> foot)
			: _foot(foot) {
	}

	Mesh generate();

}; // class Field

#endif // FIELD_H

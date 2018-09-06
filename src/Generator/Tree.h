/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TREE_H
#define TREE_H

#include "../Engine/Mesh.h"
#include "../Engine/Object3D.h"

class Tree : public Object3D {
public:
	Mesh _mesh;
	Mesh _mesh1;

	Tree(Vector3F pos) {
		_node.setPosition(pos);
		_mesh = Mesh::cube(pos, pos + Vector3F(0.01, 3.0, 0.0), 0.3);
		_mesh1 = Mesh::sphere(pos + Vector3F(0.01, 3.0, 0.0), 2.0);
		_mesh.prepareIllumination();
		_mesh1.prepareIllumination();
	}

}; // class Tree

#endif // TREE_H

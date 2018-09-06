/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _OBJECT3D_H
#define _OBJECT3D_H

#include "Node.h"

class Object3D {
protected:
	Node _node;

public:
	Object3D() {}
	Node &getNode() {
		return _node;
	}

}; // class Object3D

#endif // _OBJECT3D_H

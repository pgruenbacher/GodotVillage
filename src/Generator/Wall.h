/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef WALL_H
#define WALL_H

#include "../Utils/LinkedList.h"

class Wall {
public:
	LinkedList<Vector2F> _nodes;
	LinkedList<Vector2F> _doors;
}; // class Wall

#endif // WALL_H

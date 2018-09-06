/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef DE_CASTELJAU_H
#define DE_CASTELJAU_H

#include "../Utils/LinkedList.h"
#include "Vector.h"

class DeCasteljau {
public:
	static Vector2F evaluate(LinkedList<Vector2F> controlPoints, float t);
	static LinkedList<Vector2F> bezierCurve(LinkedList<Vector2F> controlPoints, float step);
}; // class DeCasteljau

#endif // DE_CASTELJAU_H

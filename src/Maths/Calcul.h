/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef CALCUL_H
#define CALCUL_H

#include "../Utils/LinkedList.h"
#include "../Utils/Table.h"
#include "Vector.h"

#define MAX(a, b) ((a) >= (b)) ? (a) : (b)
#define MIN(a, b) ((a) <= (b)) ? (a) : (b)
#define ABS(a) ((a) < 0 ? (-a) : (a))

float distanceToAPolygon(Vector2F point, Table<Vector2F> polygon);
float distanceToACurve(Vector2F point, Table<Vector2F> curve);

float distanceToAPolygon(Vector2F point, LinkedList<Vector2F> polygon);
float distanceToACurve(Vector2F point, LinkedList<Vector2F> curve);

LinkedList<Vector2F> computeJarvis(LinkedList<Vector2F> vertices);

#endif // CALCUL_H

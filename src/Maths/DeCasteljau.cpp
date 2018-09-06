/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "DeCasteljau.h"
#include <QVector>

Vector2F DeCasteljau::evaluate(LinkedList<Vector2F> controlPoints, float t) {
	int degree = controlPoints.getSize() - 1;
	//Vector2F buffer[degree+1];
	QVector<Vector2F> buff;

	LinkedList<Vector2F>::LinkedListIterator it(&controlPoints);
	int i = 0;
	while (it.hasNext()) {
		it.next();
		//buffer[i] = it.value();
		buff.append(it.value());
		++i;
	}

	// solving
	for (unsigned int currentDeg = 0; currentDeg < degree; ++currentDeg) {
		for (unsigned int currentInd = 0; currentInd < degree - currentDeg; ++currentInd) {
			buff[currentInd] = ((1.f - t) * buff[currentInd]) + (t * buff[currentInd + 1]);
			//buffer[currentInd] = ((1.f-t) * buffer[currentInd]) + (t * buffer[currentInd + 1]);
		}
	}
	return buff[0];
	//return buffer[0];
}

LinkedList<Vector2F> DeCasteljau::bezierCurve(LinkedList<Vector2F> controlPoints, float step) {
	LinkedList<Vector2F> result;

	float current = 0.0;
	while (current < 1.0) {
		result.pushLast(evaluate(controlPoints, current));
		current += step;
	}

	return result;
}

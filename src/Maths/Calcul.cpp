/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Calcul.h"

float distanceToAPolygon(Vector2F point, Table<Vector2F> polygon) {
	if (!polygon.getSize())
		return 0;
	// Initialisation
	float distance = norm(point - polygon[0]);
	// Loop on points
	for (unsigned int i = 1; i < polygon.getSize(); ++i) {
		distance = MIN(ABS(distance), norm(point - polygon[i]));
	}
	// Are we inside or outside the polygon
	int inside = 0;
	// Loop on projections
	for (unsigned int i = 0; i < polygon.getSize(); ++i) {
		Vector2F p1 = polygon[i];
		Vector2F p2 = (i == polygon.getSize() - 1) ? polygon[0] : polygon[i + 1];
		float k = dotProduct(point - p1, normalize(p2 - p1));
		Vector2F projection = p1 + normalize(p2 - p1) * k;

		// Inside ?
		float yMin = MIN(p1.y, p2.y);
		float yMax = MAX(p1.y, p2.y);
		if (point.y >= yMin && point.y < yMax) {
			float k_horizontal = (point.y - p1.y) / (normalize(p2 - p1)).y;
			if ((p1 + normalize(p2 - p1) * k_horizontal).x < point.x)
				++inside;
		}

		// The projection is not on the segment
		if (k >= norm(p2 - p1) || k <= 0) {
			continue;
		}

		// The projection is on the polygon
		distance = MIN(ABS(distance), norm(point - projection));
	}
	//return distance;
	return ((inside % 2) == 0) ? distance : -distance;
}

float computePolarAngle(const Vector2F &a, const Vector2F &b, const Vector2F &c) {
	if (a == b)
		return 999.0;

	float x;
	float y;
	if (a == c) {
		x = (b - a).y;
		y = (b - a).x;
	} else {
		Vector2F proj;
		Vector2F nac = normalize(a - c);
		proj.x = dotProduct((b - a), nac);
		Vector2F normal(-nac.y, nac.x);
		proj.y = dotProduct((b - a), normal);
		x = proj.y;
		y = proj.x;
	}

	if (x >= 0 && y > 0)
		return atanf(y / x);
	else if (x > 0 && y < 0)
		return atanf(y / x) + 2.0 * M_PI;
	else if (x < 0)
		return atanf(y / x) + M_PI;
	else if (x == 0 && y > 0)
		return M_PI / 2.0;
	else
		return 3.0 * M_PI / 2.0;
}

float distanceToACurve(Vector2F point, Table<Vector2F> polygon) {
	if (!polygon.getSize())
		return 0;
	// Initialisation
	float distance = norm(point - polygon[0]);
	// Loop on points
	for (unsigned int i = 1; i < polygon.getSize(); ++i) {
		distance = MIN(distance, norm(point - polygon[i]));
	}
	// Loop on projections
	for (unsigned int i = 0; i < polygon.getSize() - 1; ++i) {
		Vector2F p1 = polygon[i];
		Vector2F p2 = polygon[i + 1];
		float k = dotProduct(point - p1, normalize(p2 - p1));
		Vector2F projection = p1 + normalize(p2 - p1) * k;

		// The projection is not on the segment
		if (k >= norm(p2 - p1) || k <= 0) {
			continue;
		}

		// The projection is on the polygon
		distance = MIN(distance, norm(point - projection));
	}
	//return distance;
	return distance;
}

float distanceToAPolygon(Vector2F point, LinkedList<Vector2F> polygon) {
	if (!polygon.getSize())
		return 0;
	// Initialisation
	float distance = norm(point - polygon.getFirst());
	// Loop on points
	LinkedList<Vector2F>::LinkedListIterator it(&polygon);
	while (it.hasNext()) {
		it.next();
		distance = MIN(ABS(distance), norm(point - it.value()));
	}
	// Are we inside or outside the polygon
	int inside = 0;
	// Loop on projections
	LinkedList<Vector2F>::LinkedListIterator it2(&polygon);
	Vector2F first, prev, current;
	if (it2.hasNext()) {
		it2.next();
		first = prev = it2.value();
	}
	while (it2.hasNext()) {
		it2.next();
		current = it2.value();
		Vector2F &p1 = prev;
		Vector2F &p2 = current;
		float k = dotProduct(point - p1, normalize(p2 - p1));
		Vector2F projection = p1 + normalize(p2 - p1) * k;

		// Inside ?
		float yMin = MIN(p1.y, p2.y);
		float yMax = MAX(p1.y, p2.y);
		if (point.y >= yMin && point.y < yMax) {
			float k_horizontal = (point.y - p1.y) / (normalize(p2 - p1)).y;
			if ((p1 + normalize(p2 - p1) * k_horizontal).x < point.x)
				++inside;
		}

		// The projection is not on the segment
		if (k >= norm(p2 - p1) || k <= 0) {
			prev = current;
			continue;
		}

		// The projection is on the polygon
		distance = MIN(ABS(distance), norm(point - projection));
		prev = current;
	}
	//return distance;
	return ((inside % 2) == 0) ? distance : -distance;
}

float distanceToACurve(Vector2F point, LinkedList<Vector2F> polygon) {
	if (!polygon.getSize())
		return 0;
	// Initialisation
	float distance = norm(point - polygon.getFirst());
	// Loop on points
	LinkedList<Vector2F>::LinkedListIterator it(&polygon);
	while (it.hasNext()) {
		it.next();
		distance = MIN(ABS(distance), norm(point - it.value()));
	}
	// Loop on projections
	LinkedList<Vector2F>::LinkedListIterator it2(&polygon);
	Vector2F current, prev;
	if (it2.hasNext()) {
		it2.next();
		prev = it2.value();
	}
	while (it2.hasNext()) {
		it2.next();
		current = it2.value();
		Vector2F &p1 = prev;
		Vector2F &p2 = current;
		float k = dotProduct(point - p1, normalize(p2 - p1));
		Vector2F projection = p1 + normalize(p2 - p1) * k;

		// The projection is not on the segment
		if (k >= norm(p2 - p1) || k <= 0) {
			prev = current;
			continue;
		}

		// The projection is on the polygon
		distance = MIN(ABS(distance), norm(point - projection));
		prev = current;
	}
	//return distance;
	return distance;
}

LinkedList<Vector2F> computeJarvis(LinkedList<Vector2F> vertices) {
	LinkedList<Vector2F> result;
	if (vertices.getSize() == 0)
		return result;

	// Find minus X point value
	Vector2F start = vertices.getFirst();
	LinkedList<Vector2F>::LinkedListIterator it(&vertices);
	while (it.hasNext()) {
		it.next();
		if (it.value().x < start.x) {
			start = it.value();
		}
	}
	//std::cout << "Start : " << start << std::endl;

	// Jarvis travel
	result.pushLast(start);
	LinkedList<Vector2F> vertexList = vertices;
	float angle;
	float minAngle;
	Vector2F current = start;
	Vector2F next;
	Vector2F last = start;
	do {
		minAngle = 4.0 * M_PI;
		LinkedList<Vector2F>::LinkedListIterator it2(&vertexList);
		while (it2.hasNext()) {
			it2.next();
			angle = computePolarAngle(current, it2.value(), last);
			//std::cout << "it2.value() " << it2.value() << " angle " << angle << std::endl;
			if (angle < minAngle) {
				next = it2.value();
				minAngle = angle;
				//std::cout << "Next is changing: " << next << std::endl;
			}
		}
		result.pushLast(next);
		vertexList.remove(next);
		//std::cout << "Next : " << next << std::endl;
		//sleep(1);
		last = current;
		current = next;
	} while (!(current == start));
	return result;
}

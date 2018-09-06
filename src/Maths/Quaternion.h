/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "Vector.h"
#include <math.h>
#include <iostream>

class Quaternion {
public:
	Vector3F vector;

	/** Angle in radian. */
	float angle;

	/** Angle € [-pi, pi] */
	void _correctAngle();

public:
	Quaternion(float angleInRad = 1,
			Vector3F vector = Vector3F::ZERO);

	Quaternion(float angleInRad,
			float x,
			float y,
			float z);

	Quaternion(const Quaternion &quat);

	~Quaternion();

	static Quaternion createFromAxisAndAngle(float angleInRad = 0,
			Vector3F axis = Vector3F::XAXIS);

	Quaternion operator*=(const Quaternion &quat);
	friend Quaternion operator*(const Quaternion &quat1, const Quaternion &quat2);
	friend bool operator==(const Quaternion &quat1, const Quaternion &quat2);
	friend std::ostream &operator<<(std::ostream &o, const Quaternion &quat);
	friend float norm(const Quaternion &quat);
	friend Quaternion normalize(const Quaternion &quat);

	static const Quaternion ZERO;

}; // class Quaternion

#endif // _QUATERNION

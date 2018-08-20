/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef RAY_H
#define RAY_H

#include "../Maths/Vector.h"

#ifndef INFINITY
#define INFINITY 9999999
#endif

struct Ray
{

    Vector3F _position;
    Vector3F _direction;

    float    distFromASurface(Vector3F position, Vector3F normal)
    {
        float num   = dotProduct(position - _position, normal);
        float denum = dotProduct(_direction, normal);

        if (denum == 0.f)
            return INFINITY;
        else
            return (num/denum);
    }

    Vector3F intersectionWithASurface(Vector3F position, Vector3F normal)
    {

        return _position + distFromASurface(position,normal) * _direction;
    }


}; // class Ray

#endif // RAY_H

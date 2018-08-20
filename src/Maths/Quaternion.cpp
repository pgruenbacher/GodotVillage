/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Quaternion.h"

const Quaternion Quaternion::ZERO = Quaternion(1, 0, 0, 0);

Quaternion::Quaternion(float angleInRad,
                       Vector3F vvector)
    : vector(vvector)
    , angle(angleInRad)
{
    _correctAngle();
}

Quaternion::Quaternion(float aangleInRad,
                       float x,
                       float y,
                       float z)
    : vector(Vector3F(x,y,z))
    , angle(aangleInRad)
{
    _correctAngle();
}

void Quaternion::_correctAngle()
{
    while (angle > M_PI)
    {
        angle -= 2.0 * M_PI;
    }
}

Quaternion::Quaternion(const Quaternion& quat)
    : vector(quat.vector)
    , angle(quat.angle)
{
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator*=(const Quaternion& quat)
{
    (*this) = (*this) * quat;
    return (*this);
}

Quaternion operator*(const Quaternion& quat1, const Quaternion& quat2)
{
    Quaternion result;

    result.angle  = quat1.angle * quat2.angle
                    - dotProduct(quat1.vector, quat2.vector);

    result.vector = quat1.angle * quat2.vector
                    + quat2.angle * quat1.vector
                    + crossProduct(quat1.vector, quat2.vector);

    result._correctAngle();

    return result;
}

bool operator==(const Quaternion& quat1, const Quaternion& quat2)
{
    return (quat1.angle == quat2.angle) && (quat1.vector == quat2.vector);
}

std::ostream& operator<<(std::ostream& o, const Quaternion& quat)
{
    o << quat.angle << " " << quat.vector;
    return o;
}

float norm(const Quaternion& quat)
{
    return sqrt(float( quat.angle * quat.angle
                       + quat.vector.x * quat.vector.x
                       + quat.vector.y * quat.vector.y
                       + quat.vector.z * quat.vector.z ));
}

Quaternion normalize(const Quaternion& quat)
{
    if (quat == Quaternion::ZERO)
        return quat;
    float normm = norm(quat);
    return Quaternion(quat.angle    / normm,
                      quat.vector.x / normm,
                      quat.vector.y / normm,
                      quat.vector.z / normm);
}


Quaternion Quaternion::createFromAxisAndAngle(float angleInRad,
        Vector3F axis)
{
    float s = sinf(angleInRad / 2.0);
    float c = cosf(angleInRad / 2.0);

    Quaternion quat;
    quat.vector.x = axis.x * s;
    quat.vector.y = axis.y * s;
    quat.vector.z = axis.z * s;
    quat.angle    = c;

    return normalize(quat);
}


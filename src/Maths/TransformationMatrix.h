/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _TransformationMatrix_H
#define _TransformationMatrix_H

#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"


template <class T>
class TransformationMatrix : public Matrix<T,4,4>
{

public :

    /** Create a new Transformation Matrix.
        It is an Identity transformation. */
    TransformationMatrix<T>();
    TransformationMatrix<T>(const Matrix<T,4u,4u>& matrix);

    /** Create a matrix by seting its default values. */
    TransformationMatrix<T>(const T& m11, const T& m12, const T& m13, const T& m14,
                            const T& m21, const T& m22, const T& m23, const T& m24,
                            const T& m31, const T& m32, const T& m33, const T& m34,
                            const T& m41, const T& m42, const T& m43, const T& m44);

    /** Create a projection matrix with the given arguments. */
    static TransformationMatrix<T> projection(T near,
            T far,
            T fov,
            T aspect);

    /** Create a orthogonal matrix with the given arguments. */
    static TransformationMatrix<T> ortho(T near,
                                         T far,
                                         T fov,
                                         T aspect);

    static Matrix<T, 3, 3> normal(const TransformationMatrix<T>& modelViewMatrix);

    /** Return a translation matrix. */
    static TransformationMatrix<T> translation(const Vector3<T>& translation);

    /** Return a rotation around the axis X. */
    static TransformationMatrix<T> rotationAroundX(const T& angle);

    /** Return a rotation around the axis Y. */
    static TransformationMatrix<T> rotationAroundY(const T& angle);

    /** Return a rotation around the axis Z. */
    static TransformationMatrix<T> rotationAroundZ(const T& angle);

    /** Return a rotation using a quaternion. */
    static TransformationMatrix<T> rotation(const Quaternion& quat);

    /** Return a scaling matrix. */
    static TransformationMatrix<T> scaling(const Vector3<T>& scale);

    TransformationMatrix<T>& operator=(const Matrix<T,4u,4u>& matrix);

    Vector4<T> operator*(const Vector4<T>& vector);
    Vector3<T> operator*(const Vector3<T>& vector);

    /* return the rotation matrix associated to the transform */
    static TransformationMatrix<T> extractRotation(const TransformationMatrix<T> &m);
    static TransformationMatrix<T> extractTranslation(const TransformationMatrix<T> &m);

}; // class TransformationMatrix

typedef TransformationMatrix<float> TransformMatrix;
typedef Matrix<float,3,3>           NormalMatrix;


#include "TransformationMatrix.impl"



#endif // _TransformationMatrix_H

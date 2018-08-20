/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef Vector_H
#define Vector_H

#include <ostream>

// Friends pre-declaration
template <class T>
class Vector2;
template <class T>
bool operator<(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
bool operator<=(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
std::ostream& operator<<(std::ostream& o, const Vector2<T>& v );
template <class T>
Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
Vector2<T> operator*(const Vector2<T>& v1, const T& a);
template <class T>
Vector2<T> operator*(const T& a, const Vector2<T>& v1);
template <class T>
Vector2<T> operator/(const Vector2<T>& v1, const T& a);
template <class T>
bool operator==(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
float norm(const Vector2<T>& v);
template <class T>
Vector2<T> normalize(const Vector2<T>& v);
template <class T>
float dotProduct(const Vector2<T>& v1, const Vector2<T>& v2);
template <class T>
float orientedAngle(const Vector2<T>& v1, const Vector2<T>& v2);

template <class T>
class Vector3;
template <class T>
float norm(const Vector3<T>& v);
template <class T>
Vector3<T> normalize(const Vector3<T>& v);
template <class T>
Vector3<T> crossProduct(const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
float dotProduct(const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
Vector3<T> operator+ (const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
Vector3<T> operator+ (const T& a, const Vector3<T>& v1);
template <class T>
Vector3<T> operator+ (const Vector3<T>& v1, const T& a);
template <class T>
Vector3<T> operator+ (const Vector3<T>& v1);
template <class T>
Vector3<T> operator- (const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
Vector3<T> operator- (const Vector3<T>& v1, const T& a);
template <class T>
Vector3<T> operator- (const T& a, const Vector3<T>& v1);
template <class T>
Vector3<T> operator- (const Vector3<T>& v1);
template <class T>
Vector3<T> operator*(const Vector3<T>& v1, const T& coeff);
template <class T>
Vector3<T> operator*(const T& coeff, const Vector3<T>& v1);
template <class T>
Vector3<T> operator/(const Vector3<T>& v1, const T& coeff);
template <class T>
Vector3<T> operator/(const T& coeff, const Vector3<T>& v1);
template <class T>
bool operator==(const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
bool operator!=(const Vector3<T>& v1, const Vector3<T>& v2);
template <class T>
Vector3<T> rotationAroundAVector(const Vector3<T>& v1, const Vector3<T>& v2, float angle);
template <class T>
std::ostream& operator<<(std::ostream& o, const Vector3<T>& v1 );




template <class T>
class Vector4;
template <class T>
float norm(Vector4<T>& v);
template <class T>
Vector4<T> crossProduct(const Vector4<T>& v1, const Vector4<T>& v2);
template <class T>
float dotProduct(const Vector4<T>& v1, const Vector4<T>& v2);
template <class T>
Vector4<T> operator+ (const Vector4<T>& v1, const Vector4<T>& v2);
template <class T>
Vector4<T> operator+ (const Vector4<T>& v1, const T& a);
template <class T>
Vector4<T> operator+ (const T& a, const Vector4<T>& v1);
template <class T>
Vector4<T> operator+ (const Vector4<T>& v1);
template <class T>
Vector4<T> operator- (const Vector4<T>& v1, const Vector4<T>& v2);
template <class T>
Vector4<T> operator- (const Vector4<T>& v1, const T& a);
template <class T>
Vector4<T> operator- (const T& a, const Vector4<T>& v1);
template <class T>
Vector4<T> operator- (const Vector4<T>& v1);
template <class T>
Vector4<T> operator*(const Vector4<T>& v1, const T& coeff);
template <class T>
Vector4<T> operator*(const T& coeff, const Vector4<T>& v1);
template <class T>
Vector4<T> operator/(const Vector4<T>& v1, const T& coeff);
template <class T>
Vector4<T> operator/(const T& coeff, const Vector4<T>& v1);
template <class T>
std::ostream& operator<<(std::ostream& o, const Vector4<T>& v1 );



template <class T>
class Vector2
{
public:
    // Attributes
    T x;
    T y;

    // Constructors
    Vector2();
    Vector2(T xx, T yy);

    template <class U>
    Vector2(U xx, U yy);

    Vector2(const Vector2<T>& vec);

    template <class U>
    Vector2(const Vector2<U>& vec);

    // Operators
    T& operator[](unsigned int index);
    Vector2<T>& operator= (const Vector2<T>& vec);

    friend Vector2<T> normalize <>(const Vector2<T>& v);
};

template <class T>
class Vector3 : public Vector2<T>
{
public:
    // Attributes
    T z;

    // Constructors
    Vector3();

    Vector3(T xx, T yy, T zz);

    Vector3(const Vector3<T>& vec);
    Vector3(const Vector2<T>& vec);

    template <class U>
    Vector3(U xx, U yy, U zz);

    template <class U>
    Vector3(const Vector2<U>& vec);

    template <class U>
    Vector3(const Vector3<U>& vec);

    // Display
    void display() const;

    friend float norm <>(const Vector3<T>& v);
    friend Vector3<T> normalize <>(const Vector3<T>& v);
    friend Vector3<T> crossProduct <>(const Vector3<T>& v1, const Vector3<T>& v2);
    friend float dotProduct <>(const Vector3<T>& v1, const Vector3<T>& v2);

    T& operator[](unsigned int index);
    T operator[](unsigned int index) const;

    // Operators
    Vector3<T>& operator= (const Vector3<T>& vec);
    Vector3<T>& operator+= (const Vector3<T>& v2);
    Vector3<T>& operator+= (const T& a);
    Vector3<T>& operator-= (const Vector3<T>& v2);
    Vector3<T>& operator-= (const T& a);
    Vector3<T>& operator*= (const T& a);
    Vector3<T>& operator/= (const T& a);

    friend Vector3<T> operator+  <>(const Vector3<T>& v1, const Vector3<T>& v2);
    friend Vector3<T> operator+  <>(const Vector3<T>& v1);
    friend Vector3<T> operator+  <>(const Vector3<T>& v1, const T& a);
    friend Vector3<T> operator+  <>(const T& a, const Vector3<T>& v1);
    friend Vector3<T> operator-  <>(const Vector3<T>& v1, const Vector3<T>& v2);
    friend Vector3<T> operator-  <>(const Vector3<T>& v1, const T& a);
    friend Vector3<T> operator-  <>(const T& a, const Vector3<T>& v1);
    friend Vector3<T> operator-  <>(const Vector3<T>& v1);

    friend Vector3<T> operator* <>(const Vector3<T>& v1, const T& coeff);
    friend Vector3<T> operator* <>(const T& coeff, const Vector3<T>& v1);

    friend Vector3<T> operator/ <>(const Vector3<T>& v1, const T& coeff);
    friend Vector3<T> operator/ <>(const T& coeff, const Vector3<T>& v1);

    friend std::ostream& operator<< <>(std::ostream& o, const Vector3<T>& v1 );

    friend bool       operator== <>(const Vector3<T>& v1, const Vector3<T>& v2);
    friend bool       operator!= <>(const Vector3<T>& v1, const Vector3<T>& v2);
    // Rotations
    friend Vector3<T> rotationAroundAVector <>(const Vector3<T>& U, const Vector3<T>& N, float angle);
    void rotationAroundAxisX(float angleRad);
    void rotationAroundAxisY(float angleRad);
    void rotationAroundAxisZ(float angleRad);

    static const Vector3<T> ZERO;
    static const Vector3<T> XAXIS;
    static const Vector3<T> YAXIS;
    static const Vector3<T> ZAXIS;
    static const Vector3<T> UNIT;

};

template <class T>
class Vector4 : public Vector3<T>
{
public:
    // Attributes
    T w;

    // Constructors
    Vector4<T>();
    Vector4<T>(T xx, T yy, T zz, T ww);
    Vector4<T>(const Vector4<T>& vec);

    template <class U>
    Vector4<T>(const Vector4<U>& vec);

    template <class U>
    Vector4<T>(U xx, U yy, U zz, U ww);

    // Display
    void display() const;

    // Special Operations
    void normalize();


    friend float norm <>(Vector4<T>& v);
    friend float dotProduct <>(const Vector4<T>& v1, const Vector4<T>& v2);

    // Operators
    Vector4<T>& operator= (const Vector4<T>& vec);
    Vector4<T>& operator+= (const Vector4<T>& v2);
    Vector4<T>& operator+= (const T& a);
    Vector4<T>& operator-= (const Vector4<T>& v2);
    Vector4<T>& operator-= (const T& a);
    Vector4<T>& operator*= (const T& a);
    Vector4<T>& operator/= (const T& a);

    T& operator[](unsigned int index);
    T operator[](unsigned int index) const;

    friend Vector4<T> operator+  <>(const Vector4<T>& v1, const Vector4<T>& v2);
    friend Vector4<T> operator+  <>(const Vector4<T>& v1);
    friend Vector4<T> operator+  <>(const Vector4<T>& v1, const T& a);
    friend Vector4<T> operator-  <>(const Vector4<T>& v1, const Vector4<T>& v2);
    friend Vector4<T> operator-  <>(const Vector4<T>& v1, const T& a);
    friend Vector4<T> operator-  <>(const Vector4<T>& v1);


    friend Vector4<T> operator* <>(const Vector4<T>& v1, const T& coeff);
    friend Vector4<T> operator* <>(const T& coeff, const Vector4<T>& v1);

    friend Vector4<T> operator/ <>(const Vector4<T>& v1, const T& coeff);
    friend Vector4<T> operator/ <>(const T& coeff, const Vector4<T>& v1);

    friend std::ostream& operator<< <>(std::ostream& o, const Vector3<T>& v1 );

    static const Vector4<T> ZERO;
    static const Vector4<T> UNIT;
};

// -------------------
// TYPEDEFS
// -------------------
/* A vector of 2 ints */
typedef Vector2<int> Vector2I;
/* A vector of 2 unsigned ints */
typedef Vector2<unsigned int> Vector2UI;
/* A vector of 2 floats */
typedef Vector2<float> Vector2F;
/* A vector of 3 floats */
typedef Vector3<float> Vector3F;
/* A vector of 3 integers */
typedef Vector3<int> Vector3I;
/* A vector of 3 unsigned integers */
typedef Vector3<unsigned int> Vector3UI;
/* A vector of 3 unsigned char */
typedef Vector3<unsigned char> Vector3UC;
/* A vector of 4 integers */
typedef Vector4<int> Vector4I;
/* A vector of 4 floats */
typedef Vector4<float> Vector4F;
/* A vector of 4 unsigned char */
typedef Vector4<unsigned char> Vector4UC;







// This is an include of the implementation
#include "Vector.impl"
#endif

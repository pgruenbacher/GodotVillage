/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _Matrix_H
#define _Matrix_H

#define COORD(i, j, nbcol) (((i) * (nbcol)) + (j))

#include <iostream>

/** Output. */
template <class T, unsigned int _nbLines, unsigned int _nbColumns>
class Matrix;

template <class T, unsigned int _nbLines, unsigned int _nbColumns>
std::ostream &operator<<(std::ostream &os, const Matrix<T, _nbLines, _nbColumns> &m);

template <class T, unsigned int _nbLines, unsigned int _nbColumns>
class Matrix {

protected:
	T _array[_nbLines * _nbColumns];

public:
	/** Create a new matrix Matrix.
        It is an null Matrix. */
	Matrix<T, _nbLines, _nbColumns>();

	/** Copy. (deep)*/
	Matrix<T, _nbLines, _nbColumns>(const Matrix<T, _nbLines, _nbColumns> &matrix);

	/** Operator= */
	Matrix<T, _nbLines, _nbColumns> &operator=(const Matrix<T, _nbLines, _nbColumns> &matrix);

	/** Return the number of lines. */
	unsigned int getNbLines() const;

	/** Return the number of Columns. */
	unsigned int getNbColumns() const;

	/** Return the array. */
	const T *getArray() const;

	/** Return the identity matrix. */
	static Matrix<T, _nbLines, _nbColumns> identity();

	/** Transpose the matrix. */
	static Matrix<T, _nbColumns, _nbLines> transpose(const Matrix<T, _nbLines, _nbColumns> &matrix);

	/** Invert the matrix. */
	static Matrix<T, _nbLines, _nbColumns> invert(const Matrix<T, _nbLines, _nbColumns> &matrix);
	Matrix<T, _nbLines, _nbColumns> invertOfUpperTriangularMatrix(const Matrix<T, _nbLines, _nbColumns> &matrix);

	/** Lu Factorization. */
	static void LUfacto(const Matrix<T, _nbLines, _nbColumns> &matrix,
			Matrix<T, _nbLines, _nbColumns> *L,
			Matrix<T, _nbLines, _nbColumns> *U);

	static Matrix<T, _nbLines, _nbColumns> ForwardLU(Matrix<T, _nbLines, _nbColumns> &L,
			Matrix<T, _nbLines, _nbColumns> &b);

	static Matrix<T, _nbLines, _nbColumns> BackwardLU(Matrix<T, _nbLines, _nbColumns> &U,
			Matrix<T, _nbLines, _nbColumns> &Y);

	/** **/
	bool operator==(const Matrix<T, _nbLines, _nbColumns> &m);

	T &operator[](unsigned int index);
	T operator[](unsigned int index) const;

	T &get(unsigned int line, unsigned int column);
	T get(unsigned int line, unsigned int column) const;

	/** Friends */
	friend std::ostream &operator<<<>(std::ostream &os, const Matrix<T, _nbLines, _nbColumns> &m);

	template <class TT, unsigned int nbLines, unsigned int nbColumn2, unsigned nbColumns>
	friend Matrix<TT, nbLines, nbColumns> operator*(const Matrix<TT, nbLines, nbColumn2> &m1,
			const Matrix<TT, nbColumn2, nbColumns> &m2);

	template <class TT, unsigned int nbLines, unsigned nbColumns>
	friend Matrix<TT, nbLines, nbColumns> operator+(const Matrix<TT, nbLines, nbColumns> &m1,
			const Matrix<TT, nbLines, nbColumns> &m2);

	template <class TT, unsigned int nbLines, unsigned nbColumns>
	friend Matrix<TT, nbLines, nbColumns> operator-(const Matrix<TT, nbLines, nbColumns> &m1,
			const Matrix<TT, nbLines, nbColumns> &m2);

	template <class TT, unsigned int nbLines, unsigned nbColumns>
	friend Matrix<TT, nbLines, nbColumns> operator*(const TT &a,
			const Matrix<TT, nbLines, nbColumns> &m);
}; // class Matrix

#include "Matrix.impl"

#endif // _Matrix_H

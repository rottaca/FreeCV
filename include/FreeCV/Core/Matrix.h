/*
 * Matrix.h
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "string.h"
#include "sstream"
#include <typeinfo>
#include "assert.h"

#include "FreeCV/Core/Logger.h"
#include "FreeCV/Core/Vector.h"
#include "FreeCV/Core/Macros.h"

namespace fcv {


template<typename T, size_t COMPILE_ROWS, size_t COMPILE_COLS >
class Matrix {

public:
	Matrix(size_t rows, size_t cols, T* data = NULL) {
		m = NULL;
		init(rows, cols, data);
	}
	Matrix(const Matrix<T, COMPILE_ROWS, COMPILE_COLS>& m2) {
		m = NULL;
		init(m2.getRows(), m2.getCols(), m2.getPtr());
	}

	~Matrix() {
		if (m != NULL)
			delete[] m;
	}
	void init(size_t rows, size_t cols, void* data = NULL) {
		if (m != NULL)
			delete[] m;

		if(COMPILE_ROWS != FCV_DYNAMIC_SIZE)
			rows = COMPILE_ROWS;
		if(COMPILE_COLS != FCV_DYNAMIC_SIZE)
			cols = COMPILE_COLS;

		this->cols = cols;
		this->rows = rows;
		m = new T[rows * cols];

		if (data != NULL)
			memcpy(m, data, cols * rows * sizeof(T));
		else
			memset(m, 0, cols * rows * sizeof(T));
	}

	inline T* getPtr() const {
		return m;
	}
	inline T* getPtr(int y) const {
		assert(y < rows && y>=0);
		return m + y * rows;
	}
	inline T* getPtr(int y, int x) const {
		assert(y < rows && x < cols && y >= 0 && x >= 0);
		return m + y * rows + x;
	}
	T& at(int y, int x) {
		assert(y < rows && x < cols && y >= 0 && x >= 0);
		return *getPtr(y, x);
	}

	size_t getCols() const {
		return cols;
	}
	size_t getRows() const {
		return rows;
	}

	std::string toString() const {
		std::stringstream s;
		s << "Mat (Type ";
		if (typeid(T) == typeid(double))
			s << "double";
		else if (typeid(T) == typeid(float))
			s << "float";
		else if (typeid(T) == typeid(int))
			s << "int";
		else if (typeid(T) == typeid(unsigned char))
			s << "byte";
		else
			s << "unknown";

		s << ", Size " << rows << " Rows x " << cols << " Cols): \n";

		T* mPtr = m;
		for (int r = 0; r < rows - 1; r++) {
			for (int c = 0; c < cols - 1; c++) {
				s << *mPtr++ << ", ";
			}
			s << *mPtr++ << "\n";
		}

		for (int c = 0; c < cols - 1; c++) {
			s << *mPtr++ << ", ";
		}
		s << *mPtr++;

		return s.str();
	}
	////////////////////////////////////////
	// Operator overloading
	////////////////////////////////////////
	Matrix<T,COMPILE_ROWS, COMPILE_COLS> operator=(const Matrix<T, COMPILE_ROWS, COMPILE_COLS>& m2) {
		init(m2.getRows(), m2.getCols(), m2.getPtr());
		return *this;
	}

	Vector<T,COMPILE_ROWS> operator*(Vector<T, COMPILE_COLS> v2) {
		assert(v2.getSize() == cols);
		Vector<T,COMPILE_ROWS> nv(rows);

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				nv[r] += v2[c] * at(c, r);
			}
		}
		return nv;
	}
	// TODO
//	Matrix<T, COLS, ROWS> operator*(Matrix<T, ROWS, COLS> v2) {
//		Matrix<T, COLS, ROWS> nv(*this);
//		for (int i = 0; i < S; i++) {
//			nv[i] *= s;
//		}
//		return nv;
//	}

private:
	size_t cols, rows;
	// Contains Matrix Data
	T* m;
};

typedef Matrix<unsigned char,FCV_DYNAMIC_SIZE,FCV_DYNAMIC_SIZE> Matrixb;
typedef Matrix<float,FCV_DYNAMIC_SIZE,FCV_DYNAMIC_SIZE> Matrixf;
typedef Matrix<int,FCV_DYNAMIC_SIZE,FCV_DYNAMIC_SIZE> Matrixi;

typedef Matrix<unsigned char,2,2> Matrix2x2b;
typedef Matrix<float,2,2> Matrix2x2f;
typedef Matrix<int,2,2> Matrix2x2i;

typedef Matrix<unsigned char,3,3> Matrix3x3b;
typedef Matrix<float,3,3> Matrix3x3f;
typedef Matrix<int,3,3> Matrix3x3i;

typedef Matrix<unsigned char,4,4> Matrix4x4b;
typedef Matrix<float,4,4> Matrix4x4f;
typedef Matrix<int,4,4> Matrix4x4i;

} /* namespace fcv */

#endif /* MATRIX_H_ */

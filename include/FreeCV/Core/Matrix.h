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
	/**
	 * Creates a matrix with the specified size and copy the optional data into it.
	 * Attention: matrix size is ignored if matrix is a fixed size type.
	 * Attention: Data array should have the correct size (rows*cols*sizeof(T))
	 */
	Matrix(size_t rows, size_t cols, T* data = NULL) {
		initEmpty();
		init(rows, cols, data);
	}

	Matrix() {
		initEmpty();
		init(0, 0);
	}
	/**
	 * Copy Constructor
	 */
	Matrix(const Matrix<T, COMPILE_ROWS, COMPILE_COLS>& m2) {
		initEmpty();
		init(m2.getRows(), m2.getCols(), m2.getPtr());
	}

	~Matrix() {
		if (m != NULL)
			delete[] m;
	}

	/**
	 * Initializes the Matrix with a given size and optional data.
	 * Size changes are ignored if matrix has a fixed size.
	 */
	void init(size_t rows, size_t cols, void* data = NULL) {
		if (m != NULL){
			delete[] m;
			m = NULL;
		}

		if(COMPILE_ROWS != FCV_DYNAMIC_SIZE)
			rows = COMPILE_ROWS;
		if(COMPILE_COLS != FCV_DYNAMIC_SIZE)
			cols = COMPILE_COLS;

		if(cols == 0 || rows == 0)
			return;

		this->cols = cols;
		this->rows = rows;

		m = new T[rows * cols];

		if (data != NULL)
			memcpy(m, data, cols * rows * sizeof(T));
		else
			memset(m, 0, cols * rows * sizeof(T));
	}

	/**
	 * Returns a pointer to the first entry.
	 */
	inline T* getPtr() const {
		return m;
	}
	/**
	 * Returns a pointer to the first entry of the specified row.
	 */
	inline T* getPtr(int y) const {
		assert(y < rows && y>=0);
		return &m[y * cols];
	}
	/**
	 * Returns a pointer to the specified matrix entry.
	 */
	inline T* getPtr(int y, int x) const {
		assert(y < rows && x < cols && y >= 0 && x >= 0);
		return &m[y * cols + x];
	}
	/**
	 * Returns a reference to the specified matrix entry.
	 * Attention no range check!
	 */
	T& at(int y, int x) {
		assert(y < rows && x < cols && y >= 0 && x >= 0);
		return m[y * cols + x];
	}

	/**
	 * Returns column count
	 */
	size_t getCols() const {
		return cols;
	}
	/**
	 * Returns row count
	 */
	size_t getRows() const {
		return rows;
	}


	/**
	 * Formats the matrix as string.
	 */
	std::string toString() const {
		std::stringstream s;
		s << "Matrix (Type ";
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

		if(m == 0)
		{
			s << "NOT INITIALIZED";
		}
		else{
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
		}
		return s.str();
	}

	/**
	 * Calculates the determinant of the matrix.
	 * Currently only implemented for 2x2 and 3x3 Matrices.
	 */
	T det() {
		assert(rows == cols);
		if (rows == 2) {
			return at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
		} else if (rows == 3) {
			return at(0, 0) *(at(1,1)*at(2,2) - at(1,2)*at(2,1))
				 - at(0, 1) *(at(1,0)*at(2,2) - at(1,2)*at(2,0))
				 + at(0, 2) *(at(1,0)*at(2,1) - at(1,1)*at(2,0));
		} else {
			// TODO not implemented!
			return 0;
		}

	}

	Matrix<T,COMPILE_ROWS, COMPILE_COLS> inverse(){
		Matrix<T,COMPILE_ROWS, COMPILE_COLS> inv;
		LOG_ERROR("Inverse Matrix not implemented yet!");
		return inv;
		assert(rows == cols);
		if(rows == 2){

		}else if(rows == 3){

		}else{
			LOG_FORMAT_ERROR("Inverse Matrix not implemented for %dx%d Matrix",rows,rows);
		}

		return inv;
	}

	void setIdentity(){
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				at(y,x) = (int)(x == y);
			}
		}
	}
	Matrix<T,COMPILE_ROWS, COMPILE_COLS> transpose(){
		assert(cols == rows);
		Matrix<T,COMPILE_ROWS, COMPILE_COLS> mat;
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x <= y; x++) {
				mat.at(y,x) = at(x,y);
				mat.at(x,y) = at(y,x);
			}
		}
		return mat;
	}

	void setTransform(Matrix<T,3,3>R, Vector<T,3> TVec){

		setIdentity();
		for(int y = 0; y < 3; y++){
			for (int x = 0; x < 3; x++) {
				at(y,x) = R.at(y,x);
			}
		}

		at(0,3) = TVec[0];
		at(1,3) = TVec[1];
		at(2,3) = TVec[2];
	}

	bool isEmpty(){
		for (int i = 0; i < rows*cols; i++) {
			if(m[i] != 0)
				return false;
		}
		return true;
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
				nv[r] += v2[c] * at(r,c);
			}
		}
		return nv;
	}

	// TODO
	Matrix<T, COMPILE_COLS, COMPILE_ROWS> operator*(Matrix<T, COMPILE_COLS, COMPILE_ROWS> m2) {
		Matrix<T, COMPILE_ROWS, COMPILE_ROWS> nv(rows, m2.getCols());
		for (int y = 0; y < rows; y++)
			for (int x = 0; x < m2.getCols(); x++) {
				nv.at(y,x) = 0;
				for (int i = 0; i < cols; i++)
					nv.at(y,x) += at(y,i) * m2.at(i,x);
			}
		return nv;
	}
private:
	/**
	 * Initializes the matrix as empty matrix.
	 */
	void initEmpty(){
		m = NULL;
		cols = 0;
		rows = 0;
	}
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

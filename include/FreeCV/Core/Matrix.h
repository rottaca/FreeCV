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
#include <math.h>

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

	Matrix(T a00){
		initEmpty();
		init();
		for(int y = 0; y < rows; y++)
			for(int x = 0; x < cols; x++)
				at(y,x) = a00;

	}
	Matrix(T a00,T a01,T a10,T a11) {
		initEmpty();
		init();
		at(0,0) = a00;
		at(1,0) = a10;
		at(0,1) = a01;
		at(1,1) = a11;
	}
	Matrix(T a00,T a01,T a02,T a10,T a11,T a12,T a20,T a21,T a22) {
		initEmpty();
		init();
		at(0,0) = a00;
		at(0,1) = a01;
		at(0,2) = a02;
		at(1,0) = a10;
		at(1,1) = a11;
		at(1,2) = a12;
		at(2,0) = a20;
		at(2,1) = a21;
		at(2,2) = a22;
	}
	Matrix(T a00,T a01,T a02,T a03,T a10,T a11,T a12,T a13,T a20,T a21,T a22,T a23,T a30,T a31,T a32,T a33) {
		initEmpty();
		init();
		at(0,0) = a00;
		at(0,1) = a01;
		at(0,2) = a02;
		at(0,3) = a03;
		at(1,0) = a10;
		at(1,1) = a11;
		at(1,2) = a12;
		at(1,3) = a13;
		at(2,0) = a20;
		at(2,1) = a21;
		at(2,2) = a22;
		at(2,3) = a23;
		at(3,0) = a30;
		at(3,1) = a31;
		at(3,2) = a32;
		at(3,3) = a33;
	}

	Matrix() {
		initEmpty();
		init();
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
	void init(size_t rows = 0, size_t cols = 0, void* data = NULL) {
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
	inline T* getPtr(int row) const {
		assert(row < rows && row>=0);
		return &m[row * cols];
	}
	/**
	 * Returns a pointer to the specified matrix entry.
	 */
	inline T* getPtr(int row, int col) const {
		assert(row < rows && col < cols && row >= 0 && col >= 0);
		return &m[row * cols + col];
	}
	/**
	 * Returns a reference to the specified matrix entry.
	 * Attention no range check!
	 */
	T& at(int row, int col) {
		assert(row < rows && col < cols && row >= 0 && col >= 0);
		return m[row * cols + col];
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
			LOG_FORMAT_ERROR("Inverse Matrix not implemented for %dx%d Matrix",rows,rows);
			return 0;
		}

	}
	/**
	 * Decomposes the Matrix into a lower and upper triangle matrix (Doolittle factorization) where L*R=A.
	 */
	bool decomposeLU(Matrix<T,COMPILE_ROWS,COMPILE_COLS>& L,Matrix<T,COMPILE_ROWS,COMPILE_COLS>& U){
		assert(cols == rows);

		U = Matrix<T,COMPILE_ROWS,COMPILE_COLS>(*this);
		L = Matrix<T,COMPILE_ROWS,COMPILE_COLS>(rows,cols);

		// Init Lower left matrix. Zeros at the top right triangle.
		for (int c = 0; c < cols; c++) {
			for (int r = 0; r < c+1; r++) {
				if(r == c)
					L.at(r, c) = 1;
				else
					L.at(r, c) = 0;
			}
		}

		// Calculate upper right triangle matrix by eliminating all values in the lower left triangle matrix
		// Calculate lower left triangle matrix
		// For each column
		for(int c = 0; c < cols; c++){
			T r1 = U.at(c,c);// Get Entry on diagonal
			if(r1 == 0)
				return false;
			// For each row, calculate the coefficient l
			for(int r = c+1; r < rows; r++){
				L.at(r,c) = U.at(r,c)/r1;
				// For each entry in the current row, update the values
				for(int c2 = c; c2 < cols; c2++){
					U.at(r,c2) -= U.at(c,c2)*L.at(r,c);
				}
			}
		}

		return true;
	}


	Matrix<T,COMPILE_ROWS, COMPILE_COLS> inverse(){
		Matrix<T,COMPILE_ROWS, COMPILE_COLS> inv;
//		LOG_ERROR("Inverse Matrix not implemented yet!");
//		return inv;
		assert(rows == cols);

		Matrix<T,COMPILE_ROWS, COMPILE_COLS> L,U;
		decomposeLU(L,U);


//		LOG_FORMAT_ERROR("Inverse Matrix not implemented for %dx%d Matrix",rows,rows);

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
	/**
	 * Rotate the matrix around the given direction and by the given angle.
	 * Coordinate system: X-Right, Y-IN, Z-UP
	 */
	void rotate(Vector<T, 3> rotAxis, T angle) {
		// Code from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/

		rotAxis.normalize();

		double x = rotAxis[0];
		double y = rotAxis[1];
		double z = rotAxis[2];

		double x2 = x * x;
		double y2 = y * y;
		double z2 = z * z;
		double xy = x * y;
		double yz = y * z;
		double xz = x * z;

		double _1_cos = 1-cos(angle);
		double _cos = cos(angle);
		double _sin = sin(angle);

		Matrix<T, 4, 4> rotationMatrix;
		rotationMatrix.setIdentity();

		rotationMatrix.at(0,0) = x2*_1_cos + _cos;
		rotationMatrix.at(0,1) = xy*_1_cos - z*_sin;
		rotationMatrix.at(0,2) = xz*_1_cos + y*_sin;

		rotationMatrix.at(1,0) = xy*_1_cos + z*_sin;;
		rotationMatrix.at(1,1) = y2*_1_cos + _cos;
		rotationMatrix.at(1,2) = yz*_1_cos - x*_sin;

		rotationMatrix.at(2,0) = xz*_1_cos - y*_sin;
		rotationMatrix.at(2,1) = yz*_1_cos + x*_sin;
		rotationMatrix.at(2,2) = z2*_1_cos + _cos;

		// Apply rotation
		*this = rotationMatrix * (*this);
	}
	void translate(Vector<T, 3> Tr)
	{
		Matrix<T,4,4> translationMatrix;
		translationMatrix.setIdentity();
		translationMatrix.at(0,3) = Tr[0];
		translationMatrix.at(1,3) = Tr[1];
		translationMatrix.at(2,3) = Tr[2];

		// Apply translation
		*this = translationMatrix * (*this);
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
		Vector<T,COMPILE_ROWS> nv;

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

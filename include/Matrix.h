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

#include "Logger.h"
#include "Vector.h"

namespace fcv {

template<typename T, size_t COLS, size_t ROWS>
class Matrix {
public:
	Matrix():cols(COLS),rows(ROWS){
		memset(&m,0,COLS*ROWS*sizeof(T));
	}
	Matrix(const Matrix<T,COLS,ROWS>& m2):cols(COLS),rows(ROWS){
		memcpy(&m,&m2.m,COLS*ROWS*sizeof(T));
	}

	/**
	 * Returns Pointer to the MatrixData
	 */
	T* getPtr() {return m;}

	// Get ref to Component
	T& at(size_t col,size_t row) {return m[row][col];}

	size_t getCols(){return cols;}
	size_t getRows(){return rows;}

    std::string toString()
    {
    	std::stringstream s;
    	s << "Mat (Type ";
    	if(typeid(T) == typeid(double))
    		s << "double";
    	else if(typeid(T) == typeid(float))
    		s << "float";
    	else if(typeid(T) == typeid(int))
    		s << "int";
    	else if(typeid(T) == typeid(unsigned char))
    		s << "byte";
    	else
    		s << "unknown";

    	s << ", Size " << ROWS << " Rows x " << COLS << " Cols): \n";

    	for(int r = 0; r < ROWS-1; r++){
			for (int c = 0; c < COLS - 1; c++) {
				s << m[r][c] << ", ";
			}
			s << m[r][COLS - 1] << "\n";
    	}

		for (int c = 0; c < COLS - 1; c++) {
			s << m[ROWS-1][c] << ", ";
		}
		s << m[ROWS-1][COLS - 1];


    	return s.str();
    }
	////////////////////////////////////////
	// Operator overloading
	////////////////////////////////////////
	Matrix<T, COLS, ROWS> operator=(const Matrix<T, COLS, ROWS>& m2) {
		memcpy(m, m2.m, COLS * ROWS * sizeof(T));
		return *this;
	}

	Vector<T, ROWS> operator*(Vector<T, COLS> v2) {
		Vector<T, ROWS> nv;

		for(int r = 0; r < ROWS; r++){
			for (int c = 0; c < COLS; c++) {
				nv[r] += v2[c]*at(c,r);
			}
		}
		return nv;
	}

//	Matrix<T, COLS, ROWS> operator*(Matrix<T, ROWS, COLS> v2) {
//		Matrix<T, COLS, ROWS> nv(*this);
//		for (int i = 0; i < S; i++) {
//			nv[i] *= s;
//		}
//		return nv;
//	}

private:
	size_t cols,rows;
	// Contains Matrix Data
	T m[ROWS][COLS];
};

// Define default matrix types
typedef Matrix<double, 2,2> Matrix2x2d;
typedef Matrix<int, 2,2> Matrix2x2i;
typedef Matrix<unsigned char, 2,2> Matrix2x2b;

typedef Matrix<double, 3,3> Matrix3x3d;
typedef Matrix<int, 3,3> Matrix3x3i;
typedef Matrix<unsigned char, 3,3> Matrix3x3b;

typedef Matrix<double, 4,4> Matrix4x4d;
typedef Matrix<int, 4,4> Matrix4x4i;
typedef Matrix<unsigned char, 4,4> Matrix4x4b;

} /* namespace fcv */

#endif /* MATRIX_H_ */

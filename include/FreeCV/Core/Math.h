/*
 * Math.h
 *
 *  Created on: 12.05.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_CORE_MATH_H_
#define FREECV_INCLUDE_FREECV_CORE_MATH_H_

#include "FreeCV/Core/Matrix.h"
#include "FreeCV/Core/Vector.h"

namespace fcv {

template<typename T, size_t COMPILE_ROWS, size_t COMPILE_COLS>
bool solve(Matrix<T,COMPILE_ROWS,COMPILE_COLS>& A,Vector<T,COMPILE_COLS>& b,Vector<T,COMPILE_COLS>& x)
{
	assert(A.getRows() == A.getCols());
	assert(A.getCols() == x.getSize());
	assert(A.getCols() == b.getSize());

	size_t cols, rows;
	cols = rows = A.getCols();


	Matrix<T,COMPILE_ROWS,COMPILE_COLS> L(A.getCols(),A.getRows()),U(A.getCols(),A.getRows());
	if(!A.decomposeLU(L,U))
		return false;


	// Solve Ly = b
	Vector<T,COMPILE_COLS> y(x.getSize());
	for(int i=0; i < cols;i++)
	{
		y[i] = b[i];
		for(int j=0; j < i; j++){
			y[i] -= L.at(i,j)*y[j];
		}

		//y[i] /= L.at(i,i); // Skip that, L(i,i) = 1
	}
	// Solve Ux = y
	x = Vector<T,COMPILE_COLS>(x.getSize());
	for(int i=cols-1; i >= 0;i--)
	{
		x[i] = y[i];
		for(int j=i+1; j < cols; j++){
			x[i] -= U.at(i,j)*x[j];
		}

		x[i] /= U.at(i,i);
	}
	return true;
}

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_CORE_MATH_H_ */

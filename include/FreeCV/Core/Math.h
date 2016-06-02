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

#include <vector>

namespace fcv {

/**
 * Decomposes the Matrix into a lower and upper triangle matrix (Doolittle factorization) where L*R=P*A.
 */
template<typename T, size_t COMPILE_ROWS, size_t COMPILE_COLS>
bool decomposeLU(const Matrix<T,COMPILE_ROWS,COMPILE_COLS>& A,
		Matrix<T,COMPILE_ROWS,COMPILE_COLS>& LU,
		Matrix<T,COMPILE_ROWS,COMPILE_COLS>& P){
	assert(A.getCols() == A.getRows());

	int n = A.getCols();

	LU = A;

	// Create Index vector
	std::vector<unsigned int> p(n);
	for(unsigned int i = 0;i<n;i++)
		p[i] = i;

	// Calculate upper right triangle matrix by eliminating all values in the lower left triangle matrix
	// Calculate lower left triangle matrix
	// For each column
	for(unsigned int c = 0; c < n-1; c++){
		// Find pivot element in column c
		unsigned int pIdx = c;
		T pVal = fabs(LU.at(c,c));
		for(int i = c+1; i < n;i++)
		{
			if(fabs(LU.at(i,c))>pVal){
				pVal = fabs(LU.at(i,c));
				pIdx = i;
			}
		}
		// Swap pivot row
		int tmp = p[c];
		p[c] = p[pIdx];
		p[pIdx] = tmp;

		// For each row, calculate the coefficient l
		for(unsigned int r = c+1; r < n; r++){
			LU.at(p[r],c) = LU.at(p[r],c)/LU.at(p[c],c);
			// For each entry in the current row, update the values
			for(unsigned int c2 = c+1; c2 < n; c2++){
				LU.at(p[r],c2) -= LU.at(p[c],c2)*LU.at(p[r],c);
			}
		}
	}
	// Reconstruct P
	P = Matrix<T,COMPILE_ROWS,COMPILE_COLS>(n,n);
	P.setIdentity();
	for(unsigned int i = 0; i < n; i++)
	{
		int tmp = P.at(i,i) ;
		P.at(i,i)  = P.at(p[i],i);
		P.at(p[i],i)  = tmp;
	}

	// Sort each row back to normal
	LU = P*LU;

	return true;
}

template<typename T, size_t COMPILE_ROWS, size_t COMPILE_COLS>
bool solve(const Matrix<T,COMPILE_ROWS,COMPILE_COLS>& A, const Vector<T,COMPILE_COLS>& b,Vector<T,COMPILE_COLS>& x)
{
	assert(A.getRows() == A.getCols());
	assert(A.getCols() == x.getSize());
	assert(A.getCols() == b.getSize());

	size_t n = A.getCols();

	Matrix<T,COMPILE_ROWS,COMPILE_COLS> LU,P;
	if(!decomposeLU(A,LU,P))
		return false;

	Vector<T,COMPILE_COLS> b_ = P*b;

	// Solve Ly = b
	Vector<T,COMPILE_COLS> y(x.getSize());
	for(int i=0; i < n;i++)
	{
		y[i] = b_[i];
		for(int j=0; j < i; j++){
			if(i == j)
				y[i] -= y[j];
			else
				y[i] -= LU.at(i,j)*y[j];
		}

		//y[i] /= L.at(i,i); // Skip that, L(i,i) = 1
	}
	// Solve Ux = y
	x = Vector<T,COMPILE_COLS>(x.getSize());
	for(int i=n-1; i >= 0;i--)
	{
		x[i] = y[i];
		for(int j=i+1; j < n; j++){
			x[i] -= LU.at(i,j)*x[j];
		}

		x[i] /= LU.at(i,i);
	}
	return true;
}

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_CORE_MATH_H_ */

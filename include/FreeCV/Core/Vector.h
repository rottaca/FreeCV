/*
 * Vector.h
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "string.h"
#include "sstream"
#include <typeinfo>
#include "assert.h"
#include <algorithm>
#include <math.h>

#include "FreeCV/Core/Logger.h"
#include "FreeCV/Core/Macros.h"

namespace fcv {

template<typename T, size_t COMPILE_SIZE>
class Vector {
public:
	/**
	 * Creates a vector with the specified size.
	 * Attention: Parameter will be ignored if vector is a fixed size type.
	 */
//	Vector(size_t elemCnt){
//		v = NULL;
//		init(elemCnt);
//	}
	Vector(){
		v = NULL;
		cnt = 0;
		init();
	}
	/**
	 * Copy constructor
	 */
	Vector(const Vector<T, COMPILE_SIZE>& v2){
		v = NULL;
		init(v2.getSize(),v2.v);
	}

	// Constructs for different sized vectors
	Vector(T s){
		v = NULL;
		cnt = 0;
		init(s);

	}
	Vector(T v0,T v1){
		v = NULL;
		cnt = 0;
		init();
		v[0] = v0;
		v[1] = v1;
	}
	Vector(T v0,T v1,T v2){
		v = NULL;
		cnt = 0;
		init();
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
	Vector(T v0,T v1,T v2,T v3){
		v = NULL;
		cnt = 0;
		init();
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		v[3] = v3;
	}

	~Vector(){
		if(v != NULL)
			delete[] v;
	}
	/**
	 * Initializes the matrix with the specified size an copy the optional data into it.
	 * Attention: Size will be ignored if vector is a fixed size type.
	 * Attention: Data should have the correct size (elemCnt*sizeof(T))
	 */
	void init(size_t elemCnt = 0, void* data = NULL){
		if(v != NULL)
			delete[] v;

		v = NULL;

		if(COMPILE_SIZE != FCV_DYNAMIC_SIZE)
			elemCnt = COMPILE_SIZE;

		if(elemCnt <= 0)
			return;

		cnt = elemCnt;
		v = new T[cnt];
		if(data != NULL)
		{
			memcpy(v,data,cnt*sizeof(T));
		}
		else
		{
			memset(v,0,cnt*sizeof(T));
		}
	}

	/**
	 * Returns Pointer to the first entry in the vector
	 */
	T* getPtr()const  {return v;}
	/**
	 * Set Vector Component
	 */
	void set(int i, T val) {assert(i< cnt);v[i] = val;}
	/**
	 * Get Vector Component
	 */
	T get(int i)const  {assert(i< cnt);return v[i];}
	/**
	 * Returns vector component count
	 */
	size_t getSize()const {return cnt;}
	/**
	 * Formats the vector as string
	 */
    std::string toString() const
    {
    	std::stringstream s;
    	s << "Vect (Type ";
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

    	s << ", Size " << cnt << "): ";

    	for(int i = 0; i < cnt-1; i++)
    		s << v[i] << ", ";

		s << v[cnt-1];

    	return s.str();
    }
    /**
     * Returns the maximum value in the vector
     */
	T getMax() const {
		return *std::max_element(v, v+cnt);
	}
    /**
     * Returns the minimum value in the vector
     */
	T getMin() const {
		return *std::min_element(v, v+cnt);
	}
	T getLength() const{
		double l = 0;

		for(int i = 0; i < cnt; i++)
			l += (*this)[i]*(*this)[i];
		return sqrt(l);
	}
	void normalize(){
		*this = *this * (1.0 / getLength());
	}
	Vector<T,COMPILE_SIZE> normalized()const{
		return Vector(*this * (1.0 / getLength()));
	}

	////////////////////////////////////////
	// Operator overloading
	////////////////////////////////////////
	// Get Value reference
    T& operator[](size_t i){
    	assert(i< cnt);

    	return v[i];
    }
    const T& operator[](size_t i) const{
    	assert(i< cnt);

    	return v[i];
    }

    Vector<T,COMPILE_SIZE>& operator=(const Vector<T,COMPILE_SIZE>& v2){
    	init(v2.getSize(),v2.v);
    	return *this;
    }

    Vector<T,COMPILE_SIZE> operator+(const Vector<T,COMPILE_SIZE>& v2) const
    {
    	assert(v2.getSize() == cnt);
    	Vector<T,COMPILE_SIZE> nv(*this);
    	for(int i = 0; i < cnt;i++)
    		nv[i] += v2.get(i);
    	return nv;
    }
    Vector<T,COMPILE_SIZE>& operator+=(const Vector<T,COMPILE_SIZE>& v2)
    {
    	assert(v2.getSize() == cnt);
    	for(int i = 0; i < cnt;i++)
    		v[i] += v2.get(i);
    	return *this;
    }
    Vector<T,COMPILE_SIZE> operator-(const Vector<T,COMPILE_SIZE>& v2) const
    {
    	assert(v2.getSize() == cnt);
    	Vector<T,COMPILE_SIZE> nv(*this);
    	for(int i = 0; i < cnt;i++)
    		nv[i] -= v2.get(i);
    	return nv;
    }
    Vector<T,COMPILE_SIZE>& operator-=(const Vector<T,COMPILE_SIZE>& v2)
    {
    	assert(v2.getSize() == cnt);
    	for(int i = 0; i < cnt;i++)
    		v[i] -= v2.get(i);
    	return *this;
    }
    Vector<T,COMPILE_SIZE> operator-()
    {
    	Vector<T,COMPILE_SIZE> nv;
    	for(int i = 0; i < cnt;i++)
    		nv[i] = -v[i];
    	return nv;
    }

    Vector<T,COMPILE_SIZE> operator*(const T& s) const
    {
    	Vector<T,COMPILE_SIZE> nv(*this);
    	for(int i = 0; i < cnt;i++){
    		nv[i] *= s;
    	}
    	return nv;
    }

    Vector<T,COMPILE_SIZE> operator/(const T& s) const
    {
    	Vector<T,COMPILE_SIZE> nv(*this);
    	for(int i = 0; i < cnt;i++){
    		nv[i] /= s;
    	}
    	return nv;
    }

    Vector<T,COMPILE_SIZE> operator*(const Vector<T,COMPILE_SIZE>& v2) const
    {
    	assert(v2.getSize() == getSize());
    	Vector<T,COMPILE_SIZE> vNew(*this);

    	for(int i = 0; i < getSize();i++){
    		vNew[i] *= v2.get(i);
    	}
    	return vNew;
    }
    Vector<T,COMPILE_SIZE>& operator*=(const Vector<T,COMPILE_SIZE>& v2)
    {
    	assert(v2.getSize() == getSize());
    	Vector<T,COMPILE_SIZE> vNew(*this);

    	for(int i = 0; i < getSize();i++){
    		v[i] *= v2.get(i);
    	}
    	return *this;
    }
    Vector<T,COMPILE_SIZE>& operator/=(const Vector<T,COMPILE_SIZE>& v2)
    {
    	assert(v2.getSize() == getSize());
    	Vector<T,COMPILE_SIZE> vNew(*this);

    	for(int i = 0; i < getSize();i++){
    		v[i] /= v2.get(i);
    	}
    	return *this;
    }

    /**
     * Scalar product
     */
    T dot(const Vector<T,COMPILE_SIZE> v2)
    {
		T val = 0;
		for (int i = 0; i < getSize(); i++) {
			val += v[i] * v2[i];
		}
		return val;
    }

    Vector<T,COMPILE_SIZE> cross(Vector<T,COMPILE_SIZE> v2){
    	assert(v2.getSize() == 3);
    	assert(getSize() == 3);

    	 Vector<T,COMPILE_SIZE> vn;
    	 vn[0] = v[1]*v2[2] - v[2]*v2[1];
    	 vn[1] = v[2]*v2[0] - v[0]*v2[2];
    	 vn[2] = v[0]*v2[1] - v[1]*v2[0];

    	return vn;
    }


private:
	// Contains Vector Data
	T* v;
	size_t cnt;

};

// Define some vector types with fixed size and type
typedef Vector<unsigned char,FCV_DYNAMIC_SIZE> Vectorb;
typedef Vector<float,FCV_DYNAMIC_SIZE> Vectord;
typedef Vector<float,FCV_DYNAMIC_SIZE> Vectorf;
typedef Vector<int,FCV_DYNAMIC_SIZE> Vectori;

typedef Vector<unsigned char,2> Vector2b;
typedef Vector<float,2> Vector2d;
typedef Vector<float,2> Vector2f;
typedef Vector<int,2> Vector2i;

typedef Vector<unsigned char,3> Vector3b;
typedef Vector<float,3> Vector3d;
typedef Vector<float,3> Vector3f;
typedef Vector<int,3> Vector3i;

typedef Vector<unsigned char,4> Vector4b;
typedef Vector<float,4> Vector4d;
typedef Vector<float,4> Vector4f;
typedef Vector<int,4> Vector4i;

} /* namespace fcv */

#endif /* VECTOR_H_ */

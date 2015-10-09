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

#include "Logger.h"
namespace fcv {

template<typename T, size_t S>
class Vector {
public:
	Vector():cnt(S){
		memset(&v,0,S*sizeof(T));
	}
	Vector(const Vector<T,S>& v2):cnt(S){
		memcpy(&v,&v2.v,S*sizeof(T));
	}

	/**
	 * Returns Pointer to the VectorData
	 */
	T* getPtr() {return v;}
	// Set Vector Component
	void set(int i, T val) {v[i] = val;}
	// Get Vector Component
	T get(int i) {return v[i];}

    std::string toString()
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

    	s << ", Size " << S << "): ";

    	for(int i = 0; i < S-1; i++)
    		s << v[i] << ", ";

		s << v[S-1];

    	return s.str();
    }

	////////////////////////////////////////
	// Operator overloading
	////////////////////////////////////////
	// Get Value reference
    T& operator[](size_t i){
    	if(i >= S){
    		LOG_ERROR("Index out of bounds!");
    		i = 0;
    	}
    	return v[i];
    }

    Vector<T,S> operator=(const Vector<T,S>& v2){
    	memcpy(v,v2.v,S*sizeof(T));
    	return *this;
    }

    Vector<T,S> operator+(Vector<T,S>& v2)
    {
    	Vector<T,S> nv(*this);
    	for(int i = 0; i < S;i++)
    		nv[i] += v2[i];
    	return nv;
    }

    Vector<T,S> operator*(T s)
    {
    	Vector<T,S> nv(*this);
    	for(int i = 0; i < S;i++){
    		nv[i] *= s;
    	}
    	return nv;
    }

//    Vector<T,S> operator*(Vector<T,S> v2)
//    {
//    	Vector<T,S> nv(*this);
//    	for(int i = 0; i < S;i++){
//    		nv[i] *= s;
//    	}
//    	return nv;
//    }


private:
	// Contains Vector Data
	T v[S];
	size_t cnt;

};

// Define default vector types
typedef Vector<double, 2> Vector2d;
typedef Vector<int, 2> Vector2i;
typedef Vector<unsigned char, 2> Vector2b;

typedef Vector<double, 3> Vector3d;
typedef Vector<int, 3> Vector3i;
typedef Vector<unsigned char, 3> Vector3b;

typedef Vector<double, 4> Vector4d;
typedef Vector<int, 4> Vector4i;
typedef Vector<unsigned char, 4> Vector4b;

} /* namespace fcv */

#endif /* VECTOR_H_ */

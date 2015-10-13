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

#include "Core/Logger.h"
namespace fcv {

template<typename T>
class Vector {
public:
	Vector(size_t elemCnt){
		v = NULL;
		init(elemCnt);
	}
	Vector(){
		v = NULL;
		cnt = 0;
	}
	Vector(const Vector<T>& v2){
		v = NULL;
		init(v2.getSize(),v2.v);
	}
	~Vector(){
		if(v != NULL)
			delete[] v;
	}
	void init(size_t elemCnt, void* data = NULL){
		cnt = elemCnt;
		if(v != NULL)
			delete[] v;
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
	 * Returns Pointer to the VectorData
	 */
	T* getPtr()const  {return v;}
	// Set Vector Component
	void set(int i, T val) {assert(i< cnt);v[i] = val;}
	// Get Vector Component
	T get(int i)const  {assert(i< cnt);return v[i];}
	// Get Vector Component Cnt
	size_t getSize()const {return cnt;}

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

	T getMax() {
		T max = v[0];
		for (int i = 0; i < cnt; i++)
			if (v[i] > max)
				max = v[i];

		return max;
	}
	T getMin() {
		T min = v[0];
		for (int i = 0; i < cnt; i++)
			if (v[i] < min)
				min = v[i];

		return min;
	}


	////////////////////////////////////////
	// Operator overloading
	////////////////////////////////////////
	// Get Value reference
    T& operator[](size_t i){
    	assert(i< cnt);

    	return v[i];
    }

    Vector<T> operator=(const Vector<T>& v2){
    	init(v2.getSize(),v2.v);
    	return *this;
    }

    Vector<T> operator+(Vector<T>& v2)
    {
    	assert(v2.getSize() == cnt);
    	Vector<T> nv(*this);
    	for(int i = 0; i < cnt;i++)
    		nv[i] += v2[i];
    	return nv;
    }

    Vector<T> operator*(T s)
    {
    	Vector<T> nv(*this);
    	for(int i = 0; i < cnt;i++){
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
	T* v;
	size_t cnt;

};


} /* namespace fcv */

#endif /* VECTOR_H_ */

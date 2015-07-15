#pragma once

#include <stdexcept>



template<class T, int SIZE>
class Vector
{
public:

	// -- CREATORS --
	Vector(T t = 0);

	template<class T2, int SIZE2>
	explicit Vector(Vector<T2, SIZE2> v, T t = 0);

	// -- MANIPULATORS --

	Vector& operator+= (const Vector& rhs);
	Vector& operator-= (const Vector& rhs);
	Vector& operator*= (float rhs);

	Vector& normalize();

	T& operator[](int idx);

	// -- ACCESSORS --


	const T& operator[](int idx) const;

	T dot(const Vector& lhs) const;
	Vector getNormalized() const;
	float getMagnitude() const;
	float getMagnitudeSq() const;



protected:
	T m_Ts[SIZE];
};

template <class T, int SIZE>
Vector<T, SIZE> operator +(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs);

template <class T, int SIZE>
Vector<T, SIZE> operator -(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs);

template <class T, int SIZE>
Vector<T, SIZE> operator*(const Vector<T, SIZE>& lhs, T rhs);

template <class T, int SIZE>
Vector<T, SIZE> operator*(T lhs, const Vector<T, SIZE>& rhs);

template <class T, int SIZE>
T operator *(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs);

template <class T, int SIZE>
bool operator ==(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs);


#include "Vector.hpp"

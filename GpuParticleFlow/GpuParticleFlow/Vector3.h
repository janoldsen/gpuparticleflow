#pragma once


#include <stdexcept>
#include "Vector.h"


template<class T>
class Vector3 : public Vector < T, 3 >
{
public:

	Vector3(T t = 0);
	Vector3(const Vector<T, 3>& v);
	Vector3(T x, T y, T z);

	template <class T2, int SIZE2>
	explicit Vector3(const Vector<T2, SIZE2>& T2, T t = 0);


	// -- MANIPULATORS --

	T& x();
	T& y();
	T& z();

	// -- ACCESSORS --
	const T& x() const;
	const T& y() const;
	const T& z() const;

	Vector3 cross(const Vector3& lhs) const;

private:


};


typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;


#include "Vector3.hpp"


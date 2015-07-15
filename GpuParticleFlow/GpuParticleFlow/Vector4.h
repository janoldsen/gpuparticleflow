#pragma once

#include <stdexcept>
#include "Vector.h"


template<class T>
class Vector4 : public Vector < T, 4 >
{
public:

	Vector4(T t = 0);
	Vector4(const Vector<T, 4>& v);
	Vector4(T x, T y, T z, T w);

	template <class T2, int SIZE2>
	explicit Vector4(const Vector<T2, SIZE2>& T2, T t = 0);


	// -- MANIPULATORS --

	T& x();
	T& y();
	T& z();
	T& w();

	// -- ACCESSORS --
	const T& x() const;
	const T& y() const;
	const T& z() const;
	const T& w() const;

private:


};



typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;

#include "Vector4.hpp"

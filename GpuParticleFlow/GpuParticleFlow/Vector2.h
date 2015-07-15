#pragma once

#include <stdexcept>
#include "Vector.h"

template<class T>
class Vector2 : public Vector < T, 2 >
{
public:

	Vector2(T t = 0);
	Vector2(const Vector<T, 2>& v);
	Vector2(T x, T y);

	template <class T2, int SIZE2>
	explicit Vector2(const Vector<T2, SIZE2>& T2, T t = 0);


	// -- MANIPULATORS --

	T& x();
	T& y();

	// -- ACCESSORS --
	const T& x() const;
	const T& y() const;

private:


};


typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

#include "Vector2.hpp"


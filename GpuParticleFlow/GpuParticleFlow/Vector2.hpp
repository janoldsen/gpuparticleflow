#pragma once


template <class T>
inline Vector2<T>::Vector2(T t)
	: Vector(t)
{
}

template <class T>
inline Vector2<T>::Vector2(const Vector<T, 2>& v)
	: Vector(v)
{
}

template <class T>
inline Vector2<T>::Vector2(T x, T y)
{
	m_Ts[0] = x;
	m_Ts[1] = y;
}



template <class T>
template<class T2, int SIZE2>
inline Vector2<T>::Vector2(const Vector<T2, SIZE2>& v, T t = 0)
	: Vector(v, t)
{
}



template<class T>
T& Vector2<T>::x()
{
	return m_Ts[0];
}

template<class T>
const T& Vector2<T>::x() const
{
	return m_Ts[0];
}

template<class T>
T& Vector2<T>::y()
{
	return m_Ts[1];
}

template<class T>
const T& Vector2<T>::y() const
{
	return m_Ts[1];
}

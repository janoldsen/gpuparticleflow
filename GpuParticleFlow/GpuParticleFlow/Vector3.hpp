#pragma once


template <class T>
inline Vector3<T>::Vector3(T t)
	: Vector(t)
{
}

template <class T>
inline Vector3<T>::Vector3(const Vector<T, 3>& v)
	: Vector(v)
{
}

template <class T>
inline Vector3<T>::Vector3(T x, T y, T z)
{
	m_Ts[0] = x;
	m_Ts[1] = y;
	m_Ts[2] = z;
}



template <class T>
template<class T2, int SIZE2>
inline Vector3<T>::Vector3(const Vector<T2, SIZE2>& v, T t = 0)
	: Vector(v, t)
{
}


template <class T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& lhs) const
{
	Vector3<T> v;
	v.m_Ts[0] = m_Ts[1] * lhs.m_Ts[2] - m_Ts[2] * lhs.m_Ts[1];
	v.m_Ts[1] = m_Ts[2] * lhs.m_Ts[0] - m_Ts[0] * lhs.m_Ts[2];
	v.m_Ts[2] = m_Ts[0] * lhs.m_Ts[1] - m_Ts[1] * lhs.m_Ts[0];

	return v;
}


template<class T>
T& Vector3<T>::x()
{
	return m_Ts[0];
}

template<class T>
const T& Vector3<T>::x() const
{
	return m_Ts[0];
}

template<class T>
T& Vector3<T>::y()
{
	return m_Ts[1];
}

template<class T>
const T& Vector3<T>::y() const
{
	return m_Ts[1];
}


template<class T>
T& Vector3<T>::z()
{
	return m_Ts[2];
}

template<class T>
const T& Vector3<T>::z() const
{
	return m_Ts[2];
}



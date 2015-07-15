#pragma once


template <class T>
inline Vector4<T>::Vector4(T t)
	: Vector(t)
{
}

template <class T>
inline Vector4<T>::Vector4(const Vector<T, 4>& v)
	: Vector(v)
{
}

template <class T>
inline Vector4<T>::Vector4(T x, T y, T z, T w)
{
	m_Ts[0] = x;
	m_Ts[1] = y;
	m_Ts[2] = z;
	m_Ts[3] = w;
}



template <class T>
template<class T2, int SIZE2>
inline Vector4<T>::Vector4(const Vector<T2, SIZE2>& v, T t = 0)
	: Vector(v, t)
{
}



template<class T>
T& Vector4<T>::x()
{
	return m_Ts[0];
}

template<class T>
const T& Vector4<T>::x() const
{
	return m_Ts[0];
}

template<class T>
T& Vector4<T>::y()
{
	return m_Ts[1];
}

template<class T>
const T& Vector4<T>::y() const
{
	return m_Ts[1];
}


template<class T>
T& Vector4<T>::z()
{
	return m_Ts[2];
}

template<class T>
const T& Vector4<T>::z() const
{
	return m_Ts[2];
}

template<class T>
T& Vector4<T>::w()
{
	return m_Ts[2];
}

template<class T>
const T& Vector4<T>::w() const
{
	return m_Ts[2];
}

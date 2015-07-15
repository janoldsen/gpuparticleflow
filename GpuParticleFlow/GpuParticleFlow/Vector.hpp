


template <class T, int SIZE>
Vector<T, SIZE>::Vector(T t)
{
	for (auto m_T : m_Ts)
	{
		m_T = t;
	}
}

template <class T, int SIZE>
template <class T2, int SIZE2>
Vector<T, SIZE>::Vector(Vector<T2, SIZE2> v, T t)
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (i < SIZE2)
			m_Ts[i] = v[i];
		else
			m_Ts[i] = t;
	}
}


template <class T, int SIZE>
T& Vector<T, SIZE>::operator[](int idx)
{
	if (idx >= SIZE)
	{
		throw std::out_of_range("bad index");
	}

	return m_Ts[idx];
}



template <class T, int SIZE>
Vector<T, SIZE>& Vector<T, SIZE>::normalize()
{
	float magnitude = getMagnitude();
	for (auto m_T : m_Ts)
	{
		m_T /= magnitude;
	}
	return *this;
}

template <class T, int SIZE>
Vector<T, SIZE>& Vector<T, SIZE>::operator+=(const Vector& lhs)
{
	for (int i = 0; i < SIZE; ++i)
	{
		m_Ts[i] += lhs.m_Ts[i];
	}
	return *this;
}


template <class T, int SIZE>
Vector<T, SIZE>& Vector<T, SIZE>::operator-=(const Vector& lhs)
{
	for (int i = 0; i < SIZE; ++i)
	{
		m_Ts[i] -= lhs.m_Ts[i];
	}
	return *this;
}


template <class T, int SIZE>
Vector<T, SIZE>& Vector<T, SIZE>::operator*=(float lhs)
{
	for (int i = 0; i < SIZE; i++)
	{
		m_Ts[i] *= lhs;
	}
	return *this;
}


template <class T, int SIZE>
const T& Vector<T, SIZE>::operator[](int idx) const
{
	if (idx >= SIZE)
	{
		throw std::out_of_range("bad index");
	}

	return m_Ts[idx];
}



template <class T, int SIZE>
T Vector<T, SIZE>::dot(const Vector<T, SIZE>& lhs) const
{
	float result = 0.0f;
	for (int i = 0; i < SIZE; ++i)
	{
		result += m_Ts[i] * lhs.m_Ts[i];
	}
	return result;
}


template<class T, int SIZE>
Vector<T, SIZE> Vector<T, SIZE>::getNormalized() const
{
	return Vector(*this).normalize();
}


template<class T, int SIZE>
float Vector<T, SIZE>::getMagnitude() const
{
	return sqrt(getMagnitudeSq());
}

template<class T, int SIZE>
float Vector<T, SIZE>::getMagnitudeSq() const
{
	float result = 0.0f;
	for (auto t : m_Ts)
	{
		result += t * t;
	}
	return result;
}


template<class T, int SIZE>
Vector<T, SIZE> operator +(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs)
{
	return Vector<T, SIZE>(lhs) += rhs;
}

template<class T, int SIZE>
Vector<T, SIZE> operator -(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs)
{
	return Vector<T, SIZE>(lhs) -= rhs;
}

template<class T, int SIZE>
Vector<T, SIZE> operator *(const Vector<T, SIZE>& lhs, T rhs)
{
	return Vector<T, SIZE>(lhs) *= rhs;
}

template<class T, int SIZE>
Vector<T, SIZE> operator*(T lhs, const Vector<T, SIZE>& rhs)
{
	return Vector<T, SIZE>(rhs) *= lhs;
}

template <class T, int SIZE>
T operator *(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs)
{
	return lhs.dot(rhs);
}

template <class T, int SIZE>
bool operator ==(const Vector<T, SIZE>& lhs, const Vector<T, SIZE>& rhs)
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (lhs[i] != rhs[i])
			return false;
	}
	return true;
}



template <class T>
Matrix4x4<T>::Matrix4x4(
	T m00, T m01, T m02, T m03,
	T m10, T m11, T m12, T m13,
	T m20, T m21, T m22, T m23,
	T m30, T m31, T m32, T m33)
{
	m_T[0] = Vector4<T>(m00, m10, m20, m30);
	m_T[1] = Vector4<T>(m01, m11, m21, m31);
	m_T[2] = Vector4<T>(m02, m12, m22, m32);
	m_T[3] = Vector4<T>(m03, m13, m23, m33);
}


template <class T>
Vector4<T>& Matrix4x4<T>::operator[] (int idx)
{
	return m_T[idx];
}


template <class T>
const Vector4<T>& Matrix4x4<T>::operator[] (int idx) const
{
	return m_T[idx];
}


template<class T>
Matrix4x4<T> Matrix4x4<T>::getTranspose() const
{
	Matrix4x4<T> m;

	m.m_T[0] = Vector4<T>(m_T[0][0], m_T[1][0], m_T[2][0], m_T[3][0]);
	m.m_T[1] = Vector4<T>(m_T[0][1], m_T[1][1], m_T[2][1], m_T[3][1]);
	m.m_T[2] = Vector4<T>(m_T[0][2], m_T[1][2], m_T[2][2], m_T[3][2]);
	m.m_T[3] = Vector4<T>(m_T[0][3], m_T[1][3], m_T[2][3], m_T[3][3]);

	return m;

}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getInverse() const
{
	Matrix4x4<T> m;


	return m;

}



template <class T>
Matrix4x4<T> Matrix4x4<T>::getScale(const Vector3f& scale)
{
	return Matrix4x4(
		scale.x(), 0.0f, 0.0f, 0.0f,
		0.0f, scale.y(), 0.0f, 0.0f,
		0.0f, 0.0f, scale.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

template <class T>
Matrix4x4<T> Matrix4x4<T>::getTranslation(const Vector3f& position)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		position.x(), position.y(), position.z(), 1.0f);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getRotationX(const float angle)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(angle), -sin(angle), 0.0f,
		0.0f, sin(angle), cos(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getRotationY(const float angle)
{
	return Matrix4x4(
		cos(angle), 0.0f, sin(angle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(angle), 0.0f, cos(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getRotationZ(const float angle)
{
	return Matrix4x4(
		cos(angle), -sin(angle), 0.0f, 0.0f,
		sin(angle), cos(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getRotation(const Vector3f& angle)
{
	return getRotationZ(angle.z()) * getRotationY(angle.y()) * getRotationX(angle.x());
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getRotation(const Quaternion& quaternion)
{
	float x2 = 2 * quaternion.getV().x();
	float y2 = 2 * quaternion.getV().y();
	float z2 = 2 * quaternion.getV().z();

	float xx2 = quaternion.getV().x() * x2;
	float yy2 = quaternion.getV().y() * y2;
	float zz2 = quaternion.getV().z() * z2;

	float yz2 = quaternion.getV().y() * z2;
	float wx2 = quaternion.getS() * x2;

	float xy2 = quaternion.getV().x() * y2;
	float wz2 = quaternion.getS() * z2;

	float xz2 = quaternion.getV().x() * z2;
	float wy2 = quaternion.getS() * y2;

	return Matrix4x4(
		1 - yy2 - zz2,	xy2 + wz2,				xz2 - wy2,		0,
		xy2 - wz2,				1 - xx2 - zz2,	yz2 + wx2,		0,
		xz2 + wy2,				yz2 - wx2,				1 - xx2 - yy2,	0,
		0,						0,						0,				1
		);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getTransformation(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
{
	return getScale(scale) * getRotation(rotation) * getTranslation(position);
}



template<class T>
Matrix4x4<T> Matrix4x4<T>::getTransformation(const Vector3f& position, const Quaternion& rotation, const Vector3f& scale)
{
	return getScale(scale) * getRotation(rotation) * getTranslation(position);
}

template<class T>
Matrix4x4<T> Matrix4x4<T>::getProjection(float fov, float aspect, float nearPlane, float farPlane)
{
	float xScale = 1.0f / tan(fov / 2.0f);
	float yScale = xScale / aspect;

	return Matrix4x4<T>(
		yScale, 0.0f, 0.0f, 0.0f,
		0.0f, xScale, 0.0f, 0.0f,
		0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f,
		0.0f, 0.0f, (-nearPlane * farPlane) / (farPlane - nearPlane), 0.0f);

}


template<class T>
Matrix4x4<T> Matrix4x4<T>::getView(const Vector3f& pos, const Vector3f& angle)
{
	Matrix4x4f rotation = getRotation(-angle);
	Vector3f xAxis = Vector3f(rotation[0]).normalize();
	Vector3f yAxis = Vector3f(rotation[1]).normalize();
	Vector3f zAxis = Vector3f(rotation[2]).normalize();

	return Matrix4x4f(
		xAxis.x(), yAxis.x(), zAxis.x(), 0,
		xAxis.y(), yAxis.y(), zAxis.y(), 0,
		xAxis.z(), yAxis.z(), zAxis.z(), 0,
		-xAxis.dot(pos), -yAxis.dot(pos), -zAxis.dot(pos), 1);



}


template<class T>
Matrix4x4<T> Matrix4x4<T>::getView(const Vector3f& pos, const Vector3f& look, const Vector3f up)
{
	Vector3f right = look.cross(up);

	return Matrix4x4(
		right.x(), up.x(), look.x(), 0,
		right.y(), up.y(), look.y(), 0,
		right.z(), up.z(), look.z(), 0,
		-(right*pos), -(up*pos), -(look*pos), 1);
}



template<class T>
Matrix4x4<T> Matrix4x4<T>::getLookAt(const Vector3f& pos, const Vector3f& toLookAt, const Vector3f& up)
{
	Vector3f zAxis = (toLookAt - pos).normalize();
	Vector3f xAxis = up.cross(zAxis).normalize();
	Vector3f yAxis = zAxis.cross(xAxis);

	return Matrix4x4f(
		xAxis.x(), yAxis.x(), zAxis.x(), 0,
		xAxis.y(), yAxis.y(), zAxis.y(), 0,
		xAxis.z(), yAxis.z(), zAxis.z(), 0,
		-xAxis.dot(pos), -yAxis.dot(pos), -zAxis.dot(pos), 1);
}


template<class T>
Vector4<T> operator*(const Vector4<T>& lhs, const Matrix4x4<T>& rhs)
{
	return Vector4<T>(
		lhs.dot(rhs[0]),
		lhs.dot(rhs[1]),
		lhs.dot(rhs[2]),
		lhs.dot(rhs[3])
		);
}






template<class T>
Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4& lhs) const
{
	return Matrix4x4
		(
		m_T[0][0] * lhs.m_T[0][0] + m_T[1][0] * lhs.m_T[0][1] + m_T[2][0] * lhs.m_T[0][2] + m_T[3][0] * lhs.m_T[0][3],
		m_T[0][0] * lhs.m_T[1][0] + m_T[1][0] * lhs.m_T[1][1] + m_T[2][0] * lhs.m_T[1][2] + m_T[3][0] * lhs.m_T[1][3],
		m_T[0][0] * lhs.m_T[2][0] + m_T[1][0] * lhs.m_T[2][1] + m_T[2][0] * lhs.m_T[2][2] + m_T[3][0] * lhs.m_T[2][3],
		m_T[0][0] * lhs.m_T[3][0] + m_T[1][0] * lhs.m_T[3][1] + m_T[2][0] * lhs.m_T[3][2] + m_T[3][0] * lhs.m_T[3][3],
																				   						   
		m_T[0][1] * lhs.m_T[0][0] + m_T[1][1] * lhs.m_T[0][1] + m_T[2][1] * lhs.m_T[0][2] + m_T[3][1] * lhs.m_T[0][3],
		m_T[0][1] * lhs.m_T[1][0] + m_T[1][1] * lhs.m_T[1][1] + m_T[2][1] * lhs.m_T[1][2] + m_T[3][1] * lhs.m_T[1][3],
		m_T[0][1] * lhs.m_T[2][0] + m_T[1][1] * lhs.m_T[2][1] + m_T[2][1] * lhs.m_T[2][2] + m_T[3][1] * lhs.m_T[2][3],
		m_T[0][1] * lhs.m_T[3][0] + m_T[1][1] * lhs.m_T[3][1] + m_T[2][1] * lhs.m_T[3][2] + m_T[3][1] * lhs.m_T[3][3],
																					   						   
		m_T[0][2] * lhs.m_T[0][0] + m_T[1][2] * lhs.m_T[0][1] + m_T[2][2] * lhs.m_T[0][2] + m_T[3][2] * lhs.m_T[0][3],
		m_T[0][2] * lhs.m_T[1][0] + m_T[1][2] * lhs.m_T[1][1] + m_T[2][2] * lhs.m_T[1][2] + m_T[3][2] * lhs.m_T[1][3],
		m_T[0][2] * lhs.m_T[2][0] + m_T[1][2] * lhs.m_T[2][1] + m_T[2][2] * lhs.m_T[2][2] + m_T[3][2] * lhs.m_T[2][3],
		m_T[0][2] * lhs.m_T[3][0] + m_T[1][2] * lhs.m_T[3][1] + m_T[2][2] * lhs.m_T[3][2] + m_T[3][2] * lhs.m_T[3][3],
																					   						   
		m_T[0][3] * lhs.m_T[0][0] + m_T[1][3] * lhs.m_T[0][1] + m_T[2][3] * lhs.m_T[0][2] + m_T[3][3] * lhs.m_T[0][3],
		m_T[0][3] * lhs.m_T[1][0] + m_T[1][3] * lhs.m_T[1][1] + m_T[2][3] * lhs.m_T[1][2] + m_T[3][3] * lhs.m_T[1][3],
		m_T[0][3] * lhs.m_T[2][0] + m_T[1][3] * lhs.m_T[2][1] + m_T[2][3] * lhs.m_T[2][2] + m_T[3][3] * lhs.m_T[2][3],
		m_T[0][3] * lhs.m_T[3][0] + m_T[1][3] * lhs.m_T[3][1] + m_T[2][3] * lhs.m_T[3][2] + m_T[3][3] * lhs.m_T[3][3]
		);
}

template<class T>
Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4& lhs)
{
	*this = *this * lhs;
	return *this;
}

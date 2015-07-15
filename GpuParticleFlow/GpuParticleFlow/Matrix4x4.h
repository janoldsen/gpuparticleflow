#pragma once

#include "Vector4.h"
#include "Vector3.h"
#include "Quaternion.h"


template <class T>
class Matrix4x4
{
public:

	// -- CREATORS --

	Matrix4x4(T m00 = 1, T m01 = 0, T m02 = 0, T m03 = 0,
		T m10 = 0, T m11 = 1, T m12 = 0, T m13 = 0,
		T m20 = 0, T m21 = 0, T m22 = 1, T m23 = 0,
		T m30 = 0, T m31 = 0, T m32 = 0, T m33 = 1);


	// -- MANIPULATORS --



	Matrix4x4 operator*(const Matrix4x4& lhs) const;
	Matrix4x4& operator*=(const Matrix4x4& lhs);


	// -- ACCESSORS--

	Vector4<T>& operator[](int idx);
	const Vector4<T>& operator[](int idx) const;

	Matrix4x4 getTranspose() const;
	Matrix4x4 getInverse() const;


	static Matrix4x4 getScale(const Vector3f& scale);
	static Matrix4x4 getTranslation(const Vector3f& position);
	static Matrix4x4 getRotationX(const float angle);
	static Matrix4x4 getRotationY(const float angle);
	static Matrix4x4 getRotationZ(const float angle);
	static Matrix4x4 getRotation(const Vector3f& angle);
	static Matrix4x4 getRotation(const Quaternion& quaternion);


	static Matrix4x4 getTransformation(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale);
	static Matrix4x4 getTransformation(const Vector3f& position, const Quaternion& roatation, const Vector3f& scale);

	static Matrix4x4 getProjection(float fov, float aspect, float nearPlane, float farPlane);
	static Matrix4x4 getView(const Vector3f& pos, const Vector3f& angle);
	static Matrix4x4 getView(const Vector3f& pos, const Vector3f& look, const Vector3f up);
	static Matrix4x4 getLookAt(const Vector3f& pos, const Vector3f& toLookAt, const Vector3f& up);

private:
	Vector4<T> m_T[4];


public:


};

template<class T>
Vector4<T> operator*(const Vector4<T>& lhs, const Matrix4x4<T>& rhs);

#include "Matrix4x4.hpp"

typedef Matrix4x4<float> Matrix4x4f;

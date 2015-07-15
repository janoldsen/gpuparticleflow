
#pragma once

#include "Vector3.h"


template<class T>
class Vector4;

typedef Vector4<float> Vector4f;


class Quaternion
{
public:

	// --CREATORS--

	Quaternion();
	Quaternion(const Vector4f& vs);
	Quaternion(const Vector3f& v, float angle);
	Quaternion(float s, const Vector3f& v);

	// --MANIPULATOR--

	Quaternion& conjugate();
	Quaternion& inverse();
	Quaternion& normalize();

	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion& operator*=(float rhs);
	Quaternion& operator/=(float rhs);

	// --ACCESSOR--

	float getNorm() const;
	float getNormSq() const;
	Quaternion getInverse() const;
	Quaternion getConjugate() const;
	Quaternion getNormalized() const;

	Vector3f rotate(const Vector3f& vector) const;

	const Vector3f& getV() const;
	float getS() const;

private:
	float m_S;
	Vector3f m_V;

	friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
};


Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator*(const Quaternion& lhs, float rhs);
Quaternion operator/(const Quaternion& lhs, float rhs);

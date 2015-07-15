#include "Quaternion.h"
#include "Vector4.h"

Quaternion::Quaternion()
	:m_S(cos(0.0f)), m_V(0.0f, 0.0f, sin(0))
{

}

Quaternion::Quaternion(const Vector4f& vs)
	: m_S(vs.w()), m_V(vs.x(), vs.y(), vs.z())
{
}


Quaternion::Quaternion(const Vector3f& v, float angle)
	: m_S(cos(0.5f*angle)), m_V(v.getNormalized() * sin(0.5f*angle))
{
}

Quaternion::Quaternion(float s, const Vector3f& v)
	: m_S(s), m_V(v)
{

}

Quaternion& Quaternion::conjugate()
{
	m_V *= -1;
	return *this;
}

Quaternion& Quaternion::inverse()
{
	if (this->getNormSq() == 0.0f)
		return *this;

	return this->conjugate() /= this->getNormSq();
}

Quaternion& Quaternion::normalize()
{
	return *this /= getNorm();
}


Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	*this = Quaternion(*this) * rhs;
	return *this;
}

Quaternion& Quaternion::operator*=(float rhs)
{
	m_S *= rhs;
	m_V *= rhs;
	return *this;
}

Quaternion& Quaternion::operator/=(float rhs)
{
	m_S /= rhs;
	m_V *= 1.0f / rhs;
	return *this;
}

float Quaternion::getNorm() const
{
	return sqrt(this->getNormSq());
}

float Quaternion::getNormSq() const
{
	return m_S * m_S + m_V.getMagnitudeSq();
}

Quaternion Quaternion::getInverse() const
{
	return Quaternion(*this).inverse();
}

Quaternion Quaternion::getConjugate() const
{
	return Quaternion(*this).conjugate();
}

Quaternion Quaternion::getNormalized() const
{
	return *this / getNorm();
}

const Vector3f& Quaternion::getV() const
{
	return m_V;
}

float Quaternion::getS() const
{
	return m_S;
}





Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion q;

	q.m_S = lhs.m_S * rhs.m_S - lhs.m_V * rhs.m_V;
	q.m_V = lhs.m_S * rhs.m_V + rhs.m_S * lhs.m_V + lhs.m_V.cross(rhs.m_V);

	return q;
}

Quaternion operator*(const Quaternion& lhs, float rhs)
{
	return Quaternion(lhs) *= rhs;
}

Quaternion operator/(const Quaternion& lhs, float rhs)
{
	return Quaternion(lhs) /= rhs;
}


Vector3f Quaternion::rotate(const Vector3f& vector) const
{
	return (*this * Quaternion(0, vector) * this->getInverse()).getV();
}

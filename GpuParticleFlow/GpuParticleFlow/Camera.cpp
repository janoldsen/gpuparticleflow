
#include "Camera.h"

Camera::Camera()
	: m_MatrixNeedUpdate(true),
	m_ViewNeedUpdate(true),
	m_Position(0, 0, 0),
	m_Rotation(Vector3f(0, 0, 1), 0.0f),
	m_Scale(1, 1, 1)
{
	m_Projection = Matrix4x4f::getProjection(90.0f * 3.14f / 180.0f, 1.3, 0.01f, 10000.0f);
}





Camera::~Camera()
{

}



void Camera::move(const Vector3f& offset)
{
	m_Position += offset;
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}

void Camera::moveLocal(const Vector3f& offset)
{
	m_Position += (m_Rotation * Quaternion(Vector4f(offset.x(), offset.y(), offset.z(), 0)) * m_Rotation.getInverse()).getV();
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}

void Camera::setPosition(const Vector3f& position)
{
	m_Position = position;
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}

void Camera::rotate(const Quaternion& offset)
{
	m_Rotation = offset * m_Rotation;
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}


void Camera::rotateLocal(const Quaternion& offset)
{
	Vector3f v = (m_Rotation * Quaternion(0, offset.getV()) * m_Rotation.getInverse()).getV();

	//Vector3f v = Vector4f(offset.getV(),1) * getTransformationMatrix();

	m_Rotation = Quaternion(offset.getS(), v) * m_Rotation;
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}

void Camera::setRotation(const Quaternion& rotation)
{
	m_Rotation = rotation;
	m_MatrixNeedUpdate = true;
	m_ViewNeedUpdate = true;
}

void Camera::scale(const Vector3f& offset)
{
	m_Scale += offset;
}

void Camera::setScale(const Vector3f& scale)
{
	m_Scale = scale;
}


const Matrix4x4f& Camera::getTransformationMatrix() const
{
	if (m_MatrixNeedUpdate)
	{
		m_Matrix = Matrix4x4f::getTransformation(m_Position, m_Rotation, m_Scale);
		m_MatrixNeedUpdate = false;
	}

	return m_Matrix;
}


const Matrix4x4f& Camera::getViewMatrix() const
{
	if (m_ViewNeedUpdate)
	{

		Vector3f look = (m_Rotation * Quaternion(Vector4f(0, 0, 1, 0)) * m_Rotation.getInverse()).getV();
		Vector3f up = (m_Rotation * Quaternion(Vector4f(0, 1, 0, 0)) * m_Rotation.getInverse()).getV();


		m_View = Matrix4x4f::getView(m_Position, look, up);
		m_ViewNeedUpdate = false;
	}

	return m_View;
}

const Matrix4x4f& Camera::getProjectionMatrix() const
{
	return m_Projection;
}

const Vector3f& Camera::getPosition() const
{
	return m_Position;
}

const Quaternion& Camera::getRotation() const
{
	return m_Rotation;
}

const Vector3f& Camera::getScale() const
{
	return m_Scale;
}
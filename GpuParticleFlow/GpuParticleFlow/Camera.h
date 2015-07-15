#pragma once

#include <vector>

#include "Vector3.h"
#include "Matrix4x4.h"

class Camera
{
public:


	Camera();
	
	~Camera();

	void move(const Vector3f& offset);
	void moveLocal(const Vector3f& offset);
	void setPosition(const Vector3f& position);

	void rotate(const Quaternion& offset);
	void rotateLocal(const Quaternion& offset);
	void setRotation(const Quaternion& rotation);

	void scale(const Vector3f& offset);
	void setScale(const Vector3f& scale);

	const Matrix4x4f& getTransformationMatrix() const;
	const Matrix4x4f& getViewMatrix() const;
	const Matrix4x4f& getProjectionMatrix() const;
	const Vector3f& getPosition() const;
	const Quaternion& getRotation() const;
	const Vector3f& getScale() const;


private:

	mutable Matrix4x4f m_View;
	Matrix4x4f m_Projection;


	Vector3f m_Position;
	Vector3f m_Scale;
	Quaternion m_Rotation;

	mutable bool m_MatrixNeedUpdate;
	mutable bool m_ViewNeedUpdate;

	mutable Matrix4x4f m_Matrix;
};


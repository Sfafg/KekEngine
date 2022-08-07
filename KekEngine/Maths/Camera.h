#pragma once
#include <glad\glad.h>
#include "Matrix.h"
#include "Vectors.h"
#include "Transform.h"
#include "../Misc.h"
class Camera
{
public:
	Transform transform;
	Matrix4x4 projectionMatrix;
	class Mat
	{
		void Set() {}
		void Get() {
			PTR(Camera, viewMatrix);
			if (ptr->transform.flagSet.GetBit(Transform::Flags::TransformChanged))
			{
				ptr->transform.matrix();
				ptr->viewMatrix = Matrix::lookAt(ptr->transform.position, ptr->transform.position + ptr->transform.forward, ptr->transform.up);
			}
		}
		GetSetMat(Matrix4x4);
		Mat() : v(Matrix4x4()) {}
	}viewMatrix;
	Camera(Transform transform) : transform(transform) {}

	Camera(Transform transform, Matrix4x4 projectionMatrix) : transform(transform), projectionMatrix(projectionMatrix)
	{
		viewMatrix();
	}

	Camera() {}
	void operator = (Camera const& obj) {
		transform = obj.transform; viewMatrix = obj.viewMatrix; projectionMatrix = obj.projectionMatrix;
	}
};
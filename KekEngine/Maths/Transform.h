#pragma once
#include "Matrix.h"
#include "Byte.h"

class Transform2D
{
	vec2f m_position;
	vec2f m_scale;
	float m_rotation;
	mat3x3 m_matrix;

	bool isMatrixUpdated;
	public:

	Transform2D(vec2f position = { 0,0 }, vec2f scale = { 1,1 }, float rotation = 0)
		: m_position(position), m_scale(scale), m_rotation(rotation), m_matrix(), isMatrixUpdated(true)
	{
		m_matrix.Transform(m_position, m_scale, m_rotation);
	}

	const vec2f& GetPosition() { return m_position; }
	void SetPosition(const vec2f& position) { m_position = position; isMatrixUpdated = false; }
	const vec2f& GetScale() { return m_scale; }
	void SetScale(const vec2f& scale) { m_scale = scale; isMatrixUpdated = false; }
	const float& GetRotation() { return m_rotation; }
	void SetRotation(const float& rotation) { m_rotation = rotation; isMatrixUpdated = false; }

	const mat3x3& GetMatrix()
	{
		if(!isMatrixUpdated)
		{
			m_matrix = mat3x3();
			m_matrix.Transform(m_position, m_scale, m_rotation);
			isMatrixUpdated = true;
		}
		return m_matrix;
	}

	vec2f Right()
	{
		return GetMatrix().TransformDir({ 1,0 });
	}
	vec2f Up()
	{
		return GetMatrix().TransformDir({ 0,1 });
	}
};


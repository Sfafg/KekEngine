#pragma once
#include <vector>
#include "Matrix.h"
#include "Vectors.h"
#include "../Debug.h"
#include "../Misc.h"
#include "Byte.h"

class Transform2D
{
public:
	struct Flags
	{
		static const int TransformChanged = 0, UP = 1, RIGHT = 2;
	};
	Byte flagSet;
	class Pos
	{
		void Set() { PTR(Transform2D, position); ptr->flagSet.SetBit(Flags::TransformChanged, 1); }
		void Get() {}
		GetSetVec(vec2f);
		Pos(vec2f a) : v(a) { Set(); }
	}position;
	class Sca
	{
		void Set() { PTR(Transform2D, scale); ptr->flagSet.SetBit(Flags::TransformChanged, 1); }
		void Get() {}
		GetSetVec(vec2f);
		Sca(vec2f a) : v(a) { Set(); }
	}scale;
	class Rot
	{
		void Set() {
			PTR(Transform2D, rotation);
			ptr->flagSet.SetBit(Flags::TransformChanged, 1);
			ptr->flagSet.SetBit(Flags::UP, 1);
			ptr->flagSet.SetBit(Flags::RIGHT, 1);
		}
		void Get() {}
		GetSetF;
		Rot(float a) : v(a) { Set(); }
	}rotation;

	class Mat
	{
		void UpdateMatrix(Transform2D* ptr)
		{
			ptr->matrix.v = Matrix3x3();
			ptr->matrix.v.transform(ptr->position, ptr->scale, ptr->rotation);
			ptr->flagSet.SetBit(Flags::TransformChanged, 0);
		}
		void Set() {}
		void Get() {
			PTR(Transform2D, matrix);
			if (ptr->flagSet.GetBit(Flags::TransformChanged))UpdateMatrix(ptr);
		}
		GetSetMat(Matrix3x3);
		Mat() : v(Matrix3x3()) { Set(); }
	}matrix;
	class Up
	{
		void Set() {}
		void Get() {
			PTR(Transform2D, up);
			if (!ptr->flagSet.GetBit(Flags::UP)) return;
			ptr->flagSet.SetBit(Flags::UP, false);
			*this = ptr->matrix().TransformDir(vec2f::up);
		}
		GetSetVec(vec2f);
		Up(vec2f a = vec2f::up) : v(a) { Set(); }
	}up;
	class Right
	{
		void Set() {}
		void Get() {
			PTR(Transform2D, right);
			if (!ptr->flagSet.GetBit(Flags::RIGHT)) return;
			ptr->flagSet.SetBit(Flags::RIGHT, false);
			*this = ptr->matrix().TransformDir(vec2f::right);
		}
		GetSetVec(vec2f);
		Right(vec2f a = vec2f::right) : v(a) { Set(); }
	}right;


	Transform2D(vec2f pos = vec2f(0, 0), vec2f scale = vec2f(1, 1), float rot = 0)
		: position(pos), scale(scale), rotation(rot) {}
	Matrix3x3 inverse() { Matrix3x3 t; t.inverseTransform(position, scale, rotation); return t; }

	void operator = (Transform2D const& o) { flagSet = o.flagSet; position = o.position; scale = o.scale; rotation = o.rotation; matrix = o.matrix; }
};
std::ostream& operator<<(std::ostream& os, const Transform2D& o)
{
	os << "Position: " << o.position << "\n";
	os << "Scale: " << o.scale << "\n";
	os << "Rotation: " << o.rotation << "\n";
	os << o.matrix;
	return os;
}

class Transform
{
protected:
public:
	struct Flags
	{
		static const int TransformChanged = 0, UP = 1, RIGHT = 2, FORWARD = 3;
	};
	Byte flagSet = 0;
	class Pos
	{
		void Set() { PTR(Transform, position); ptr->flagSet.SetBit(Flags::TransformChanged, 1); }
		void Get() {}
		GetSetVec(vec3f);
		Pos(vec3f a) : v(a) { Set(); }
	}position;
	class Sca
	{
		void Set() { PTR(Transform, scale); ptr->flagSet.SetBit(Flags::TransformChanged, 1); }
		void Get() {}
		GetSetVec(vec3f);
		Sca(vec3f a) : v(a) { Set(); }
	}scale;
	class Rot
	{
		void Set() {
			PTR(Transform, rotation);
			ptr->flagSet.SetBit(Flags::TransformChanged, 1);
			ptr->flagSet.SetBit(Flags::UP, 1);
			ptr->flagSet.SetBit(Flags::RIGHT, 1);
			ptr->flagSet.SetBit(Flags::FORWARD, 1);
		}
		void Get() {}
		GetSetVec(vec3f);
		Rot(vec3f a) : v(a) { Set(); }
	}rotation;

	class Mat
	{
		void UpdateMatrix(Transform* ptr)
		{
			ptr->matrix.v = Matrix4x4();
			ptr->matrix.v.transform(ptr->position, ptr->scale, ptr->rotation);
			ptr->flagSet.SetBit(Flags::TransformChanged, 0);
		}
		void Set() {}
		void Get() {
			PTR(Transform, matrix);
			if (ptr->flagSet.GetBit(Flags::TransformChanged))UpdateMatrix(ptr);
		}
		GetSetMat(Matrix4x4);
		Mat() : v(Matrix4x4()) { Set(); }
	}matrix;
	class Forward
	{
		void Set() {}
		void Get() {
			PTR(Transform, forward);
			if (!ptr->flagSet.GetBit(Flags::FORWARD)) return;
			ptr->flagSet.SetBit(Flags::FORWARD, 0);
			*this = ptr->matrix().TransformDir(vec3f::forward);
		}
		GetSetVec(vec3f);
		Forward(vec3f a = vec3f::forward) : v(a) { Set(); }
	}forward;
	class Up
	{
		void Set() {}
		void Get() {
			PTR(Transform, up);
			if (!ptr->flagSet.GetBit(Flags::UP)) return;
			ptr->flagSet.SetBit(Flags::UP, 0);
			*this = ptr->matrix().TransformDir(vec3f::up);
		}
		GetSetVec(vec3f);
		Up(vec3f a = vec3f::up) : v(a) { Set(); }
	}up;
	class Right
	{
		void Set() {}
		void Get() {
			PTR(Transform, right);
			if (!ptr->flagSet.GetBit(Flags::RIGHT)) return;
			ptr->flagSet.SetBit(Flags::RIGHT, 0);
			*this = ptr->matrix().TransformDir(vec3f::right);
		}
		GetSetVec(vec3f);
		Right(vec3f a = vec3f::right) : v(a) { Set(); }
	}right;
public:
	Transform(vec3f pos = vec3f(0, 0, 0), vec3f scale = vec3f(1, 1, 1), vec3f rot = vec3f(0, 0, 0)) : position(pos), scale(scale), rotation(rot)
	{}

	Matrix4x4 inverse() { Matrix4x4 t; t.inverseTransform(position, scale, rotation); return t; }

	void operator = (Transform const& o) {
		position = o.position; scale = o.scale; rotation = o.rotation; matrix = o.matrix; flagSet = o.flagSet; forward = o.forward; right = o.right; up = o.up;
	}
};
std::ostream& operator<<(std::ostream& os, const Transform& o)
{
	os << "Position: " << o.position << "\n";
	os << "Scale: " << o.scale << "\n";
	os << "Rotation: " << o.rotation << "\n";
	os << "Forward: " << o.forward << "\n";
	os << "Up: " << o.up << "\n";
	os << "Right: " << o.right << "\n";
	os << o.matrix << "\n";
	os << "FlagSet: " << o.flagSet;
	return os;
}
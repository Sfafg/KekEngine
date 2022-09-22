#pragma once
#include <iosfwd>
#include "Transform.h"

class Camera
{
	public:
	Transform3D transform;
	mat4x4 projection;
};


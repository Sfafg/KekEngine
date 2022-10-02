#pragma once

namespace Kek
{
	namespace WindowContext
	{
		extern bool Init(int majorVersion, int minorVersion);
		extern void Terminate();
	}
	namespace GraphicsContext
	{
		extern bool Init();
	}
}
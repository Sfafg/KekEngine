#pragma once

namespace Kek
{
	namespace SystemContext
	{
		extern bool Init();
		extern void Terminate();
	}
	namespace GraphicsContext
	{
		extern bool Init(int majorVersion, int minorVersion);
	}
}
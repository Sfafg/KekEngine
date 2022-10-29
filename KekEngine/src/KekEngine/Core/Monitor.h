#pragma once
#include "KekEngine/Maths/Vector.h"

namespace Kek
{

	class Monitor;
	namespace Monitors
	{
		Monitor* GetAll();
		Monitor* GetAll(int& size);
		Monitor Get(int index);
	};

	class Monitor
	{
		void* monitor;

		public:
		Monitor(void* monitor = NULL);
		Monitor(int index);

		Monitor(const Monitor& o);

		operator void* ();

		const char* Name() const;
		vec2i Size() const;
		vec2i PhisicalSize() const;
		vec2i Position() const;
		int RefreshRate() const;
	};
}
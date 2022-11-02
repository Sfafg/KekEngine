#include "KekEngine/Core/Log.h"
#include "KekEngine/Maths/Byte.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/SystemIO/SystemIO.h"
/// LOOP WHEN MAPPING A TO B AND B TO A.
/// ADD INPUT CAPTURING TO EVENTS.

using namespace Kek;

void Logger(int key, int state, int code, FlagSet modulators)
{
	if(key <= Mouse_Last)
	{
		Log<Warning>(SystemIO::mousePos, " ", SystemIO::mouseDelta);
	}
	else Log<Warning>(KeyName(key), " ", StateName(state), " ");
}
void Loggerwh(vec2i delta, FlagSet modulators)
{
	Log<Warning>(delta);
}

Application app;
void Kek::Setup()
{
	SystemIO::Init();

	//SystemIO::MapKey(Event_Scroll_Up, Key_Space);
	SystemIO::keyEvent += Logger;
	SystemIO::scrollWheelEvent += Loggerwh;
}
void Kek::Update()
{ 
	if(SystemIO::Key(Key_Escape) == State_Click) app.Close();
	SystemIO::PollEvents();
}
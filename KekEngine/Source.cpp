#include "KekEngine/Core/Log.h"
#include "KekEngine/Maths/Byte.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/SystemIO/SystemIO.h"

#include "KekEngine/Core/Timing.h"
using namespace Kek;

bool isMapped = false;
void MapKey(int key, int state, int code, FlagSet mods)
{
	if(state == State_Release || !mods.IsUp(Mod_Shift)) return;
	if(key == Key_Escape)
	{
		Application::Close();
	}
	if(key == Key_Enter)
	{
		if(isMapped)
		{
			SystemIO::MapKey(Event_Scroll_Up, Event_Scroll_Up);
			SystemIO::MapKey(Event_Scroll_Down, Event_Scroll_Down);
			SystemIO::MapKey(Key_Space, Key_Space);
			SystemIO::MapKey(Mouse_Middle, Mouse_Middle);
			SystemIO::MapKey(Mouse_4, Mouse_4);
			SystemIO::MapKey(Mouse_5, Mouse_5);
		}
		else
		{
			SystemIO::MapKey(Event_Scroll_Up, Key_Comma);
			SystemIO::MapKey(Event_Scroll_Down, Key_Period);
			SystemIO::MapKey(Key_Space, Key_Backspace);
			SystemIO::MapKey(Mouse_Middle, Key_6);
			SystemIO::MapKey(Mouse_4, Key_7);
			SystemIO::MapKey(Mouse_5, Key_5);
		}
		isMapped = !isMapped;
	}
}

void Time(int key, int state, int code, FlagSet mods)
{
	if(state == State_Release) return;

	if(key == Key_S)
	{
		Log<Info>("Took [{LCyan}]<", Timing::GetTiming() * Microseconds, Microseconds, "> to simulate input.");
	}

}
void Time2(vec2i delta, FlagSet mods)
{
	Timing::RestartTiming();
	SystemIO::SetKey({ Key_S, State_Click });
}
Application app;
void Kek::Setup()
{
	SystemIO::Init();
	//SystemIO::MapKey(Event_Scroll_Down, Key_S);
	//SystemIO::keyEventAsync += MapKey;
	SystemIO::keyEventAsync += Time;
	SystemIO::scrollWheelEventAsync += Time2;
}

void Kek::Update()
{
	SystemIO::PollEvents();
}
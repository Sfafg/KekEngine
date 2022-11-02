#include "SystemIO.h"
#include <map>
#include <vector>
#include <mutex>
#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "PlatformIO.h"

#define DEBUGISINIT(initialized); if(!initialized)Log<Error>("SystemIO is UnInitialized.")

namespace Kek
{
	std::map<int, int> keyMap;
	int keyStateAsync[Key_Last + 1];
	int keyState[Key_Last + 1];

	void SetModulators(int key, int state, FlagSet& mods)
	{
		switch(key)
		{
		case Key_Left_Shift: case Key_Right_Shift:
			(state == State_Press) ? mods.SetUp(Mod_Shift) : mods.SetDown(Mod_Shift); break;
		case Key_Left_Control: case Key_Right_Control:
			(state == State_Press) ? mods.SetUp(Mod_Control) : mods.SetDown(Mod_Control); break;
		case Key_Left_Alt: case Key_Right_Alt:
			(state == Mod_Alt) ? mods.SetUp(Mod_Alt) : mods.SetDown(Mod_Alt); break;
		case Key_Left_Super: case Key_Right_Super:
			(state == State_Press) ? mods.SetUp(Mod_Super) : mods.SetDown(Mod_Super); break;
		case Key_Caps_Lock:
			if(state == State_Press) mods ^= Mod_Caps; break;
		case Key_Num_Lock:
			if(state == State_Press) mods ^= Mod_Num; break;
		default: break;
		}
	}

	enum class EventType :char
	{
		None = 0,
		Key,
		Scroll,
		Mouse
	};
	struct KeyEventData
	{
		int key;
		int state;
		int code;
	};
	struct ScrollEventData
	{
		vec2i delta;
	};
	struct MouseEventData
	{
		vec2i pos;
	};

	struct EventData
	{
		EventType type = EventType::None;
		union
		{
			KeyEventData key;
			ScrollEventData scroll;
			MouseEventData mouse;
		};

		EventData(EventType type, KeyEventData data) : type(type)
		{
			key = data;
		}
		EventData(EventType type, ScrollEventData data) : type(type)
		{
			scroll = data;
		}
		EventData(EventType type, MouseEventData data) : type(type)
		{
			mouse = data;
		}
	};

	std::vector<EventData> eventBuffer;
	std::mutex eventBufferMutex;

	namespace SystemIO
	{
		Event<int, int, int, FlagSet> keyEventAsync;
		Event<vec2i, FlagSet> scrollWheelEventAsync;
		Event<vec2i, FlagSet> mouseMoveEventAsync;
		FlagSet modulatorsAsync = 0;
		vec2i mousePosAsync;

		Event<int, int, int, FlagSet> keyEvent;
		Event<vec2i, FlagSet> scrollWheelEvent;
		Event<vec2i, FlagSet> mouseMoveEvent;
		FlagSet modulators = 0;
		vec2i scrollDelta;
		vec2i mousePos;
		vec2i lastMousePos;
		vec2i mouseDelta;
	}
	namespace SystemIO
	{
		bool KeyCallback(int key, int state, int code, bool simulated)
		{
			if(simulated) Log<Info>("SIMULATED");

			// Key mapping.
			auto search = keyMap.find(key);
			if(search != keyMap.end())
			{
				int key = search->second;
				SetKey({ search->second,state });
				return false;
			}

			// Async events.
			SetModulators(key, state, modulatorsAsync);
			if(state == State_Press)
			{
				keyStateAsync[key] = (keyStateAsync[key] == State_Release) ? State_Click : State_Press;
			}
			else keyStateAsync[key] = State_Release;
			keyEventAsync(key, keyStateAsync[key], code, modulatorsAsync);

			// Event Buffering.
			eventBufferMutex.lock();
			eventBuffer.push_back({ EventType::Key, KeyEventData(key,state,code) });
			eventBufferMutex.unlock();
			return true;
		}
		bool ScrollCallback(vec2i delta, bool simulated)
		{
			// Event mapping.
			if(delta.x > 0)
			{
				auto search = keyMap.find(Event_Scroll_Right);
				if(search != keyMap.end())
				{
					SetKey({ search->second,1 });
					return false;
				}
			}
			if(delta.x < 0)
			{
				auto search = keyMap.find(Event_Scroll_Left);
				if(search != keyMap.end())
				{
					SetKey({ search->second,1 });
					return false;
				}
			}
			if(delta.y > 0)
			{
				auto search = keyMap.find(Event_Scroll_Up);
				if(search != keyMap.end())
				{
					SetKey({ search->second,1 });
					return false;
				}
			}
			if(delta.y < 0)
			{
				auto search = keyMap.find(Event_Scroll_Down);
				if(search != keyMap.end())
				{
					SetKey({ search->second,1 });
					return false;
				}
			}

			scrollWheelEventAsync(delta, modulatorsAsync);

			// Event Buffering.
			eventBufferMutex.lock();
			eventBuffer.push_back(EventData{ EventType::Scroll,ScrollEventData(delta) });
			eventBufferMutex.unlock();
			return true;
		}
		bool MouseMoveCallback(vec2i pos, bool simulated)
		{
			// Event mapping.
			vec2i delta = pos - mousePosAsync;
			mousePosAsync = pos;

			if(delta.x > 0)
			{
				auto search = keyMap.find(Event_Mouse_Move_Right);
				if(search != keyMap.end())
				{
					SetKey({ search->second,1 });
					delta.x = 0;
					SetMousePos(mousePosAsync + delta);
					return false;
				}
			}

			mouseMoveEventAsync(pos, modulatorsAsync);

			// Event Buffering.
			eventBufferMutex.lock();
			eventBuffer.push_back(EventData{ EventType::Mouse,MouseEventData(pos) });
			eventBufferMutex.unlock();
			return true;
		}

		void PollEvents()
		{
			std::vector<EventData> events;
			eventBufferMutex.lock();
			std::swap(events, eventBuffer);
			eventBufferMutex.unlock();

			for(int i = 0; i < Key_Last + 1; i++)
			{
				if(keyState[i] == State_Click) keyState[i] = State_Press;
			}
			scrollDelta = vec2i(0, 0);
			lastMousePos = mousePos;

			for(EventData e : events)
			{
				switch(e.type)
				{
				case EventType::Key:
				{
					if(e.key.state == State_Press)
					{
						keyState[e.key.key] = (keyState[e.key.key] == State_Release) ? State_Click : State_Press;
					}
					else keyState[e.key.key] = State_Release;
					SetModulators(e.key.key, e.key.state, modulators);
					keyEvent(e.key.key, keyState[e.key.key], e.key.code, modulators);
					break;
				}
				case EventType::Scroll:
				{
					scrollDelta = e.scroll.delta;
					scrollWheelEvent(e.scroll.delta, modulators);
					break;
				}
				case EventType::Mouse:
				{
					mousePos = e.mouse.pos;
					mouseDelta = lastMousePos - mousePos;
					mouseMoveEvent(e.mouse.pos, modulators);
					break;
				}
				default:break;
				}
			}
		}

		bool initialized = false;
		void Init()
		{
#ifndef NDEBUG
			if(initialized){ Log<Error>("You must NOT initialize SystemIO twice."); return; }
#endif
			PlatformIO::Init();

			PlatformIO::keyCallback = KeyCallback;
			PlatformIO::scrollWheelCallback = ScrollCallback;
			PlatformIO::mouseMoveCallback = MouseMoveCallback;

			initialized = true;
		}

		int Key(int index)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			return keyState[index];
		}

		void SetMousePos(vec2i pos)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			PlatformIO::SetMousePos(pos);
		}
		void SetMouseDelta(vec2i delta)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			PlatformIO::SetMouseDelta(delta);
		}
		void SetScrollDelta(vec2i scrollDelta)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			PlatformIO::SetScrollDelta(scrollDelta);
		}
		void SetKey(KeyData key)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			if(key.index == Key_None) return;
			if(key.index <= Key_Last) PlatformIO::SetKey(key);
			else
			{
				switch(key.index)
				{
				case Event_Mouse_Move_Left: SetMouseDelta(vec2i(-key.state, 0)); break;
				case Event_Mouse_Move_Right: SetMouseDelta(vec2i(key.state, 0)); break;
				case Event_Mouse_Move_Up: SetMouseDelta(vec2i(0, key.state)); break;
				case Event_Mouse_Move_Down: SetMouseDelta(vec2i(0, -key.state)); break;

				case Event_Scroll_Left: SetScrollDelta(vec2i(-120 * key.state, 0)); break;
				case Event_Scroll_Right: SetScrollDelta(vec2i(120 * key.state, 0)); break;
				case Event_Scroll_Up: SetScrollDelta(vec2i(0, 120 * key.state)); break;
				case Event_Scroll_Down: SetScrollDelta(vec2i(0, -120 * key.state)); break;
				default:break;
				}
			}
		}
		void MapKey(int keyA, int keyB)
		{
#ifndef NDEBUG
			DEBUGISINIT(initialized);
#endif
			if(keyA == keyB)
			{
				keyMap.erase(keyA);
				return;
			}
			keyMap[keyA] = keyB;
		}
	}
}
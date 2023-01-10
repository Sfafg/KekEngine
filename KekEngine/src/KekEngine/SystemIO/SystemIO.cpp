#include "SystemIO.h"
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "PlatformIO.h"

#define DEBUGISINIT(initialized); if(!initialized)Log<ERROR>("SystemIO is UnInitialized.")

namespace Kek
{
	std::map<int, int> keyMap;
	int keyStateAsync[KEY_LAST + 1];
	int keyState[KEY_LAST + 1];

	void SetModulators(int key, int state, bool simulated, FlagSet& mods)
	{
		switch(key)
		{
		case KEY_LEFT_SHIFT:   case KEY_RIGHT_SHIFT:   (state == STATE_PRESS) ? mods.SetUp(MOD_SHIFT) : mods.SetDown(MOD_SHIFT); break;
		case KEY_LEFT_CONTROL: case KEY_RIGHT_CONTROL: (state == STATE_PRESS) ? mods.SetUp(MOD_CONTROL) : mods.SetDown(MOD_CONTROL); break;
		case KEY_LEFT_ALT:     case KEY_RIGHT_ALT:     (state == STATE_PRESS) ? mods.SetUp(MOD_ALT) : mods.SetDown(MOD_ALT); break;
		case KEY_LEFT_SUPER:   case KEY_RIGHT_SUPER:   (state == STATE_PRESS) ? mods.SetUp(MOD_SUPER) : mods.SetDown(MOD_SUPER); break;
		case KEY_CAPS_LOCK: if(state == STATE_PRESS) mods ^= MOD_CAPS; break;
		case KEY_NUM_LOCK: if(state == STATE_PRESS) mods ^= MOD_NUM; break;
		default: break;
		}
		(simulated) ? mods.SetUp(MOD_SIMULATED) : mods.SetDown(MOD_SIMULATED);
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
		vec2i delta;
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
		FlagSet modulators;

		EventData(EventType type, KeyEventData data, FlagSet modulators) : type(type), modulators(modulators)
		{
			key = data;
		}
		EventData(EventType type, ScrollEventData data, FlagSet modulators) : type(type), modulators(modulators)
		{
			scroll = data;
		}
		EventData(EventType type, MouseEventData data, FlagSet modulators) : type(type), modulators(modulators)
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
		Event<vec2i, vec2i, FlagSet> mouseMoveEventAsync;
		FlagSet modulatorsAsync = 0;
		vec2i mousePosAsync;

		Event<int, int, int, FlagSet> keyEvent;
		Event<vec2i, FlagSet> scrollWheelEvent;
		Event<vec2i, vec2i, FlagSet> mouseMoveEvent;
		FlagSet modulators = 0;
		vec2i scrollDelta;
		vec2i mousePos;
		vec2i lastMousePos;
		vec2i mouseDelta;
	}

	bool PlatformIO::KeyCallback(int key, int state, int code, bool simulated)
	{
		// Key mapping.
		if(!simulated)
		{
			auto search = keyMap.find(key);
			if(search != keyMap.end())
			{
				if(key > MOUSE_LAST && key <= KEY_LAST || search->second > MOUSE_LAST && search->second <= KEY_LAST) SetKey({ search->second, state });
				else
				{
					std::thread th([state]()
						{
							SetKey({ MOUSE_LEFT, state });
						});
					th.detach();
				}
				return false;
			}
		}

		// Async events.
		SetModulators(key, state, simulated, SystemIO::modulatorsAsync);
		if(state == STATE_PRESS)
		{
			keyStateAsync[key] = (keyStateAsync[key] == STATE_RELEASE) ? STATE_CLICK : STATE_PRESS;
		}
		else keyStateAsync[key] = STATE_RELEASE;
		SystemIO::keyEventAsync(key, keyStateAsync[key], code, SystemIO::modulatorsAsync);

		// Event Buffering.
		eventBufferMutex.lock();
		eventBuffer.push_back({ EventType::Key, KeyEventData(key,state,code), SystemIO::modulatorsAsync });
		eventBufferMutex.unlock();
		return true;
	}
	bool PlatformIO::ScrollCallback(vec2i delta, bool simulated)
	{
		// Event mapping.
		if(!simulated)
		{
			std::map<int, int>::iterator search;
			if(delta.x > 0)search = keyMap.find(EVENT_SCROLL_RIGHT);
			else if(delta.x < 0)search = keyMap.find(EVENT_SCROLL_LEFT);

			if(delta.y > 0) search = keyMap.find(EVENT_SCROLL_UP);
			else if(delta.y < 0)search = keyMap.find(EVENT_SCROLL_DOWN);

			if(search != keyMap.end())
			{
				if(search->second > MOUSE_LAST && search->second <= KEY_LAST) SetKey({ search->second, STATE_CLICK });
				else
				{
					std::thread th([search]()
						{
							SetKey({ search->second, STATE_CLICK });
						});
					th.detach();
				}
				return false;
			}
		}

		SystemIO::scrollWheelEventAsync(delta, SystemIO::modulatorsAsync);

		// Event Buffering.
		eventBufferMutex.lock();
		eventBuffer.push_back(EventData{ EventType::Scroll,ScrollEventData(delta), SystemIO::modulatorsAsync });
		eventBufferMutex.unlock();
		return true;
	}
	bool PlatformIO::MouseMoveCallback(vec2i pos, bool simulated)
	{
		// Event mapping.
		vec2i delta = pos - SystemIO::mousePosAsync;
		SystemIO::mousePosAsync = pos;

		if(!simulated)
		{
			auto search = keyMap.find(EVENT_MOUSE_MOVE);
			if(search != keyMap.end())
			{
				if(search->second > MOUSE_LAST && search->second <= KEY_LAST) SetKey({ search->second, STATE_CLICK });
				else
				{
					std::thread th([search]()
						{
							SetKey({ search->second, STATE_CLICK });
						});
					th.detach();
				}
				return false;
			}
		}

		SystemIO::mouseMoveEventAsync(pos, delta, SystemIO::modulatorsAsync);

		// Event Buffering.
		eventBufferMutex.lock();
		eventBuffer.push_back(EventData{ EventType::Mouse,MouseEventData(pos,delta), SystemIO::modulatorsAsync });
		eventBufferMutex.unlock();
		return true;
	}

	namespace SystemIO
	{
		void PollEvents()
		{
			std::vector<EventData> events;
			eventBufferMutex.lock();
			std::swap(events, eventBuffer);
			eventBufferMutex.unlock();

			for(int i = 0; i < KEY_LAST + 1; i++)
			{
				if(keyState[i] == STATE_CLICK) keyState[i] = STATE_PRESS;
			}
			scrollDelta = vec2i(0, 0);
			lastMousePos = mousePos;

			for(EventData e : events)
			{
				switch(e.type)
				{
				case EventType::Key:
				{
					if(e.key.state == STATE_PRESS)
					{
						keyState[e.key.key] = (keyState[e.key.key] == STATE_RELEASE) ? STATE_CLICK : STATE_PRESS;
					}
					else keyState[e.key.key] = STATE_RELEASE;
					keyEvent(e.key.key, keyState[e.key.key], e.key.code, e.modulators);
					break;
				}
				case EventType::Scroll:
				{
					scrollDelta = e.scroll.delta;
					scrollWheelEvent(e.scroll.delta, e.modulators);
					break;
				}
				case EventType::Mouse:
				{
					mousePos = e.mouse.pos;
					mouseDelta = lastMousePos - mousePos;
					mouseMoveEvent(e.mouse.pos, e.mouse.delta, e.modulators);
					break;
				}
				default:break;
				}
			}
			modulators = events.end()[-1].modulators;
		}

		bool initialized = false;
		void Init()
		{
#ifndef NDEBUG
			if(initialized){ Log<ERROR>("You must NOT initialize SystemIO twice."); return; }
#endif
			PlatformIO::Init();

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
			if(key.index == KEY_NONE) return;
			if(key.index <= KEY_LAST) PlatformIO::SetKey(key);
			else
			{
				switch(key.index)
				{
				case EVENT_MOUSE_MOVE: SetMouseDelta(vec2i(1, 0)); break;

				case EVENT_SCROLL_LEFT: SetScrollDelta(vec2i(-120, 0)); break;
				case EVENT_SCROLL_RIGHT: SetScrollDelta(vec2i(120, 0)); break;
				case EVENT_SCROLL_UP: SetScrollDelta(vec2i(0, 120)); break;
				case EVENT_SCROLL_DOWN: SetScrollDelta(vec2i(0, -120)); break;
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
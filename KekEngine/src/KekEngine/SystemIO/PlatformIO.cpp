#include "PlatformIO.h"
#include <Windows.h>
#include <map>
#include <thread>
#include "KekEngine/Core/Log.h"

namespace Kek
{
	namespace PlatformIO
	{
		INPUT KeyToInput(int key, bool down)
		{
			INPUT input;
			ZeroMemory(&input, sizeof(input));

			input.type = INPUT_KEYBOARD;
			input.ki.wVk = key;
			if(!down)
			{
				input.ki.dwFlags = KEYEVENTF_KEYUP;
			}
			return input;
		}
		INPUT ButtonToInput(int button, bool down)
		{
			INPUT input;
			ZeroMemory(&input, sizeof(input));

			input.type = INPUT_MOUSE;
			switch(button)
			{
			case VK_LBUTTON:
				input.mi.dwFlags = (down) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
				break;
			case VK_RBUTTON:
				input.mi.dwFlags = (down) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
				break;
			case VK_MBUTTON:
				input.mi.dwFlags = (down) ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
				break;
			case VK_XBUTTON1:
			case VK_XBUTTON2:
				input.mi.dwFlags = (down) ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
				input.mi.mouseData = button;
				break;
			default:
				break;
			}
			return input;
		}

		std::map<int, int> keyMap;

		Event<char, char, int>& KeyEvent()
		{
			static Event<char, char, int> event;
			return event;
		};
		Event<vec2i>& MouseMoveEvent()
		{
			static Event<vec2i> event;
			return event;
		}
		Event<vec2i>& MouseScrollEvent()
		{
			static Event<vec2i> event;
			return event;
		}

		void MapKey(int keyA, int keyB)
		{
			if(keyA == keyB)
			{
				keyMap.erase(keyA);
				return;
			}
			keyMap[keyA] = keyB;
		}
		void SetKey(KeyData key)
		{
			int windowsKey = KeyToWindows(key.index);
			if(windowsKey == -1)
			{
				Log<Warning>("Windows does not support ", key.index, " key.");
				return;
			}
			if(key.index > Mouse_Last)
			{
				if(key.state == State_Click)
				{
					INPUT inputs[2] = {};
					ZeroMemory(inputs, sizeof(inputs));

					inputs[0] = KeyToInput(windowsKey, true);
					inputs[1] = KeyToInput(windowsKey, false);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
				else
				{
					INPUT inputs[1] = {};
					ZeroMemory(inputs, sizeof(inputs));

					inputs[0] = KeyToInput(windowsKey, key.state == State_Press);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
			}
			else
			{
				if(key.state == State_Click)
				{
					INPUT inputs[2] = {};
					ZeroMemory(inputs, sizeof(inputs));

					inputs[0] = ButtonToInput(windowsKey, true);
					inputs[1] = ButtonToInput(windowsKey, false);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
				else
				{
					INPUT inputs[1] = {};
					ZeroMemory(inputs, sizeof(inputs));

					inputs[0] = ButtonToInput(windowsKey, key.state == State_Press);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
			}
		}
		void SetScrollDelta(vec2i scrollDelta)
		{
			INPUT inputs[1] = {};
			ZeroMemory(inputs, sizeof(inputs));

			POINT mouse;
			GetCursorPos(&mouse);

			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dx = mouse.x;
			inputs[0].mi.dy = mouse.y;
			inputs[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
			inputs[0].mi.mouseData = (DWORD)scrollDelta.y;

			UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		}
		void SetMousePos(vec2i pos)
		{
			INPUT inputs[1] = {};
			ZeroMemory(inputs, sizeof(inputs));

			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
			inputs[0].mi.dx = pos.x;
			inputs[0].mi.dy = pos.y;

			UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		}
		void SetMouseDelta(vec2i delta)
		{
			INPUT inputs[1] = {};
			ZeroMemory(inputs, sizeof(inputs));

			inputs[0].type = INPUT_MOUSE;
			inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
			inputs[0].mi.dx = delta.x;
			inputs[0].mi.dy = delta.y;

			UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		}

		class Hook
		{
			HHOOK hook;
			public:
			Hook():hook(NULL) {};
			Hook(HHOOK hook) :hook(hook)
			{
			}
			~Hook()
			{
				if(hook == NULL) return;
				UnhookWindowsHookEx(hook);
				Log<Info>("Unhooking.");
			}
			void operator=(const HHOOK& hook_) { hook = hook_; }
			operator HHOOK() { return hook; }
		};

		Hook keyboardHook;
		Hook mouseHook;

		LRESULT CALLBACK KEYHookCallback(int nCode, WPARAM downParam, LPARAM keyDataParam)
		{
			KBDLLHOOKSTRUCT* keyData = (KBDLLHOOKSTRUCT*)keyDataParam;
			bool isDown = downParam == WM_KEYDOWN || downParam == WM_SYSKEYDOWN;
			int key = WindowsToKey(keyData->vkCode);
			if(key == -1)return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);

			KeyEvent()(key, (isDown), keyData->scanCode);

			// Do the key mapping.
			if(nCode < 0) return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);

			auto keyMapped = keyMap.find(key);
			if(keyMapped == keyMap.end()) return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);
			if(keyMapped->second == -1) return 1;

			SetKey({ (char)keyMapped->second,char(isDown) });
			return 1;
		}
		LRESULT CALLBACK MOUSEHookCallback(int nCode, WPARAM wP, LPARAM lP)
		{
			MSLLHOOKSTRUCT* keyData = (MSLLHOOKSTRUCT*)lP;
			MouseMoveEvent()(vec2i(keyData->pt.x, keyData->pt.y));

			int key = -1;
			bool isDown = false;
			vec2i scrollD(0, 0);
			unsigned short uS = 0;
			switch(wP)
			{
			case WM_LBUTTONDOWN:
				isDown = true;
			case WM_LBUTTONUP:
				key = Mouse_Left;
			case WM_LBUTTONDBLCLK:
				break;

			case WM_RBUTTONDOWN:
				isDown = true;
			case WM_RBUTTONUP:
				key = Mouse_Right;
			case WM_RBUTTONDBLCLK:
				break;

			case WM_MBUTTONDOWN:
				isDown = true;
			case WM_MBUTTONUP:
				key = Mouse_Middle;
			case WM_MBUTTONDBLCLK:
				break;

			case WM_XBUTTONDOWN:
				isDown = true;
			case WM_XBUTTONUP:
				key = HIWORD(keyData->mouseData) + 2;
			case WM_XBUTTONDBLCLK:
				break;

			case WM_MOUSEWHEEL:
				uS = HIWORD(keyData->mouseData);
				scrollD.y = *(short*)(&uS);
			case WM_MOUSEHWHEEL:
				if(wP != WM_MOUSEWHEEL)
				{
					uS = HIWORD(keyData->mouseData);
					scrollD.x = *(short*)(&uS);
				}
				MouseScrollEvent()(scrollD);
				break;

			default:
				break;
			}

			if(key == -1)return CallNextHookEx(keyboardHook, nCode, wP, lP);

			KeyEvent()(key, char(isDown), key);

			// Do the key mapping.
			if(nCode < 0) return CallNextHookEx(keyboardHook, nCode, wP, lP);

			auto keyMapped = keyMap.find(key);
			if(keyMapped == keyMap.end()) return CallNextHookEx(keyboardHook, nCode, wP, lP);
			if(keyMapped->second == -1) return 1;

			SetKey({ (char)keyMapped->second,char(isDown) });
			return 1;
		}

		bool initialized = false;
		void Init()
		{
#ifndef NDEBUG
			if(initialized)
			{
				Log<Error>("You must NOT initialize PlatformIO more then once.");
			}
			initialized = true;
#endif // !NDEBUG

			std::thread thread([]()
				{
					HHOOK hook;
					if(!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, KEYHookCallback, NULL, NULL)))
						Log<Error>("Failed to install KeyboardHook hook.");
					else Log<Info>("Installed KeyboardHook hook.");
					keyboardHook = hook;

					if(!(hook = SetWindowsHookEx(WH_MOUSE_LL, MOUSEHookCallback, NULL, NULL)))
						Log<Error>("Failed to install MouseHook hook.");
					else Log<Info>("Installed MouseHook hook.");
					mouseHook = hook;

					MSG Msg;
					while(GetMessage(&Msg, NULL, 0, 0) > 0)
					{
						TranslateMessage(&Msg);
						DispatchMessage(&Msg);
					}
				});
			thread.detach();
		}
		
	}
}
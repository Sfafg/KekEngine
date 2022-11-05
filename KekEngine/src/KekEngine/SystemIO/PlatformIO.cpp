#include "PlatformIO.h"
#include "KekEngine/Core/Log.h"
#include <Windows.h>
#include <thread>

class Hook
{
	HHOOK hook;
	public:
	Hook() :hook(NULL) {};
	Hook(HHOOK hook) :hook(hook)
	{
	}
	~Hook()
	{
		if(hook == NULL) return;
		UnhookWindowsHookEx(hook);
		Kek::Log<Kek::Info>("Unhooking.");
	}
	Hook& operator=(const HHOOK& hook_) { hook = hook_; return *this; }
	operator HHOOK () { return hook; }
};
void KeyToInput(INPUT& input, int key, bool down)
{
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	if(!down)
	{
		input.ki.dwFlags = KEYEVENTF_KEYUP;
	}
}
void ButtonToInput(INPUT& input, int button, bool down)
{
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
}

namespace Kek
{
	namespace PlatformIO
	{
		Hook keyboardHook;
		Hook mouseHook;

		FunctionPointer<bool, int, int, int, bool> keyCallback;
		FunctionPointer<bool, vec2i, bool> mouseMoveCallback;
		FunctionPointer<bool, vec2i, bool> scrollWheelCallback;

		LRESULT CALLBACK KEYHookCallback(int nCode, WPARAM downParam, LPARAM keyDataParam)
		{
			if(nCode < 0) return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);

			KBDLLHOOKSTRUCT* keyData = (KBDLLHOOKSTRUCT*)keyDataParam;
			bool isDown = downParam == WM_KEYDOWN || downParam == WM_SYSKEYDOWN;
			int key = WindowsToKey(keyData->vkCode);

			if(!keyCallback(key, isDown, keyData->scanCode, keyData->flags & LLKHF_INJECTED)) return 1;
			return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);
		}
		LRESULT CALLBACK MOUSEHookCallback(int nCode, WPARAM wP, LPARAM lP)
		{
			if(nCode < 0) return CallNextHookEx(mouseHook, nCode, wP, lP);

			MSLLHOOKSTRUCT* keyData = (MSLLHOOKSTRUCT*)lP;

			switch(wP)
			{
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN:
				if(!keyCallback(Mouse_1, State_Press, Mouse_1, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_LBUTTONUP:
				if(!keyCallback(Mouse_1, State_Release, Mouse_1, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_RBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
				if(!keyCallback(Mouse_2, State_Press, Mouse_2, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_RBUTTONUP:
				if(!keyCallback(Mouse_2, State_Release, Mouse_2, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_MBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
				if(!keyCallback(Mouse_3, State_Press, Mouse_3, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_MBUTTONUP:
				if(!keyCallback(Mouse_3, State_Release, Mouse_3, keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_XBUTTONDBLCLK:
			case WM_XBUTTONDOWN:
				if(HIWORD(keyData->mouseData) == XBUTTON1)
				{
					if(!keyCallback(Mouse_4, State_Press, Mouse_4, keyData->flags & LLKHF_INJECTED)) return 1;
				}
				else
				{
					if(!keyCallback(Mouse_5, State_Press, Mouse_5, keyData->flags & LLKHF_INJECTED)) return 1;
				}
				break;

			case WM_XBUTTONUP:
				if(HIWORD(keyData->mouseData) == XBUTTON1)
				{
					if(!keyCallback(Mouse_4, State_Release, Mouse_4, keyData->flags & LLKHF_INJECTED)) return 1;
				}
				else
				{
					if(!keyCallback(Mouse_5, State_Release, Mouse_5, keyData->flags & LLKHF_INJECTED)) return 1;
				}
				break;

			case WM_MOUSEWHEEL:
				if(!scrollWheelCallback(vec2i(0, (short)HIWORD(keyData->mouseData)), keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_MOUSEHWHEEL:
				if(!scrollWheelCallback(vec2i((short)HIWORD(keyData->mouseData), 0), keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			case WM_MOUSEMOVE:
				if(!mouseMoveCallback(vec2i(keyData->pt.x, keyData->pt.y), keyData->flags & LLKHF_INJECTED)) return 1;
				break;

			default: break;
			}

			return CallNextHookEx(mouseHook, nCode, wP, lP);
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
					if(!(keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KEYHookCallback, NULL, NULL)))
						Log<Error>("Failed to install KeyboardHook hook.");
					else Log<Info>("Installed KeyboardHook hook.");

					if(!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MOUSEHookCallback, NULL, NULL)))
						Log<Error>("Failed to install MouseHook hook.");
					else Log<Info>("Installed MouseHook hook.");

					MSG Msg;
					while(GetMessage(&Msg, NULL, 0, 0))
					{
						TranslateMessage(&Msg);
						DispatchMessage(&Msg);
					}
				});
			thread.detach();
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
					INPUT inputs[2]{};
					ZeroMemory(inputs, sizeof(inputs));

					KeyToInput(inputs[0], windowsKey, true);
					inputs[0].ki.time = DWORD(time);
					KeyToInput(inputs[1], windowsKey, false);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
				else
				{
					INPUT inputs[1]{};
					ZeroMemory(inputs, sizeof(inputs));

					KeyToInput(inputs[0], windowsKey, key.state == State_Press);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
			}
			else
			{
				if(key.state == State_Click)
				{
					INPUT inputs[2]{};
					ZeroMemory(inputs, sizeof(inputs));

					ButtonToInput(inputs[0], windowsKey, true);
					ButtonToInput(inputs[1], windowsKey, false);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
				else
				{
					INPUT inputs[1]{};
					ZeroMemory(inputs, sizeof(inputs));

					ButtonToInput(inputs[0], windowsKey, key.state == State_Press);

					UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
				}
			}
		}
		void SetScrollDelta(vec2i scrollDelta)
		{
			INPUT inputs[1]{};
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
	}
}
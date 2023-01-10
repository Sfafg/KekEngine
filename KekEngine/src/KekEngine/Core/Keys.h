#pragma once
#include <iosfwd>
#undef MOD_SHIFT
#undef MOD_CONTROL
#undef MOD_ALT

namespace Kek
{
	struct KeyData
	{
		int index;
		int state;
		bool operator ==(const KeyData& o);
	};

	extern const char* StateName(int state);
	extern const char* KeyName(int key);
	extern const char* ModName(int mod);
	extern const char* EventName(int event);

#pragma region STATES
	const int STATE_NONE = -1;
	const int STATE_RELEASE = 0;
	const int STATE_PRESS = 1;
	const int STATE_REPEAT = 2;
	const int STATE_CLICK = 3;
	const int STATE_LAST = STATE_CLICK;
#pragma endregion
#pragma region KEYS
	const int KEY_NONE = STATE_NONE;
	const int MOUSE_1 = 0;
	const int MOUSE_2 = 1;
	const int MOUSE_3 = 2;
	const int MOUSE_4 = 3;
	const int MOUSE_5 = 4;
	const int MOUSE_6 = 5;
	const int MOUSE_7 = 6;
	const int MOUSE_8 = 7;
	const int MOUSE_LAST = MOUSE_8;
	const int MOUSE_LEFT = MOUSE_1;
	const int MOUSE_RIGHT = MOUSE_2;
	const int MOUSE_MIDDLE = MOUSE_3;
	const int KEY_BACKSPACE = 8;
	const int KEY_TAB = 9;
	const int KEY_ENTER = 10;
	const int KEY_PAGE_UP = 11;
	const int KEY_PAGE_DOWN = 12;
	const int KEY_HOME = 13;
	const int KEY_END = 14;
	const int KEY_CAPS_LOCK = 15;
	const int KEY_SCROLL_LOCK = 16;
	const int KEY_NUM_LOCK = 17;
	const int KEY_LEFT_SHIFT = 18;
	const int KEY_RIGHT_SHIFT = 19;
	const int KEY_LEFT_CONTROL = 20;
	const int KEY_RIGHT_CONTROL = 21;
	const int KEY_LEFT_ALT = 22;
	const int KEY_RIGHT_ALT = 23;
	const int KEY_LEFT_SUPER = 24;
	const int KEY_RIGHT_SUPER = 25;
	const int KEY_ESCAPE = 26;
	const int KEY_INSERT = 27;
	const int KEY_PRINT_SCREEN = 28;
	const int KEY_PAUSE = 29;
	const int KEY_BACKSLASH = 30;
	//1
	const int KEY_SPACE = 32;
	const int KEY_RIGHT = 33;
	const int KEY_LEFT = 34;
	const int KEY_DOWN = 35;
	const int KEY_UP = 36;
	const int KEY_LEFT_BRACKET = 37;
	const int KEY_RIGHT_BRACKET = 38;
	const int KEY_APOSTROPHE = 39;
	const int KEY_KP_ENTER = 40;
	const int KEY_KP_EQUAL = 41;
	const int KEY_GRAVE_ACCENT = 42;
	const int KEY_KP_ADD = 43;
	const int KEY_COMMA = 44;
	const int KEY_MINUS = 45;
	const int KEY_PERIOD = 46;
	const int KEY_SLASH = 47;
	const int KEY_0 = 48;
	const int KEY_1 = 49;
	const int KEY_2 = 50;
	const int KEY_3 = 51;
	const int KEY_4 = 52;
	const int KEY_5 = 53;
	const int KEY_6 = 54;
	const int KEY_7 = 55;
	const int KEY_8 = 56;
	const int KEY_9 = 57;
	const int KEY_MENU = 58;
	const int KEY_SEMICOLON = 59;
	const int KEY_KP_DECIMAL = 60;
	const int KEY_EQUAL = 61;
	const int KEY_KP_DIVIDE = 62;
	const int KEY_KP_MULTIPLY = 63;
	const int KEY_KP_SUBTRACT = 64;
	const int KEY_A = 65;
	const int KEY_B = 66;
	const int KEY_C = 67;
	const int KEY_D = 68;
	const int KEY_E = 69;
	const int KEY_F = 70;
	const int KEY_G = 71;
	const int KEY_H = 72;
	const int KEY_I = 73;
	const int KEY_J = 74;
	const int KEY_K = 75;
	const int KEY_L = 76;
	const int KEY_M = 77;
	const int KEY_N = 78;
	const int KEY_O = 79;
	const int KEY_P = 80;
	const int KEY_Q = 81;
	const int KEY_R = 82;
	const int KEY_S = 83;
	const int KEY_T = 84;
	const int KEY_U = 85;
	const int KEY_V = 86;
	const int KEY_W = 87;
	const int KEY_X = 88;
	const int KEY_Y = 89;
	const int KEY_Z = 90;
	const int KEY_KP_0 = 91;
	const int KEY_KP_1 = 92;
	const int KEY_KP_2 = 93;
	const int KEY_KP_3 = 94;
	const int KEY_KP_4 = 95;
	const int KEY_KP_5 = 96;
	const int KEY_KP_6 = 97;
	const int KEY_KP_7 = 98;
	const int KEY_KP_8 = 99;
	const int KEY_KP_9 = 100;
	const int KEY_F1 = 101;
	const int KEY_F2 = 102;
	const int KEY_F3 = 103;
	const int KEY_F4 = 104;
	const int KEY_F5 = 105;
	const int KEY_F6 = 106;
	const int KEY_F7 = 107;
	const int KEY_F8 = 108;
	const int KEY_F9 = 109;
	const int KEY_F10 = 110;
	const int KEY_F11 = 111;
	const int KEY_F12 = 112;
	const int KEY_F13 = 113;
	const int KEY_F14 = 114;
	const int KEY_F15 = 115;
	const int KEY_F16 = 116;
	const int KEY_F17 = 117;
	const int KEY_F18 = 118;
	const int KEY_F19 = 119;
	const int KEY_F20 = 120;
	const int KEY_F21 = 121;
	const int KEY_F22 = 122;
	const int KEY_F23 = 123;
	const int KEY_F24 = 124;
	const int KEY_DELETE = 125;
	const int KEY_LAST = KEY_DELETE;
#pragma endregion
#pragma region MODULATORS
	const int MOD_SHIFT = 1;
	const int MOD_CONTROL = 2;
	const int MOD_ALT = 4;
	const int MOD_SUPER = 8;
	const int MOD_CAPS = 16;
	const int MOD_NUM = 32;
	const int MOD_SIMULATED = 64;
#pragma endregion
#pragma region Events
	const int EVENT_UNKNOWN = STATE_NONE;
	const int EVENT_MOUSE_MOVE = 126;
	const int EVENT_SCROLL_LEFT = 127;
	const int EVENT_SCROLL_RIGHT = 128;
	const int EVENT_SCROLL_UP = 129;
	const int EVENT_SCROLL_DOWN = 130;
	const int EVENT_LAST = EVENT_SCROLL_DOWN;
#pragma endregion
}
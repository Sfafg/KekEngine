#pragma once
#include <iosfwd>

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
	const int State_None = -1;
	const int State_Release = 0;
	const int State_Press = 1;
	const int State_Repeat = 2;
	const int State_Click = 3;
	const int State_Last = State_Click;
#pragma endregion
#pragma region KEYS
	const int Key_None = State_None;
	const int Mouse_1 = 0;
	const int Mouse_2 = 1;
	const int Mouse_3 = 2;
	const int Mouse_4 = 3;
	const int Mouse_5 = 4;
	const int Mouse_6 = 5;
	const int Mouse_7 = 6;
	const int Mouse_8 = 7;
	const int Mouse_Last = Mouse_8;
	const int Mouse_Left = Mouse_1;
	const int Mouse_Right = Mouse_2;
	const int Mouse_Middle = Mouse_3;
	const int Key_Backspace = 8;
	const int Key_Tab = 9;
	const int Key_Enter = 10;
	const int Key_Page_Up = 11;
	const int Key_Page_Down = 12;
	const int Key_Home = 13;
	const int Key_End = 14;
	const int Key_Caps_Lock = 15;
	const int Key_Scroll_Lock = 16;
	const int Key_Num_Lock = 17;
	const int Key_Left_Shift = 18;
	const int Key_Right_Shift = 19;
	const int Key_Left_Control = 20;
	const int Key_Right_Control = 21;
	const int Key_Left_Alt = 22;
	const int Key_Right_Alt = 23;
	const int Key_Left_Super = 24;
	const int Key_Right_Super = 25;
	const int Key_Escape = 26;
	const int Key_Insert = 27;
	const int Key_Print_Screen = 28;
	const int Key_Pause = 29;
	const int Key_Backslash = 30;
	//1
	const int Key_Space = 32;
	const int Key_Right = 33;
	const int Key_Left = 34;
	const int Key_Down = 35;
	const int Key_Up = 36;
	const int Key_Left_Bracket = 37;
	const int Key_Right_Bracket = 38;
	const int Key_Apostrophe = 39;
	const int Key_KP_Enter = 40;
	const int Key_KP_Equal = 41;
	const int Key_Grave_Accent = 42;
	const int Key_KP_Add = 43;
	const int Key_Comma = 44;
	const int Key_Minus = 45;
	const int Key_Period = 46;
	const int Key_Slash = 47;
	const int Key_0 = 48;
	const int Key_1 = 49;
	const int Key_2 = 50;
	const int Key_3 = 51;
	const int Key_4 = 52;
	const int Key_5 = 53;
	const int Key_6 = 54;
	const int Key_7 = 55;
	const int Key_8 = 56;
	const int Key_9 = 57;
	const int Key_Menu = 58;
	const int Key_Semicolon = 59;
	const int Key_KP_Decimal = 60;
	const int Key_Equal = 61;
	const int Key_KP_Divide = 62;
	const int Key_KP_Multiply = 63;
	const int Key_KP_Subtract = 64;
	const int Key_A = 65;
	const int Key_B = 66;
	const int Key_C = 67;
	const int Key_D = 68;
	const int Key_E = 69;
	const int Key_F = 70;
	const int Key_G = 71;
	const int Key_H = 72;
	const int Key_I = 73;
	const int Key_J = 74;
	const int Key_K = 75;
	const int Key_L = 76;
	const int Key_M = 77;
	const int Key_N = 78;
	const int Key_O = 79;
	const int Key_P = 80;
	const int Key_Q = 81;
	const int Key_R = 82;
	const int Key_S = 83;
	const int Key_T = 84;
	const int Key_U = 85;
	const int Key_V = 86;
	const int Key_W = 87;
	const int Key_X = 88;
	const int Key_Y = 89;
	const int Key_Z = 90;
	const int Key_KP_0 = 91;
	const int Key_KP_1 = 92;
	const int Key_KP_2 = 93;
	const int Key_KP_3 = 94;
	const int Key_KP_4 = 95;
	const int Key_KP_5 = 96;
	const int Key_KP_6 = 97;
	const int Key_KP_7 = 98;
	const int Key_KP_8 = 99;
	const int Key_KP_9 = 100;
	const int Key_F1 = 101;
	const int Key_F2 = 102;
	const int Key_F3 = 103;
	const int Key_F4 = 104;
	const int Key_F5 = 105;
	const int Key_F6 = 106;
	const int Key_F7 = 107;
	const int Key_F8 = 108;
	const int Key_F9 = 109;
	const int Key_F10 = 110;
	const int Key_F11 = 111;
	const int Key_F12 = 112;
	const int Key_F13 = 113;
	const int Key_F14 = 114;
	const int Key_F15 = 115;
	const int Key_F16 = 116;
	const int Key_F17 = 117;
	const int Key_F18 = 118;
	const int Key_F19 = 119;
	const int Key_F20 = 120;
	const int Key_F21 = 121;
	const int Key_F22 = 122;
	const int Key_F23 = 123;
	const int Key_F24 = 124;
	const int Key_Delete = 125;
	const int Key_Last = Key_Delete;
#pragma endregion
#pragma region MODULATORS
	const int Mod_Shift = 1;
	const int Mod_Control = 2;
	const int Mod_Alt = 4;
	const int Mod_Super = 8;
	const int Mod_Caps = 16;
	const int Mod_Num = 32;
#pragma endregion
#pragma region Events
	const int Event_Unknown = State_None;
	const int Event_Mouse_Move = 126;
	const int Event_Scroll_Left = 127;
	const int Event_Scroll_Right = 128;
	const int Event_Scroll_Up = 129;
	const int Event_Scroll_Down = 130;
	const int Event_Last = Event_Scroll_Down;
#pragma endregion
}
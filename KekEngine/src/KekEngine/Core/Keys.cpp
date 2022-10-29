#include "Keys.h"
#include <iostream>
#include <map>

namespace Kek
{
	bool KeyData::operator ==(const KeyData& o)
	{
		return index == o.index && state == o.state;
	}

	const std::map<int, const char*> stateNameMap
	{
		{State_Release,"Release"},
		{State_Press,"Press"},
		{State_Repeat,"Repeat"},
		{State_Click,"Click"}
	};
	const std::map<int, const char*> keyNameMap
	{
		{Mouse_1,"Mouse_1"},
		{Mouse_2,"Mouse_2"},
		{Mouse_3,"Mouse_3"},
		{Mouse_4,"Mouse_4"},
		{Mouse_5,"Mouse_5"},
		{Mouse_6,"Mouse_6"},
		{Mouse_7,"Mouse_7"},
		{Mouse_8,"Mouse_8"},
		{Key_0,"0"},
		{Key_1,"1"},
		{Key_2,"2"},
		{Key_3,"3"},
		{Key_4,"4"},
		{Key_5,"5"},
		{Key_6,"6"},
		{Key_7,"7"},
		{Key_8,"8"},
		{Key_9,"9"},
		{Key_Space,"Space"},
		{Key_Apostrophe,"Apostrophe"},
		{Key_Comma,"Comma"},
		{Key_Minus,"Minus"},
		{Key_Period,"Period"},
		{Key_Slash,"Slash"},
		{Key_Semicolon,"Semicolon"},
		{Key_Equal,"Equal"},
		{Key_Left_Bracket,"Left_Bracket"},
		{Key_Backslash,"Backslash"},
		{Key_Right_Bracket,"Right_Bracket"},
		{Key_Grave_Accent,"Grave_Accent"},
		{Key_Escape,"Escape"},
		{Key_Enter,"Enter"},
		{Key_Tab,"Tab"},
		{Key_Backspace,"Backspace"},
		{Key_Insert,"Insert"},
		{Key_Delete,"Delete"},
		{Key_Right,"Right"},
		{Key_Left,"Left"},
		{Key_Down,"Down"},
		{Key_Up,"Up"},
		{Key_Page_Up,"Page_Up"},
		{Key_Page_Down,"Page_Down"},
		{Key_Home,"Home"},
		{Key_End,"End"},
		{Key_Caps_Lock,"Caps_Lock"},
		{Key_Scroll_Lock,"Scroll_Lock"},
		{Key_Num_Lock,"Num_Lock"},
		{Key_Print_Screen,"Print_Screen"},
		{Key_Pause,"Pause"},
		{Key_F1,"F1"},
		{Key_F2,"F2"},
		{Key_F3,"F3"},
		{Key_F4,"F4"},
		{Key_F5,"F5"},
		{Key_F6,"F6"},
		{Key_F7,"F7"},
		{Key_F8,"F8"},
		{Key_F9,"F9"},
		{Key_F10,"F10"},
		{Key_F11,"F11"},
		{Key_F12,"F12"},
		{Key_F13,"F13"},
		{Key_F14,"F14"},
		{Key_F15,"F15"},
		{Key_F16,"F16"},
		{Key_F17,"F17"},
		{Key_F18,"F18"},
		{Key_F19,"F19"},
		{Key_F20,"F20"},
		{Key_F21,"F21"},
		{Key_F22,"F22"},
		{Key_F23,"F23"},
		{Key_F24,"F24"},
		{Key_A,"A"},
		{Key_B,"B"},
		{Key_C,"C"},
		{Key_D,"D"},
		{Key_E,"E"},
		{Key_F,"F"},
		{Key_G,"G"},
		{Key_H,"H"},
		{Key_I,"I"},
		{Key_J,"J"},
		{Key_K,"K"},
		{Key_L,"L"},
		{Key_M,"M"},
		{Key_N,"N"},
		{Key_O,"O"},
		{Key_P,"P"},
		{Key_Q,"Q"},
		{Key_R,"R"},
		{Key_S,"S"},
		{Key_T,"T"},
		{Key_U,"U"},
		{Key_V,"V"},
		{Key_W,"W"},
		{Key_X,"X"},
		{Key_Y,"Y"},
		{Key_Z,"Z"},
		{Key_KP_0,"KP_0"},
		{Key_KP_1,"KP_1"},
		{Key_KP_2,"KP_2"},
		{Key_KP_3,"KP_3"},
		{Key_KP_4,"KP_4"},
		{Key_KP_5,"KP_5"},
		{Key_KP_6,"KP_6"},
		{Key_KP_7,"KP_7"},
		{Key_KP_8,"KP_8"},
		{Key_KP_9,"KP_9"},
		{Key_KP_Decimal,"KP_Decimal"},
		{Key_KP_Divide,"KP_Divide"},
		{Key_KP_Multiply,"KP_Multiply"},
		{Key_KP_Subtract,"KP_Subtract"},
		{Key_KP_Add,"KP_Add"},
		{Key_KP_Enter,"KP_Enter"},
		{Key_KP_Equal,"KP_Equal"},
		{Key_Left_Shift,"Left_Shift"},
		{Key_Left_Control,"Left_Control"},
		{Key_Left_Alt,"Left_Alt"},
		{Key_Left_Super,"Left_Super"},
		{Key_Right_Shift,"Right_Shift"},
		{Key_Right_Control,"Right_Control"},
		{Key_Right_Alt,"Right_Alt"},
		{Key_Right_Super,"Right_Super"},
		{Key_Menu,"Menu"},
	};

	const char* StateName(int state) { return stateNameMap.at(state); }
	const char* KeyName(int key) { return keyNameMap.at(key); }
}
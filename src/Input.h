#pragma once

#include "RenderNS.h"
#include <gbUtils/callback.h>
#include <gbPhysics/type.h>

#ifdef _MSC_VER

//ref: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
#define GB_RENDER_INPUT_MS_L 0x01 /*VK_LBUTTON*/
#define GB_RENDER_INPUT_MS_R 0x02 /*VK_RBUTTON*/
#define GB_RENDER_INPUT_KBD_ENTER 0x0D /*VK_RETURN*/
#define GB_RENDER_INPUT_KBD_SHIFT 0x10 /*VK_SHIFT*/
#define GB_RENDER_INPUT_KBD_CONTROL 0x11 /*VK_CONTROL*/

#define GB_RENDER_INPUT_KBD_A 0x41
#define GB_RENDER_INPUT_KBD_B 0x42
#define GB_RENDER_INPUT_KBD_C 0x43
#define GB_RENDER_INPUT_KBD_D 0x44
#define GB_RENDER_INPUT_KBD_E 0x45
#define GB_RENDER_INPUT_KBD_F 0x46
#define GB_RENDER_INPUT_KBD_G 0x47
#define GB_RENDER_INPUT_KBD_H 0x48
#define GB_RENDER_INPUT_KBD_I 0x49
#define GB_RENDER_INPUT_KBD_J 0x4A
#define GB_RENDER_INPUT_KBD_K 0x4B
#define GB_RENDER_INPUT_KBD_L 0x4C
#define GB_RENDER_INPUT_KBD_M 0x4D
#define GB_RENDER_INPUT_KBD_N 0x4E 

#define GB_RENDER_INPUT_KBD_R 0x52
#define GB_RENDER_INPUT_KBD_S 0x53
#define GB_RENDER_INPUT_KBD_T 0x54
#define GB_RENDER_INPUT_KBD_W 0x57

#endif

#define GB_RENDER_INPUT_MS_MOVE 0xFF

GB_RENDER_NS_BEGIN

class Input
{
	enum class MouseAction
	{
		LBTN = 1,
		RBTN , MOVE
	};

	GB_SINGLETON(Input);


public:
	
	void Process(const std::int32_t code, const bool down);
	gb::physics::vec2<long> GetCursorPosition() const;
private:
	GB_PROPERTY_R(private, CBs, gb::utils::callback<const std::int32_t, const bool>);
	GB_PROPERTY_R(private, ClickedCBs, gb::utils::callback<>);
	GB_PROPERTY_R(private, ClickDown, bool);

};

GB_RENDER_NS_END
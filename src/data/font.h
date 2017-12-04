#pragma once
#include "atlas.h"

GB_RENDER_DATA_NAMESPACE_BEGIN
//#pragma pack(push, 1)
struct glyph:public sprite
    {
	//unit: pixel
	uint8 advanceX;
	uint8 yDelta;
	uint16 width;
	uint16 height;
    };
//#pragma pack(pop)

class font:public _atlas<unsigned long>
{
    
};
GB_RENDER_DATA_NAMESPACE_END

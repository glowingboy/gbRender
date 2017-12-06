#pragma once
#include "atlas.h"

GB_RENDER_DATA_NAMESPACE_BEGIN
struct glyph:public sprite
    {
	//unit: pixel
	uint16 width;
	uint16 height;
	uint16 advanceX;
	uint16 yDelta;

	static size_t size;
    };

class font:public _atlas<unsigned long>
{
    
};
GB_RENDER_DATA_NAMESPACE_END

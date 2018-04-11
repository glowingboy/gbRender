#pragma once
#include "Atlas.h"

namespace gb
{
	namespace render
	{
		namespace file
		{
			class Font;
		}
	}
}

GB_RENDER_DATA_NS_BEGIN

struct Glyph:public Sprite
    {
	//unit: pixel
	// uint16 width;
	// uint16 height;
	gb::render::uint32 advanceX;
	gb::render::uint32 yDelta;
    };

class Font:public _atlas<gb::render::uint32, Glyph>
{
	friend class gb::render::file::Font;
public:

private:
	GB_PROPERTY_R(private, GlyphSize, gb::render::uint32);
};

GB_RENDER_DATA_NS_END

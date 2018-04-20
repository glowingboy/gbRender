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

struct Glyph:public Sprite //has one(code) less than file::glyph
{
	//unit: pixel
	// uint16 width;
	// uint16 height;
	std::uint32_t advanceX;
	std::int32_t yDelta;
};

class Font:public _atlas<std::uint32_t, Glyph>
{
	friend class gb::render::file::Font;
public:
	Font();
	template <typename T>
	Font(T && other):
		_atlas<gb::render::uint32, Glyph>(std::forward<T>(other)),
		_GlyphSize(other._GlyphSize)
	{}

private:
	GB_PROPERTY_R(private, GlyphSize, std::uint32_t);
};

GB_RENDER_DATA_NS_END

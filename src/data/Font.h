#pragma once
#include "Atlas.h"
#include <gbPhysics/type.h>

GB_RENDER_DATA_NS_BEGIN

struct glyph:public sprite
    {
	//unit: pixel
	// uint16 width;
	// uint16 height;
	uint32 advanceX;
	uint32 yDelta;
    };

class Font:public _atlas<uint32, glyph>
{
public:
    inline void SetGlyphSize(uint32 glyphSize)
	{
	    _glyphSize = glyphSize;
	}
    inline void InsertGlyph(const uint32 code, const glyph& gly)
	{
	    _mpSprites.insert(std::pair<uint32, glyph>(code, gly));
	}
    inline void SetTexture(gb::physics::array_2d<uint8>&& texture)
	{
	    _texture = std::move(texture);
	}

    const gb::physics::array_2d<uint8>& GetTexture()const { return _texture; }
private:
    uint32 _glyphSize;
	gb::physics::array_2d<uint8> _texture;
};

GB_RENDER_DATA_NS_END

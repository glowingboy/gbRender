#pragma once
#include "atlas.h"

GB_RENDER_DATA_NAMESPACE_BEGIN
#pragma pack(push, 1)
    class glyph:public sprite
    {
	uint8 advanceX;
	uint8 yDelta;
    };
#pragma pack(pop)

class font:public texture
{
public:
private:
    std::unordered_map<uint32, glyph*> _mpGlyphs;
};
GB_RENDER_DATA_NAMESPACE_END

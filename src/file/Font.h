#pragma once
#include "FileNS.h"
#include <gbUtils/common.h>
#include <gbPhysics/type.h>
#include <vector>
#include "../data/Font.h"
/*
  font file format

  | block                | sizes(in byte) |
  | identifier("gbFont") |              6 |
  | glyph size           |              4 |
  | glyph count          |              4 |
  | glyphs               |             NA |
  | texture width        |              4 |
  | texture height       |              4 |
  | texture              |             NA |

*/

GB_RENDER_FILE_NS_BEGIN;

#define GB_RENDER_FILE_FONT_IDENDIFIER "gbFont"
#define GB_RENDER_FILE_FONT_EXTENSION "gbFont"

/*
 *@brief, has one more member(code) than data::glyph,
 *has same memory layout as data::glyph after this additonal member.
 */
struct glyph
{
    uint32 code;
    //data::glyph
    float32 uv_b;
    float32 uv_l;
    float32 uv_t;
    float32 uv_r;
    uint32 width;
    uint32 height;
    uint32 advanceX;
    uint32 yDelta;
    static size_t size;
};

struct glyph_ex:public glyph
{
    glyph_ex(uint32 idx)
	{
	    code = idx;
	}
    glyph_ex(glyph_ex&& other):
	glyph(other),
	sdf(std::move(other.sdf))
	{}
    void operator=(glyph_ex&& other)
	{
	    glyph::operator=(other);
	    sdf = std::move(other.sdf);
	}
	    
	gb::physics::array_2d<uint8>& data()
	{
	    return sdf;
	}
	gb::physics::array_2d<uint8> sdf;
};


class Font
{
    GB_SINGLETON(Font);
public:
    void SerializeToFile(const uint32_t glyphSize,
			 const std::vector<glyph_ex>& glyphs,
			 const gb::physics::array_2d<uint8>& texture,
			 const char* filePath)const;

    data::Font* ParseFromFile(const char* filePath)const;
};

GB_RENDER_FILE_NS_END;

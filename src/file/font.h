#pragma once
#include "file.h"
#include <gbUtils/common.h>
#include <gbPhysics/algorithm.h>
#include <vector>
#include "../data/font.h"
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

GB_RENDER_FILE_NAMESPACE_BEGIN;

#define GB_RENDER_FILE_FONT_IDENDIFIER "gbFont"
#define GB_RENDER_FILE_FONT_EXTENSION "gbFont"
class font
{
    GB_SINGLETON(font);
public:
    void SerializeToFile(const uint32_t glyphSize,
			 const std::vector<data::glyph>& glyphs,
			 const array_2d<std::uint8_t> texture,
			 const char* filePath)const;

    
};

GB_RENDER_FILE_NAMESPACE_END;

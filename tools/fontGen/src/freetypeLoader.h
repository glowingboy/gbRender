#pragma once
#include <gbUtils/common.h>

#define GB_FREETYPE_RENDER_GLYPH_SIZE 4096

class freetypeLoader
{
    GB_SINGLETON(freetypeLoader);

public:
    int load2gbFont(const char* szSrcFontName, const char* szDstFontName);
};

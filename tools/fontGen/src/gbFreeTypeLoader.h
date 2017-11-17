#pragma once
#include "../../Common/ClassDefine.h"
#include "../../gbUtils/Log/gbLog.h"
#include "../../gbUtils/Math/gbMath.h"

extern "C"
{
#include "freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include "freetype/include/freetype/ftoutln.h"
  
}

#ifdef _DEBUG
#pragma comment(lib, "freetype_static_vc140_MDd.lib")
#else
#pragma comment(lib, "freetype_static_vc140_MD.lib")
#endif
//#ifdef _DEBUG
//#pragma comment(lib, "freetyped.lib")
//#pragma comment(lib, "gbUtils_d.lib")
//#else		
//#pragma comment(lib, "freetype.lib")
//#pragma comment(lib, "gbUtils.lib")
//#endif
#include "../../gbUtils/FontFile/gbFontFile.h"

#define Glyph_SIZE 4096

class gbFreeTypeLoader
{
	SingletonDeclare(gbFreeTypeLoader)
public:
	~gbFreeTypeLoader();

public:
	bool Load(const char* fontName, const char* gbFontName);

private:
	static void _spans_callback(int y, int count, const FT_Span* spans, void* user);
	static int _FT_26dot6ToInt(const FT_Pos val);
	static FT_Pos _FT_IntTo26dot6(const int val);
};


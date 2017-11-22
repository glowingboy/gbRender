#include "freetypeLoader.h"
extern "C"
{
#include <ft2build.h>
#include <freetype/ftoutln.h>
}

#include <gbUtils/string.h>
#include <gbPhysics/algorithm.h>
#include <gbUtils/logger.h>

#include "../../../src/data/font.h"

using gb::render::data::font;
using gb::render::data::glyph;

using gb::utils::string;
using gb::algorithm::bit_vector;
using gb::utils::logger;
static int _gb_ft_26dot6ToInt(const FT_Pos val)
{
    return val >> 6;
}

static int _gb_ft_IntTo26dot6(const int val)
{
    return val << 6;
}

struct _gb_ft_raster_param_usrdata
{
    bit_vector data;
    FT_BBox bbox;
    unsigned int width;
    unsigned int height;
};

static void _gb_ft_spans_callback(int y, int count, const FT_Span* spans, void* userData)
{
    assert(userData != nullptr);
    _gb_ft_raster_param_usrdata* ud = reinterpret_cast<_gb_ft_raster_param_usrdata*>(userData);

    FT_BBox& bbox = ud->bbox;

    const unsigned int offset = (ud->height - (y - bbox.yMin) - 1)* ud->width;

    for(int i = 0; i < count; i++)
    {
	const FT_Span& span = spans[i];
	short x = span.x;
	x -= bbox.xMin;
	if( span.coverage > 127 )
	    ud->data.insert(offset + x, span.len, 1);
    }
}

int freetypeLoader::load2gbFont(const char* szSrcFontName, const char* szDstFontName)
{
    assert(szSrcFontName != nullptr && szDstFontName != nullptr);

    FT_Library ftLib;

    FT_Error ftErr = FT_Init_FreeType(&ftLib);
    if(ftErr != 0)
	throw string("FT_Init_FreeType error: ") + ftErr;

    FT_Face ftFace;

    ftErr = FT_New_Face(ftLib, szSrcFontName, 0, &ftFace);
    if(ftErr != 0)
    {
	string errInfo("FT_New_Face error: ");
	if(ftErr == FT_Err_Unknown_File_Format)
	    errInfo += "unknown format";
	else
	    errInfo += ftErr;
	throw errInfo;
    }

    FT_Set_Char_Size(ftFace, 0, _gb_ft_IntTo26dot6(GB_FREETYPE_RENDER_GLYPH_SIZE), 0, 0);
    FT_GlyphSlot ftSlot = ftFace->glyph;
    FT_Raster_Params rp;
    memset(&rp, 0, sizeof(FT_Raster_Params));
    rp.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
    rp.gray_spans = _gb_ft_spans_callback;

    _gb_ft_raster_param_usrdata ud;
    memset(&ud, 0, sizeof(_gb_ft_raster_param_usrdata));
    rp.user = &ud;
    FT_BBox& bbox = ud.bbox;
    

    std::vector<glyph*> glyphs;
    const unsigned int startCode = 0x5f;
    const unsigned int endCode = 65535;

    auto _gen_sdf_tex = [&](void* arg)
	{
	    
	};
    for(int i = startCode; i < endCode; i++)
    {
	FT_UInt charIdx = FT_Get_Char_Index(ftFace, i);
	if(charIdx != 0)
	{
	    ftErr = FT_Load_Glyph(ftFace, charIdx, FT_LOAD_NO_BITMAP);
	    assert(ftErr == 0);

	    FT_Outline_Get_CBox(&(ftSlot->outline), &bbox);
	    logger::Instance().log(string("charIdx @") + i);
	}
	else
	    logger::Instance().warning(string("charIdx @") + i);	    
    }
    return 0;
}

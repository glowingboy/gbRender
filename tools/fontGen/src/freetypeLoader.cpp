#include "freetypeLoader.h"
extern "C"
{
#include <ft2build.h>
#include <freetype/ftoutln.h>
}

#include <gbUtils/string.h>
using gb::utils::string;
using gb::algorithm::bit_vector;

static int _gb_ft_26dot6ToInt(const FT_Pos val)
{
    return val >> 6;
}

static int _gb_ft_IntTo26dot6(const int val)
{
    return val << 6;
}

struct gb_ft_raster_param_usrdata
{
    bit_vector data;
    FT_BBox bbox;
    unsigned int width;
    unsigned int height;
};

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
    
    return 0;
}

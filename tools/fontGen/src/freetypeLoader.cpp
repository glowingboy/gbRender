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
#include <gbUtils/concurrency.h>

#include <gbUtils/time.h>
#include <gbPhysics/image.h>

using gb::render::data::font;
using gb::render::data::glyph;

using gb::utils::string;
using namespace gb::algorithm;
using gb::utils::logger;
using gb::utils::concurrency;

using namespace gb::image;

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

    const uint32 offset = (ud->height - (y - bbox.yMin) - 1)* ud->width;

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


    concurrency<FT_ULong>::Instance().initialize();

    const uint8 threadCount = concurrency<FT_ULong>::Instance().get_threadscount();
	

    //thread variable
    struct th_va_t
    {
	~th_va_t()
	    {
		FT_Done_Face(ftFace);
		FT_Done_FreeType(ftLib);
	    }
	FT_Library ftLib;
	FT_Face ftFace;
	FT_Raster_Params rp;
	_gb_ft_raster_param_usrdata ud;
    };

    th_va_t* th_vas = new th_va_t[threadCount];

    for(int i = 0 ; i < threadCount; i++)
    {
	FT_Face& ftFace = th_vas[i].ftFace;
	FT_Library& ftLib = th_vas[i].ftLib;

	FT_Error ftErr = FT_Init_FreeType(&ftLib);
	if(ftErr != 0)
	    throw string("FT_Init_FreeType error: ") + ftErr;

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
	FT_Raster_Params& rp = th_vas[i].rp;
	memset(&rp, 0, sizeof(FT_Raster_Params));
	rp.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	rp.gray_spans = _gb_ft_spans_callback;

	_gb_ft_raster_param_usrdata& ud = th_vas[i].ud;
	memset(&ud, 0, sizeof(_gb_ft_raster_param_usrdata));
	rp.user = &ud;
    }

    struct glyph_ex:public glyph
    {
	glyph_ex(FT_ULong idx):
	    code(idx)
	    {}
	glyph_ex(glyph_ex&& other):
	    sdf(std::move(other.sdf))
	    {}
	array_2d<std::uint8_t> data()
	    {
		return sdf;
	    }
	
	FT_ULong code;
	array_2d<std::uint8_t> sdf;
    };
    std::vector<_glyph_ex> glyphs;
    
//    std::vector<glyph> glyphs;
    // std::vector<array_2d<std::uint8_t>> sdfs;
    std::mutex mtx;

    const FT_ULong startCode = 0x5f;
    const FT_ULong endCode = 0x6f;//65535;
    const FT_ULong totalCode = endCode - startCode;
    auto gen_sdf = [&glyphs, &sdfs, &mtx, &th_vas, totalCode](const uint8 threadCount, const size_t taskCount, FT_ULong idx)
    	{
    	    th_va_t& th_va = th_vas[threadCount];
    	    FT_Face& ftFace = th_va.ftFace;
    	    FT_Library& ftLib = th_va.ftLib;
	    
    	    FT_Raster_Params& rp = th_va.rp;
    	    _gb_ft_raster_param_usrdata& ud = th_va.ud;
    	    FT_BBox& bbox = ud.bbox;
    	    FT_GlyphSlot& ftSlot = ftFace->glyph;
	    
    	    FT_UInt charIdx = FT_Get_Char_Index(ftFace, idx);
    	    FT_Error ftErr;
    	    if(charIdx != 0)
    	    {
    		ftErr = FT_Load_Glyph(ftFace, charIdx, FT_LOAD_NO_BITMAP);
    		assert(ftErr == 0);

    		FT_Outline_Get_CBox(&(ftSlot->outline), &bbox);

    		glyph_ex gly(idx);

    		gly.advanceX = _gb_ft_26dot6ToInt(ftSlot->advance.x) / GB_FREETYPE_SAMPLESCALE;

    		bbox.xMax = _gb_ft_26dot6ToInt(bbox.xMax + 63);
    		bbox.yMax = _gb_ft_26dot6ToInt(bbox.yMax + 63);

    		bbox.xMin = _gb_ft_26dot6ToInt(bbox.xMin);
    		bbox.yMin = _gb_ft_26dot6ToInt(bbox.yMin);

    		ud.width = bbox.xMax - bbox.xMin;
		if(ud.width == 0)
		    return;
    		ud.height = bbox.yMax - bbox.yMin;

    		ud.data.reserve(ud.width * ud.height);

    		ftErr = FT_Outline_Render(ftLib, &(ftSlot->outline), &rp);
    		assert(ftErr == 0);

		gly.sdf = signed_distance_field(ud.data, ud.width, ud.height, GB_FREETYPE_SAMPLESCALE);

		gly.width = sdf.width;
		gly.height = sdf.height;
		gly.yDelta = gly.height - (_gb_ft_26dot6ToInt(ftSlot->metrics.horiBearingY / GB_FREETYPE_SAMPLESCALE));
		
    		{
    		    std::lock_guard<std::mutex> lck(mtx);
    		    glyphs.push_back(std::move(gly));
//		    sdfs.push_back(std::move(sdf));
		    
		    logger::Instance().progress(((float)(totalCode - taskCount + 1))/totalCode, string("glyph@") + idx);
    		}

    	    }
    	};


    for(FT_ULong i = startCode; i < endCode; i++)
    {
	concurrency<FT_ULong>::Instance().pushtask(concurrency::task_t(gen_sdf, i));
    }
    concurrency<FT_ULong>::Instance().done();
    logger::Instance().progress_done();

    if(th_vas != nullptr)
    {
	delete [] th_vas;
	th_vas = nullptr;
    }
	    
    logger::Instance().log("sdfs generating completed");

    //packing to one big array_2d
    array_2d<std::uint8_t> bin = packing<glyph_ex, std::uint8_t>(glyphs);
    return 0;
}

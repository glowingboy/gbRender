#include "gbFreeTypeLoader.h"
#include "../../gbUtils/FileIO/gbFile.h"
#include "../../gbUtils/Memory/gbBinaryData.h"
#include "../SignedDistanceFieldGen/gbSDF.h"
//SingletonDefine(gbFreeTypeLoader)

struct gbRaster_Param_UserData
{
	gbBinaryData data;
	FT_BBox bbox;
	unsigned int width;
	unsigned int height;
	//unsigned char* _cdata;
};

gbFreeTypeLoader::~gbFreeTypeLoader()
{
}

bool gbFreeTypeLoader::Load(const char * fontName, const char* gbFontName)
{
	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error != 0)
	{
		char buf[gb_LOG_MAX_LEN] = { '\0' };
		sprintf(buf, "FT_Init_FreeType_error:%d", error);
		gbLog::Instance().Error(buf);
		return false;
	}
	FT_Face face;

	//get current working path
	//	char wPath[gb_FILE_MAX_PATH] = {'\0'};
	gbString wPath;
	gbFileSystem::Instance().GetWorkPath(wPath);
		
	error = FT_New_Face(library, wPath + fontName, 0, &face);
	if (error != 0)
	{
		char buf[gb_LOG_MAX_LEN] = { '\0' };
		if (error == FT_Err_Unknown_File_Format)
			sprintf(buf, "FT_New_Face: the font file could be opened and read, but it appears that its font format is unsupported_%s", fontName);
		else
			sprintf(buf, "FT_New_Face:%d_%s", error, fontName);
		gbLog::Instance().Error(buf);
		return false;
	}

	FT_Set_Char_Size(face, 0, gbFreeTypeLoader::_FT_IntTo26dot6(Glyph_SIZE), 0, 0);
	//FT_Set_Pixel_Sizes(face, Glyph_SIZE, Glyph_SIZE);
	FT_GlyphSlot slot = face->glyph;
	FT_Raster_Params rp;
	memset(&rp, 0, sizeof(FT_Raster_Params));
	rp.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	rp.gray_spans = _spans_callback;

	std::vector<gbTextureData*> textures;
	std::vector<gbGlyph*> glyphs;
	gbRaster_Param_UserData ud;
	memset(&ud, 0, sizeof(gbRaster_Param_UserData));
	rp.user = &ud;
	FT_BBox& bbox = ud.bbox;
	//unsigned char** bufData = new unsigned char*[characters.size()];
	unsigned int dataIndex = 0;
	unsigned int startCode = 0x5f;
	unsigned int endCode = 65535;
	//wchar_t testCode[] = L"°¡ÊÀ½ç£¬ÄãºÃ£¡@glowingboy";
	// startCode = '-';
	// endCode = startCode + 10;
	for (int i = startCode; i < endCode; i++)
	{
		FT_UInt index = FT_Get_Char_Index(face, i);
		if (index == 0)
		{
			gbLog::Instance().Error("FT_Get_Char_Index");
			continue;
		}
		error = FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);
		if (error != 0)
		{
			gbLog::Instance().Error("FT_Load_Glyph");
			continue;
		}
		gbGlyph* glyph = new gbGlyph(i);
		FT_Outline_Get_CBox(&(slot->outline), &bbox);
		glyph->advanceX = (gbFreeTypeLoader::_FT_26dot6ToInt(slot->advance.x))/SampleScale;
		//convert to pixel size

		//ud.width = gbMath::min(gbMath::ceilToInt((float)(bbox.xMax - bbox.xMin) / 64), Glyph_SIZE);//ceil to int
		//ud.height = gbMath::min(gbMath::ceilToInt((float)(bbox.yMax - bbox.yMin) / 64), Glyph_SIZE);

		bbox.xMax = gbFreeTypeLoader::_FT_26dot6ToInt(bbox.xMax + 63);
		bbox.yMax = gbFreeTypeLoader::_FT_26dot6ToInt(bbox.yMax + 63);

		bbox.xMin = gbFreeTypeLoader::_FT_26dot6ToInt(bbox.xMin);
		bbox.yMin = gbFreeTypeLoader::_FT_26dot6ToInt(bbox.yMin); 

		ud.width = bbox.xMax - bbox.xMin;
		ud.height = bbox.yMax - bbox.yMin;

		unsigned int dataSize = ud.width*ud.height;
		ud.data.Reserve(dataSize);
		//ud._cdata = new unsigned char[dataSize];
		//memset(ud._cdata, 0, dataSize);

		error = FT_Outline_Render(library, &(slot->outline), &rp);
		if (error != 0)
		{
			gbSAFE_DELETE(glyph);
			gbLog::Instance().Error("FT_Outline_Render");
			continue;
		}
		gbTextureData* sdf = new gbTextureData;
		if (!gbSDF::Instance().Gen(ud.data, ud.width, ud.height, *sdf))
		{
			gbSAFE_DELETE(glyph);
			gbSAFE_DELETE(sdf);
			continue;
		}

		//gbSDF::Instance().Gen(ud._cdata, ud.width, ud.height, *sdf);

		glyph->width = sdf->Width;
		glyph->height = sdf->Height;
		glyph->yDelta = glyph->height - (gbFreeTypeLoader::_FT_26dot6ToInt(slot->metrics.horiBearingY) / SampleScale);
		textures.push_back(sdf);
		glyphs.push_back(glyph);

		 //if (textures.size() >= 5)
		 //	break;

		char buf[16] = { 0 };
		float per = (float)i / 65535;
		sprintf(buf, "%.2f", per);
		std::string strLog("glyph completed at:");
		strLog += buf;
		memset(buf, 0, 16);
		sprintf(buf, "@%d", i);
		strLog += buf;
		gbLog::Instance().Log(strLog.c_str());
		//gbLog::Instance().Log(&(*i));
	}
	gbLog::Instance().Log("glyphs completed!!!");
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	std::vector<gbTextureUV*> uvs;
	gbTextureData bin;


	gbTextureData::Packing(textures, bin, uvs);


	for (int i = 0; i < uvs.size(); i++)
	{
		gbGlyph* const glyph = glyphs[i];
		gbTextureUV* const uv = uvs[i];
		glyph->uv_b = uv->uv_b;
		glyph->uv_l = uv->uv_l;
		glyph->uv_r = uv->uv_r;
		glyph->uv_t = uv->uv_t;
	}
	gbFontFile::Create(glyphs, &bin, wPath + gbFontName/*"../gbRender/Data/test"*/, Glyph_SIZE/SampleScale);
	gbLog::Instance().Log("font file create completed!!!");
	for (std::vector<gbTextureData*>::iterator i = textures.begin(); i != textures.end(); i++)
	{
		gbSAFE_DELETE((*i));
	}
	for (std::vector<gbGlyph*>::iterator i = glyphs.begin(); i != glyphs.end(); i++)
	{
		gbSAFE_DELETE((*i));
	}
	for (std::vector<gbTextureUV*>::iterator i = uvs.begin(); i != uvs.end(); i++)
	{
		gbSAFE_DELETE((*i));
	}
	return true;
}

void gbFreeTypeLoader::_spans_callback(int y, int count, const FT_Span * spans, void * user)
{
	gbRaster_Param_UserData* ud = reinterpret_cast<gbRaster_Param_UserData*>(user);
	FT_BBox& bbox = ud->bbox;
	unsigned int offset = (ud->height - (y - bbox.yMin) - 1)*ud->width;
	//unsigned char* base = ud->data + offset;
	for (int i = 0; i < count; i++)
	{
		FT_Span span = spans[i];
		//if (span.x + gbMath::ceilToInt(float(bbox.yMin) / 64) <= 0)
		//	continue;
		span.x -= bbox.xMin;
		if (span.coverage > 127)
		{
			ud->data.In(1, offset + span.x, span.len);
			//memset(ud->_cdata + offset + span.x, 255, span.len);
		}
	}
}

int gbFreeTypeLoader::_FT_26dot6ToInt(const FT_Pos val)
{
	return val >> 6;
}

FT_Pos gbFreeTypeLoader::_FT_IntTo26dot6(const int val)
{
	return val << 6;
}

#include "font.h"
#include <gbUtils/filesystem.h>
#include <gbUtils/file.h>
using gb::render::file::font;
using gb::render::file::glyph;

using namespace gb;
using gb::utils::filesystem;
using gb::utils::string;

size_t glyph::size = sizeof(glyph);

void font::SerializeToFile(const uint32_t glyphSize,
		     const std::vector<glyph_ex>& glyphs,
		     const array_2d<std::uint8_t>& texture,
		     const char* filePath)const
{
    assert( glyphSize != 0 && glyphs.size() !=0 && filePath != nullptr);

    string path = filesystem::Instance().get_absolute_path(filePath);
    path = path + "." + GB_RENDER_FILE_FONT_EXTENSION;
    utils::file fontFile(path, false);

    fontFile.write(GB_RENDER_FILE_FONT_IDENDIFIER, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
    
    fontFile.write((const char*)&glyphSize, 4);
    const uint32 count = glyphs.size();
    fontFile.write((const char*)&count, 4);

    std::for_each(glyphs.begin(), glyphs.end(), [&fontFile](const glyph_ex& gly)
		  {
		      fontFile.write((const char*)&gly, glyph::size);
		  });

    fontFile.write((const char*)&(texture.width), 4);
    fontFile.write((const char*)&(texture.height), 4);

    fontFile.write((const char*)texture.data(), texture.row * texture.col);
}

data::font font::ParseFromFile(const char* filePath)const
{
    assert(filePath != nullptr);

    utils::file fontFile(filesystem::Instance().get_absolute_path(filePath));

    data::font ret;
    char* identifier = new char[strlen(GB_RENDER_FILE_FONT_IDENDIFIER) + 1]{'\0'};
    fontFile.read(identifier, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
    if(string(identifier) != GB_RENDER_FILE_FONT_IDENDIFIER)
	throw string("font::ParseFromFile error identifier@") + identifier + " @filePath" + filePath;
    char buf4[4] = {0};
    //glyph size
    fontFile.read(buf4, 4);
    ret.SetGlyphSize(*(uint32*)buf4);
    //glyph count
    fontFile.read(buf4, 4);
    const uint32 count = *(uint32*)buf4;

    //glyphs
    char buf_gly[glyph::size] = {0};
    uint8 data_glyph_offset = sizeof(uint32);//glyph::code
    for(int i = 0; i < count; i++)
    {
	fontFile.read(buf_gly, glyph::size);
	data::glyph gly(*(data::glyph*)(buf_gly + data_glyph_offset));
	ret.InsertGlyph(*(uint32*)buf_gly, gly);
    }

    //texture
    fontFile.read(buf4, 4);
    const uint32 width = *(uint32*)buf4;

    fontFile.read(buf4, 4);
    const uint32 height = *(uint32*)buf4;

    char* tex_data = new char[width * height];
    fontFile.read(tex_data, width * height);
    array_2d<uint8> texture;
    texture.assign(height, width, (uint8*)tex_data);
    ret.SetTexture(std::move(texture));
    return ret;
}

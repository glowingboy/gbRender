#include "Font.h"
#include <gbUtils/filesystem.h>
#include <gbUtils/file.h>

using namespace gb::render::file;
using namespace gb::render;
using gb::utils::filesystem;
using gb::utils::string;

size_t glyph::size = sizeof(glyph);

void Font::SerializeToFile(const uint32_t glyphSize,
		     const std::vector<glyph_ex>& glyphs,
		     const gb::physics::array_2d<std::uint8_t>& texture,
		     const char* filePath)const
{
    assert( glyphSize != 0 && glyphs.size() !=0 && filePath != nullptr);

    string path = filesystem::Instance().get_absolute_path(filePath);
    path = path + "." + GB_RENDER_FILE_FONT_EXTENSION;
    utils::file fontFile(path, false);

    fontFile.write(GB_RENDER_FILE_FONT_IDENDIFIER, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
    
    fontFile.write(&glyphSize, 4);
    const uint32 count = glyphs.size();
    fontFile.write(&count, 4);

    std::for_each(glyphs.begin(), glyphs.end(), [&fontFile](const glyph_ex& gly)
		  {
		      fontFile.write((const char*)&gly, glyph::size);
		  });

    fontFile.write(&(texture.width), 4);
    fontFile.write(&(texture.height), 4);

    fontFile.write(texture.data(), texture.row * texture.col);
}

data::Font* Font::ParseFromFile(const char* filePath)const
{
    assert(filePath != nullptr);

    utils::file fontFile(filesystem::Instance().get_absolute_path(filePath));

    data::Font* ret = new data::Font;
    char* identifier = new char[strlen(GB_RENDER_FILE_FONT_IDENDIFIER) + 1]{'\0'};
    fontFile.read(identifier, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
    if(string(identifier) != GB_RENDER_FILE_FONT_IDENDIFIER)
	throw string("font::ParseFromFile error identifier@") + identifier + " @filePath" + filePath;
    uint32 buf_uint32 = 0;
    //glyph size
    fontFile.read(&buf_uint32, 4);
	ret->_GlyphSize = buf_uint32;
    //glyph count
    fontFile.read(&buf_uint32, 4);
    const uint32 count = buf_uint32;

    //glyphs
    glyph tmpGlyph;
    memset(&tmpGlyph, 0, sizeof(data::Glyph));
    uint8 data_glyph_offset = sizeof(uint32);//glyph::code
    for(uint32 i = 0; i < count; i++)
    {
	fontFile.read(&tmpGlyph, glyph::size);
	data::Glyph gly(*(data::Glyph*)((char*)&tmpGlyph + data_glyph_offset));
	ret->_mpSprites.insert(std::make_pair(tmpGlyph.code, gly));
    }

    //texture
    fontFile.read(&buf_uint32, 4);
    const uint32 width = buf_uint32;

    fontFile.read(&buf_uint32, 4);
    const uint32 height = buf_uint32;

    char* tex_data = new char[width * height];
    fontFile.read(tex_data, width * height);

	ret->data = tex_data;
	ret->width = width;
	ret->height = height;

    return ret;
}


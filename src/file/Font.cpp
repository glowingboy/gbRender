#include "Font.h"

#include "../data/Font.h"

using namespace gb::render::file;
using namespace gb::render;
using gb::utils::filesystem;
using gb::utils::string;

data::Font* Font::ParseFromFile(const char* filePath)const
{
	assert(filePath != nullptr);

	utils::file fontFile(filesystem::Instance().get_absolute_path(filePath));

	data::Font* ret = new data::Font;
	char* identifier = new char[strlen(GB_RENDER_FILE_FONT_IDENDIFIER) + 1]{ '\0' };
	fontFile.read(identifier, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
	if (string(identifier) != GB_RENDER_FILE_FONT_IDENDIFIER)
		throw string("font::ParseFromFile error identifier@") + identifier + " @filePath" + filePath;
	std::uint32_t buf_uint32 = 0;
	//glyph size
	fontFile.read(&buf_uint32, 4);
	ret->_GlyphSize = buf_uint32;
	//glyph count
	fontFile.read(&buf_uint32, 4);
	const std::uint32_t count = buf_uint32;

	//glyphs
	Glyph tmpGlyph;
	memset(&tmpGlyph, 0, sizeof(data::Glyph));
	static constexpr std::uint8_t data_glyph_offset = sizeof(std::uint32_t);//glyph::code
	for (std::uint32_t i = 0; i < count; i++)
	{
		fontFile.read(&tmpGlyph, sizeof(Glyph));
		data::Glyph gly(*(data::Glyph*)((char*)&tmpGlyph + data_glyph_offset));
		ret->_mpSprites.insert(std::make_pair(tmpGlyph.code, gly));
	}

	//texture
	fontFile.read(&buf_uint32, 4);
	const std::uint32_t width = buf_uint32;

	fontFile.read(&buf_uint32, 4);
	const std::uint32_t height = buf_uint32;

	char* tex_data = new char[width * height]{};
	fontFile.read(tex_data, width * height);

	GLTexture::ImageData imgData;
	imgData.data = tex_data;
	imgData.depth = 0;
	imgData.externalFormat = GL_RED;
	imgData.height = height;
	imgData.internalFormat = GL_R8;
	imgData.levels = 1;
	imgData.target = GL_TEXTURE_2D;
	imgData.type = GL_UNSIGNED_BYTE;
	imgData.width = width;
	imgData.row_unpack_alignment = 1;

	ret->SetData(imgData);

	GB_SAFE_DELETE_ARRAY(tex_data);

	return ret;
}


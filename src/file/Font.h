#pragma once
#include "FileNS.h"
#include <gbUtils/common.h>
#include <gbPhysics/type.h>
#include <gbUtils/filesystem.h>
#include <gbUtils/file.h>

#include <vector>

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


namespace gb
{
	namespace render
	{
		namespace data
		{
			class Font;
		}
	}
}

GB_RENDER_FILE_NS_BEGIN;

#define GB_RENDER_FILE_FONT_IDENDIFIER "gbFont"
#define GB_RENDER_FILE_FONT_EXTENSION "gbFont"

/*
 *@brief, has one more member(code) than data::glyph,
 *has same memory layout as data::glyph after this additonal member.
 */
struct Glyph
{
	std::uint32_t code;
	//data::glyph
	float uv_b;
	float uv_l;
	float uv_t;
	float uv_r;
	std::uint32_t width;
	std::uint32_t height;
	std::uint32_t advanceX;
	std::uint32_t yDelta;
};

struct Glyph_ex :public Glyph
{
	Glyph_ex(std::uint32_t idx)
	{
		code = idx;
	}
	Glyph_ex(Glyph_ex&& other) :
		Glyph(other),
		sdf(std::move(other.sdf))
	{}
	void operator=(Glyph_ex&& other)
	{
		Glyph::operator=(other);
		sdf = std::move(other.sdf);
	}

	gb::physics::array_2d<std::uint8_t>& data()
	{
		return sdf;
	}
	gb::physics::array_2d<std::uint8_t> sdf;
};


class Font
{
	GB_SINGLETON(Font);
public:
	inline void SerializeToFile(const std::uint32_t glyphSize,
		const std::vector<Glyph_ex>& glyphs,
		const gb::physics::array_2d<std::uint8_t>& texture,
		const char* filePath)const
	{
		assert(glyphSize != 0 && glyphs.size() != 0 && filePath != nullptr);

		gb::utils::string path = gb::utils::filesystem::Instance().get_absolute_path(filePath);
		path = path + "." + GB_RENDER_FILE_FONT_EXTENSION;
		gb::utils::file fontFile(path, false);

		fontFile.write(GB_RENDER_FILE_FONT_IDENDIFIER, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));

		fontFile.write(&glyphSize, 4);
		const std::uint32_t count = glyphs.size();
		fontFile.write(&count, 4);

		std::for_each(glyphs.begin(), glyphs.end(), [&fontFile](const Glyph_ex& gly)
		{
			fontFile.write((const char*)&gly, sizeof(Glyph));
		});

		fontFile.write(&(texture.width), 4);
		fontFile.write(&(texture.height), 4);

		fontFile.write(texture.data(), texture.row * texture.col);
	}

	data::Font* ParseFromFile(const char* filePath)const;
};

GB_RENDER_FILE_NS_END;

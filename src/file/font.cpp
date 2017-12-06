#include "font.h"
#include <gbUtils/filesystem.h>
#include <gbUtils/file.h>
using gb::render::file::font;
using gb::utils::file;
using gb::utils::filesystem;
using gb::utils::string;
void font::SerializeToFile(const uint32_t glyphSize,
		     const std::vector<data::glyph>& glyphs,
		     const array_2d<std::uint8_t> texture,
		     const char* filePath)const;
{
    assert( glyphSize != 0 && glyphs.size() !=0 && filePath != nullptr);

    string path = get_absolute_path(filePath);
    path = path + "." + GB_RENDER_FILE_FONT_EXTENSION;
    file(filePath);

    file.write(GB_RENDER_FILE_FONT_IDENDIFIER, strlen(GB_RENDER_FILE_FONT_IDENDIFIER));
    
    file.write((const char*)&glyphSize, 4);
    file.write((const char*)&(glyphs.size()), 4);

    std::for_each(glyphs.begin(), glyphs.end(), [&file](data::glyph& gly)
		  {
		      file.write((const char*)&gly, data::glyph::size);
		  });

    file.write((const char*)&(texture.width), 4);
    file.write((const char*)&(texture.height), 4);

    file.write((const char*)texture.data(), texture.row * texture.col);
}

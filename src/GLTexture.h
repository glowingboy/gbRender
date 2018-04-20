#pragma once
#include <gbUtils/common.h>
#include "GL.h"

GB_RENDER_NS_BEGIN

class GLTexture
{
public:
	struct ImageData
	{
		ImageData();
		GLenum target;
		GLsizei levels;
		GLenum internalFormat;
		GLenum externalFormat;
		GLenum type;
		std::uint32_t width;
		std::uint32_t height;
		std::uint32_t depth;
		void* data;

		GLint row_unpack_alignment;
	};
	GLTexture();
	GLTexture(const GLTexture && o) = delete;
	~GLTexture();
public:
	void SetData(const ImageData& data);

protected:
	GB_PROPERTY_R(protected, Target, GLenum);
	GB_PROPERTY_R(protected, TextureObj, GLuint);
};

GB_RENDER_NS_END

#pragma once
#include "DataNS.h"
#include "../Type.h"
#include <gbUtils/common.h>
#include "../GL.h"

GB_RENDER_DATA_NS_BEGIN

class Texture
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
	};
	Texture();
	Texture(const Texture && o) = delete;
	~Texture();
public:
	void SetData(const ImageData& data);
	//type

protected:
	GB_PROPERTY_R(protected, Target, GLenum);
	GB_PROPERTY_R(protected, TextureObj, GLuint);
};

GB_RENDER_DATA_NS_END

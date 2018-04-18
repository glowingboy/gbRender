#include "Texture.h"

using namespace gb::render::data;

Texture::ImageData::ImageData():
	target(0),
	levels(0),
	internalFormat(0),
	externalFormat(0),
	type(0),
	width(0),
	height(0),
	data(nullptr)
{
}

Texture::Texture():
	_TextureObj(0)
{}

Texture::~Texture()
{
	glDeleteBuffers(1, &_TextureObj);
}


void Texture::SetData(const ImageData & data)
{
	if (data.target == 0)
		return;

	_Target = data.target;

	if (_TextureObj != 0)
		glDeleteBuffers(1, &_TextureObj);

	_TextureObj = 0;

	glGenTextures(1, &_TextureObj);

	glBindTexture(_Target, _TextureObj);

	if (_Target == GL_TEXTURE_2D)
	{
		glTexStorage2D(_Target, data.levels, data.internalFormat, data.width, data.height);
		if(data.data != nullptr)
			glTexSubImage2D(_Target, data.levels, 0, 0, data.width, data.height, data.externalFormat, data.type, data.data);
	}
	else if (_Target == GL_TEXTURE_2D_ARRAY)
	{
		glTexStorage3D(_Target, data.levels, data.internalFormat, data.width, data.height, data.depth);
		if(data.data != nullptr)
			glTexSubImage3D(_Target, data.levels, 0, 0, 0, data.width, data.height, data.depth, data.externalFormat, data.type, data.data);
	}


	glTexParameteri(_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(_Target, 0);

}

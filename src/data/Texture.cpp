#include "Texture.h"

using namespace gb::render::data;


Texture::Texture():
	width(0),
	height(0),
	data(0)
{

}
Texture::Texture(Texture && other):
	width(other.width),
	height(other.height),
	data(other.data)
{
}

Texture::~Texture()
{
	delete[] data;
	data = nullptr;
}

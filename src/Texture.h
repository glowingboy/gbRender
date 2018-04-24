#pragma once

#include "Render.h"
#include "GLTexture.h"

#define GB_RENDER_TEXTURE_DEFAULT_SIDE 5.0f

GB_RENDER_NS_BEGIN

class Texture : public BaseRender
{
public:
	Texture(Entity* owner);

public:
	void SetTexture(const char* szTexture);

private:
	GLTexture* _texture;
	GLVar* _vtxPos;
	GLVar* _vtxUV;
};

GB_RENDER_NS_END
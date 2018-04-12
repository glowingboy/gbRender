#pragma once
#include "DataNS.h"
#include "../Type.h"
#include <gbUtils/common.h>

GB_RENDER_DATA_NS_BEGIN

struct Texture
{
	Texture();
	Texture(Texture && other);
	~Texture();
    uint32 width;
    uint32 height;
    void* data;
	//type
};

GB_RENDER_DATA_NS_END

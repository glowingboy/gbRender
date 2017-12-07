#pragma once
#include "texture.h"
#include <unordered_map>
GB_RENDER_DATA_NAMESPACE_BEGIN

struct sprite
    {
	sprite():
	    uv_b(0.0f),
	    uv_l(0.0f),
	    uv_t(0.0f),
	    uv_r(0.0f),
	    width(0),
	    height(0)
	    {}
	float32 uv_b;
	float32 uv_l;
	float32 uv_t;
	float32 uv_r;
	uint32 width;
	uint32 height;
    };

template<typename Key, typename Sprite>
class _atlas:public texture
{
public:

protected:
    std::unordered_map<Key, Sprite> _mpSprites;
};

typedef _atlas<std::string, sprite> atlas;

GB_RENDER_DATA_NAMESPACE_END

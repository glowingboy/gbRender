#pragma once
#include "texture.h"
#include <unordered_map>
GB_RENDER_DATA_NAMESPACE_BEGIN
//#pragma pack(push, 4)
//serialize in other way
struct sprite
    {
	sprite():
	    uv_b(0.0f),
	    uv_l(0.0f),
	    uv_t(0.0f),
	    uv_r(0.0f)
	    {}
	float32 uv_b;
	float32 uv_l;
	float32 uv_t;
	float32 uv_r;
    };
//#pragma pack(pop)

template<typename Key>
class _atlas:public texture
{
public:

private:
    std::unordered_map<Key, sprite*> _mpSprites;
};

typedef _atlas<std::string> atlas;

GB_RENDER_DATA_NAMESPACE_END

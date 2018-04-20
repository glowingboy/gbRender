#pragma once
#include "../GLTexture.h"
#include <unordered_map>
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
#include "DataNS.h"

GB_RENDER_DATA_NS_BEGIN

struct Sprite
    {
	Sprite():
	    uv_b(0.0f),
	    uv_l(0.0f),
	    uv_t(0.0f),
	    uv_r(0.0f),
	    width(0),
	    height(0)
	    {}
	float uv_b;
	float uv_l;
	float uv_t;
	float uv_r;
	std::uint32_t width;
	std::uint32_t height;
    };

template<typename Key_t, typename Sprite_t>
class _atlas:public gb::render::GLTexture
{
public:
	_atlas(){}
	template <typename T>
	_atlas(T && other) :
		gb::render::GLTexture(std::forward<T>(other)),
		_mpSprites(std::forward<T>(other._mpSprites))
	{

	}
	const Sprite_t& GetSprite(const Key_t key) const
	{
		auto iter = _mpSprites.find(key);
		if (iter != _mpSprites.end())
			return iter->second;
		else
		{
			gb::utils::logger::Instance().error(gb::utils::string("_atlas::GetSprite specified sprite not found key@ ") + key);
			return _mpSprites.begin()->second;
		}
	}
protected:
    std::unordered_map<Key_t, Sprite_t> _mpSprites;
};

typedef _atlas<gb::utils::string, Sprite> Atlas;

GB_RENDER_DATA_NS_END

#pragma once
#include <cstdint>
/****************fundamental types****************/
#define GB_TYPE_DEF(type, alias, size)		\
    typedef type alias;				\
    static_assert(sizeof(type) == size, "unexpected type size @");

namespace gb
{
    namespace render
    {
	/********byte size check********/
	static_assert(sizeof(std::int8_t) == 1, "unexpected byte size");
	/********byte size check********/
	typedef std::int8_t int8;
	typedef std::uint8_t uint8;
	typedef std::int16_t int16;
	typedef std::uint16_t uint16;
	typedef std::int32_t int32;	
	typedef std::uint32_t uint32;
	typedef std::int64_t int64;
	typedef std::uint64_t uint64;
	GB_TYPE_DEF(float, float32, 4);
	GB_TYPE_DEF(double, float64, 8);
    }
};

/****************fundamental types****************/

#include "thirdpartylib/glm/glm.hpp"



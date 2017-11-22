#pragma once
#include "../type.h"

using namespace gb::render;

#define GB_RENDER_DATA_NAMESPACE_BEGIN		\
    namespace gb				\
    {						\
	namespace render			\
	{					\
	    namespace data			\
	    {

#define GB_RENDER_DATA_NAMESPACE_END		\
    };						\
	    };					\
	};

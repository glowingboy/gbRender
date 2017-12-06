#pragma once

#include "../type.h"

using namespace gb::render;

#define GB_RENDER_FILE_NAMESPACE_BEGIN		\
    namespace gb				\
    {						\
	namespace render			\
	{					\
	    namespace file			\
	    {

#define GB_RENDER_FILE_NAMESPACE_END		\
    };};};

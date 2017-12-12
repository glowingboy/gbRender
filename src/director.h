#pragma once
#include <gbUtils/common.h>
#include "type.h"
#include <gbPhysics/algorithm.h>
#include <gbUtils/string.h>
#include "gbRenderConfig.h"
using namespace gb::render;
using gb::algorithm::vec2;
namespace gb
{
	namespace render
	{
		GB_RENDER_CLASS director
		{
			GB_SINGLETON(director);
		public:
			void Ready(const vec2<uint32>& screenSize);
			void Action();
			void Directing();
			inline const vec2<uint32>& GetScreenSize()const
			{
				return _screenSize;
			}
		private:
			vec2<uint32> _screenSize;
		};
	};
};

#pragma once
#include <gbUtils/common.h>
#include "Type.h"

#include <gbUtils/string.h>
#include "Config.h"

#include "Scene.h"

GB_RENDER_NS_BEGIN
		GB_RENDER_CLASS Director
		{
			GB_SINGLETON(Director);
		public:
			Scene* Ready(const vec2<uint32>& screenSize);
			void Action();
	
			inline const vec2<uint32>& GetScreenSize()const
			{
				return _screenSize;
			}
		private:
			vec2<uint32> _screenSize;
		private:
			bool _directing();
			Scene* _curScene;
		};
GB_RENDER_NS_END

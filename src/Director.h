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
			Scene* Ready(const char* firstSceneFilePath = nullptr);
			void Action();
			Scene* LoadScene(const char* sceneFilePath = nullptr);
		private:
			bool _directing();
			Scene* _curScene;
		};
GB_RENDER_NS_END

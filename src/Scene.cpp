//#include "Scene.h"
//#include "data/Entity.h"
//using namespace gb::render;
//using namespace gb::utils;
//Scene::Scene(const char* sceneFilePath)
//{
//	if (sceneFilePath != nullptr)
//	{
//		luatable_mapper mapper(sceneFilePath, luastate_mgr::Instance().getconfig_state());
//		if (mapper.validate())
//		{
//			data::Entity e;
//			e.from_lua(mapper);
//			_root.Instantiate(e);
//		}
//		else
//
//	}
//
//}
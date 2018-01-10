#include "Scene.h"
#include "data/Entity.h"
using namespace gb::render;
using namespace gb::utils;
Scene::Scene(const char* sceneFilePath)
{
	data::Entity e;
	luatable_mapper mapper(sceneFilePath, luastate_mgr::Instance().getconfig_state());
	e.from_lua(mapper);

	_root.Instantiate(e);
}
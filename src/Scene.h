#pragma once 
#include "Config.h"
#include "RenderNS.h"

#include <unordered_map>
#include <gbUtils/string.h>
#include "Entity.h"
GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Scene
{
public:
	/*
	 *@brief, Instantiate a scene from a sceneFile while sceneFilePath != nullptr
	 *@param, if sceneFilePath == nullptr, then return a empty scene
	*/
	Scene(const char* sceneFilePath = nullptr);
	Entity* AddEntity(const char* name = nullptr);
private:
	std::unordered_multimap<gb::utils::string, Entity*> _mpEntities;
};

GB_RENDER_NS_END
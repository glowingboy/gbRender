#pragma once 
#include "Config.h"
#include "RenderNS.h"

GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Scene
{
public:
	/*
	 *@brief, empty scene
	*/
	Scene();
	/*
	 *@brief, Instantiate a scene from a sceneFile
	*/
	Scene(const char* sceneFilePath);
	//AddObj();
};

GB_RENDER_NS_END
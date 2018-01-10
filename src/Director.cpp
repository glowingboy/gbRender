#include "Director.h"
#include "Device.h"
#include <gbUtils/logger.h>

#include <gbUtils/luastate.h>

using namespace gb::render;
using namespace gb::utils;

Scene* Director::Ready(const char* firstSceneFilePath)
{

	if (!Device::Instance().IsInitialized())
	{
		logger::Instance().error("Director::Ready Device has not been Initialized");
		return nullptr;
	}

	//luastate initialize
	luastate_mgr::Instance().initialize();
	
	LoadScene(firstSceneFilePath);

	return _curScene;
}

void Director::Action()
{
//	try
//	{
		for (;;)
		{
			if (!_directing())
				break;

		}
//	}
	//catch (const string& error)
	//{
	//	logger::Instance().error("director::Action quit@" + error);
	//}
	//catch (...)
	//{
	//	logger::Instance().error("director::Action quit unknown error");
	//}
}

bool Director::_directing()
{
	return Device::Instance().Update();
}

Scene* Director::LoadScene(const char* sceneFilePath)
{
	_curScene = new Scene(sceneFilePath);
	return _curScene;
}
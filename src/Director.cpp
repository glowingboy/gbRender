#include "Director.h"
#include "Device.h"
#include <gbUtils/logger.h>
using namespace gb::render;
using gb::utils::string;
using gb::utils::logger;
Scene* Director::Ready(const char* firstSceneFilePath)
{

	if (!Device::Instance().IsInitialized())
	{
		logger::Instance().error("Director::Ready Device has not been Initialized");
		return nullptr;
	}

	_curScene = LoadScene(firstSceneFilePath);

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

Scene* Director::LoadScene(const char* sceneFilePath = nullptr)
{

}
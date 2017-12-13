#include "Director.h"
#include "Device.h"
#include <gbUtils/logger.h>
using namespace gb::render;
using gb::utils::string;
using gb::utils::logger;
Scene* Director::Ready(const vec2<uint32>& screenSize)
{
    _screenSize = screenSize;

    if(! Device::Instance().Initialize())
		throw string("director::Initialize device::Initialize failed");

	_curScene = new Scene;

	return _curScene;
}

void Director::Action()
{
	try
	{
		for (;;)
		{
			if (!_directing())
				break;
		}
	}
	catch (const string& error)
	{
		logger::Instance().error("director::Action quit@" + error);
	}
	catch (...)
	{
		logger::Instance().error("director::Action quit unknown error");
	}
}

bool Director::_directing()
{
	return Device::Instance().Update();
}
#include "Director.h"
#include "Device.h"
#include <gbUtils/luastate.h>

using namespace gb::render;
using namespace gb::utils;

Director::Argument::Argument(const char* fileOfRootEntity, const gb::algorithm::vec2<gb::render::uint32>& sizeOfScreen):
	rootEntity(fileOfRootEntity),
	screenSize(sizeOfScreen)
{

}

Director::Argument::Argument(Argument && other) :
	rootEntity(std::move(other.rootEntity)),
	screenSize(other.screenSize)
{
}

bool Director::Ready(const Argument& arg)
{
	if (!Device::Instance().Initialize(arg.screenSize))
	{
		logger::Instance().error("Director::Ready Device has not been Initialized");
		return false;
	}

	//luastate initialize
	luastate_mgr::Instance().initialize();
	
	if (arg.rootEntity.length() != 0)
		_Root.Instantiate(arg.rootEntity);

	return true;
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
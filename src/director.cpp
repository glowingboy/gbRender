#include "director.h"
#include "device.h"
#include <gbUtils/logger.h>
using namespace gb::render;
using gb::utils::string;
using gb::utils::logger;
void director::Ready(const vec2<uint32>& screenSize)
{
    _screenSize = screenSize;

    if(! device::Instance().Initialize())
		throw string("director::Initialize device::Initialize failed");
}

void director::Action()
{
	try
	{
		for (;;)
		{
			Directing();
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

void director::Directing()
{
	device::Instance().Update();
}
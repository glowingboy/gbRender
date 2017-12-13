#pragma once 
#include "GBObject.h"
#include "Config.h"
#include "RenderNS.h"
#include <gbUtils/string.h>

GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Entity: protected GBObject
{

private:
	gb::utils::string _name;
};

GB_RENDER_NS_END
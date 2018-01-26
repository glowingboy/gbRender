#pragma once 
#include "RenderNS.h"
#include <gbUtils/common.h>

GB_RENDER_NS_BEGIN

/*
 * base class of Entity and Component
*/

//TODO Message Send
class GBObject
{
protected:
	inline GBObject():
		_Active(true)
	{}
private:
	virtual void Start() = 0;
	virtual void End() = 0;

	GB_PROPERTY(protected, Active, bool);
	
};

GB_RENDER_NS_END
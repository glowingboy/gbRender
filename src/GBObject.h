#pragma once 
#include "RenderNS.h"

GB_RENDER_NS_BEGIN

/*
 * base class of Entity and Component
*/
class GBObject
{
protected:
	GBObject(){}
private:
	virtual void Start() = 0;
};

GB_RENDER_NS_END
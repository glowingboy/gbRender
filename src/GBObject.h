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
	/*
	 *@brief called after created
	*/
	virtual void Awake() = 0;
	/*
	 *@brief called after all siblings'(known by container, when instantiate prefab) Awake having been called
	*/
	virtual void Start() = 0;
};

GB_RENDER_NS_END
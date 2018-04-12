#pragma once

#include "Element.h"
#include "data/Atlas.h"
GB_RENDER_NS_BEGIN

class Sprite : public Element
{
private:
	virtual void Start() override;
	virtual void End() override;
	virtual void Awake() override;

private:
	GB_PROPERTY_R(private, Atlas, const data::Atlas*);
};

GB_RENDER_NS_END
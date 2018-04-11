#pragma once

#include "Element.h"

GB_RENDER_NS_BEGIN

class Sprite : public Element
{
private:
	virtual void Start() override;
	virtual void End() override;
	virtual void Awake() override;
};

GB_RENDER_NS_END
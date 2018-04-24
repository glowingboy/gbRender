#pragma once

#include "Element.h"

GB_RENDER_DATA_NS_BEGIN

class FPVController : public Element
{
public:
	FPVController();
public:

	virtual gb::render::Element* Instantiate(gb::render::Entity* owner) const override;
};

GB_RENDER_DATA_NS_END

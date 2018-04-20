#pragma once

#include "Element.h"

GB_RENDER_DATA_NS_BEGIN

class FPVController : public Element
{
public:
	FPVController();
public:

	virtual gb::render::Element* Instantiate(gb::render::Entity* const owner) const;
	virtual void from_lua(const gb::utils::luatable_mapper & mapper);
};

GB_RENDER_DATA_NS_END
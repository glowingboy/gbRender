#pragma once
#include "NS.h"
#include <data/Element.h>

GB_EDITOR_DATA_NS_BEGIN

class Main : public gb::render::data::Element
{
public:
	virtual gb::render::Element* Instantiate(gb::render::Entity* const owner);
	virtual void from_lua(const gb::utils::luatable_mapper & mapper);
};

GB_EDITOR_DATA_NS_END
#pragma once
#include "NS.h"
#include <data/Element.h>

GB_EDITOR_DATA_NS_BEGIN

class Main : public gb::render::data::Element
{
public:
	Main();
public:
	virtual gb::render::Element* Instantiate(gb::render::Entity* owner) const override;
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) override;
};

GB_EDITOR_DATA_NS_END
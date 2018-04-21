#pragma once

#include "Element.h"

#define GB_RENDER_DATA_TEXT_KEY_MATERIAL "Material"
#define GB_RENDER_DATA_TEXT_KEY_TEXT "Text"

GB_RENDER_DATA_NS_BEGIN

class Text : public Element
{
public:
	Text();
public:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) override;
	virtual gb::render::Element* Instantiate(gb::render::Entity* owner) const override;

	GB_PROPERTY_R(private, Material, gb::utils::string);
	GB_PROPERTY_R(private, Text, gb::utils::string);
};

GB_RENDER_DATA_NS_END
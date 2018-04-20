#pragma once

#include "Element.h"
#include <gbUtils/common.h>

#define GB_RENDER_DATA_TEXTURE_KEY_MATERIAL "Material"

GB_RENDER_DATA_NS_BEGIN

class Texture : public Element
{
public:
	Texture();
	virtual gb::render::Element* Instantiate(gb::render::Entity* const owner) const;
	virtual void from_lua(const gb::utils::luatable_mapper & mapper);

	GB_PROPERTY_R(private, Material, gb::utils::string);
};

GB_RENDER_DATA_NS_END

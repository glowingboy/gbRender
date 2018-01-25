#pragma once
#include "Element.h"
#include <gbUtils/string.h>

#define GB_RENDER_DATA_RENDER_KEY_MESH "Mesh"
#define GB_RENDER_DATA_RENDER_KEY_MATERIAL "Material"

GB_RENDER_DATA_NS_BEGIN

class Render : public Element
{
public:
	Render();
public:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) override;
	virtual gb::render::Element* Instantiate(gb::render::Entity* const owner) const override;
	GB_PROPERTY_R(private, Mesh, gb::utils::string);
	GB_PROPERTY_R(private, Material, gb::utils::string);
};

GB_RENDER_DATA_NS_END
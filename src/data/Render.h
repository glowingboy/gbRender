#pragma once
#include "Element.h"
#include <gbUtils/string.h>

GB_RENDER_DATA_NS_BEGIN

class Render : public Element
{
public:
	Render(const gb::render::Element::Type type, const gb::utils::luatable_mapper & mapper);
public:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) override;

	GB_PROPERTY_R(private, Mesh, gb::utils::string);
	GB_PROPERTY_R(private, Material, gb::utils::string);
};

GB_RENDER_DATA_NS_END
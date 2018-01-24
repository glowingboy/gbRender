#pragma once
#include "DataNS.h"
#include <gbUtils/luatable.h>
#include "../Type.h"
#include "../Element.h"

#include "../RenderNS.h"

GB_RENDER_DATA_NS_BEGIN

#define GB_RENDER_DATA_ELEMENT_KEY_TYPE "Type"

class ElementAdapter;

class Element
{
	friend class ElementAdapter;
public:
	Element(const gb::render::Element::Type type);
public:
	virtual gb::render::Element* Instantiate() const = 0;
private:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) = 0;
	GB_PROPERTY_R(protected, Type, gb::render::Element::Type);
};


class ElementAdapter
{
public:
	ElementAdapter();
public:
	void from_lua(const gb::utils::luatable_mapper& mappper);
	GB_PROPERTY_R(private, Element, Element*);
private:
	gb::render::Element::Type _Type;
};

GB_RENDER_DATA_NS_END

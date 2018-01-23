#pragma once
#include "DataNS.h"
#include <gbUtils/luatable.h>
#include "../Type.h"
#include "../Element.h"
GB_RENDER_DATA_NS_BEGIN

#define GB_RENDER_DATA_ELEMENT_KEY_TYPE "Type"

class Element
{
public:
	Element(const gb::render::Element::Type type);
	Element(const gb::render::Element::Type type, const gb::utils::luatable_mapper & mapper);
private:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) = 0;
	GB_PROPERTY_R(protected, Type, gb::render::Element::Type);
};
class ElementAdapter: protected Element
{
public:
	ElementAdapter();
public:
    void from_lua(const gb::utils::luatable_mapper& mappper);
	GB_PROPERTY_R(private, Element, Element*);
};

GB_RENDER_DATA_NS_END

#pragma once
#include "DataNS.h"
#include <gbUtils/luatable.h>
#include "../Type.h"
#include "../Element.h"

#include "../RenderNS.h"

#include <unordered_map>

namespace gb
{
	namespace render
	{
		class Entity;
	}
}


GB_RENDER_DATA_NS_BEGIN

#define GB_RENDER_DATA_ELEMENT_KEY_TYPE "Type"

class ElementAdapter;


#define GB_RENDER_DATA_ELEMENT_RECIPE_REG(type, element) \
	gb::render::data::Element::Recipe::Instance().Register(type, []()->gb::render::data::Element* \
{ \
	return new element; \
});

class Element
{
public:
	class Recipe
	{
		GB_SINGLETON(Recipe);
	public:
		void PreReg();
		void Register(const std::uint32_t type, const std::function<Element*(void)>& func);
		GB_PROPERTY_R(private, Contents, std::unordered_map<std::uint32_t, std::function<Element*(void)>>);
	};
	friend class ElementAdapter;
public:
	Element(const std::uint32_t type);
public:
	virtual gb::render::Element* Instantiate(gb::render::Entity* owner) const = 0;
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) = 0;
	GB_PROPERTY_R(protected, Type, std::uint32_t);
};


class ElementAdapter
{
public:
	ElementAdapter();
public:
	void from_lua(const gb::utils::luatable_mapper& mappper);
	GB_PROPERTY_R(private, Element, Element*);
private:
	std::uint32_t _Type;
};


GB_RENDER_DATA_NS_END

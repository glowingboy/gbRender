#include "Element.h"

#include "Render.h"
#include "Camera.h"
#include "Texture.h"

#include "Text.h"
#include "FPVController.h"
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
using namespace gb;
using namespace gb::render::data;
using namespace gb::utils;

Element::Element(const std::uint32_t type):
	_Type(type)
{}

ElementAdapter::ElementAdapter():
	_Type(render::Element::Type::Default),
	_Element(nullptr)
{}

void Element::Recipe::Register(const std::uint32_t type, const std::function<Element*(void)>& func)
{
	_Contents.insert(std::make_pair(type, func));
}

void Element::Recipe::PreReg()
{
	GB_RENDER_DATA_ELEMENT_RECIPE_REG(render::Element::Type::Camera, Camera);
	GB_RENDER_DATA_ELEMENT_RECIPE_REG(render::Element::Type::Render, Render);
	GB_RENDER_DATA_ELEMENT_RECIPE_REG(render::Element::Type::Texture, Texture);
	GB_RENDER_DATA_ELEMENT_RECIPE_REG(render::Element::Type::Text, Text);
	GB_RENDER_DATA_ELEMENT_RECIPE_REG(render::Element::Type::FPVController, FPVController);

}
void ElementAdapter::from_lua(const luatable_mapper& mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_ELEMENT_KEY_TYPE))
		_Type = mapper.get_integer_by_key(GB_RENDER_DATA_ELEMENT_KEY_TYPE);
	else
	{
		logger::Instance().error(string("ElementAdapter::from_lua error@ ") + mapper.GetData());
		return;
	}

	const auto& recipe = Element::Recipe::Instance().GetContents();

	const auto iter = recipe.find(_Type);
	if (iter != recipe.end())
		_Element = iter->second();
	else
	{
		logger::Instance().error(string("ElementAdapter::from_lua unkown type error@ ") + mapper.GetData() + "type@ " + (int)_Type);
		return;
	}
		
	_Element->from_lua(mapper);
}

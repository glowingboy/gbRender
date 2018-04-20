#include "Element.h"

#include "Render.h"
#include "Camera.h"
#include "Text.h"
#include "FPVController.h"
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
using namespace gb;
using namespace gb::render::data;
using namespace gb::utils;

Element::Element(const gb::render::Element::Type type):
	_Type(type)
{}

ElementAdapter::ElementAdapter():
	_Type(render::Element::Type::Default),
	_Element(nullptr)
{}

void ElementAdapter::from_lua(const luatable_mapper& mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_ELEMENT_KEY_TYPE))
		_Type = (render::Element::Type)(mapper.get_integer_by_key(GB_RENDER_DATA_ELEMENT_KEY_TYPE));
	else
	{
		logger::Instance().error(string("ElementAdapter::from_lua error@ ") + mapper.GetData());
		return;
	}
	if (_Type == render::Element::Type::Camera)
		_Element = new Camera();
	else if (_Type == render::Element::Type::Render)
		_Element = new Render();
	else if (_Type == render::Element::Type::Text)
		_Element = new Text();
	else if (_Type == render::Element::Type::FPVController)
		_Element = new FPVController();
	else
	{
		logger::Instance().error(string("ElementAdapter::from_lua unkown type error@ ") + mapper.GetData() + "type@ " + (int)_Type);
		return;
	}
		
	_Element->from_lua(mapper);
}

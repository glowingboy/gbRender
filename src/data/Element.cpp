#include "Element.h"

#include "Render.h"
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
		logger::Instance().error(string("ElementAdapter::from_lua error@ ") + mapper.GetFile());
		return;
	}
	if (_Type == render::Element::Type::Render)
		_Element = new Render();
	else if (_Type == render::Element::Type::Text)
		/*...*/;
	else
		return;
	_Element->from_lua(mapper);
}

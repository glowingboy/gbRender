#include "EntityPicker.h"
#include "../EntityPicker.h"

using namespace gb::render::data;
using namespace gb;

EntityPicker::EntityPicker():
	Element(render::Element::Type::EntityPicker)
{}

gb::render::Element* EntityPicker::Instantiate(gb::render::Entity* owner) const
{
    return new render::EntityPicker(owner);
}

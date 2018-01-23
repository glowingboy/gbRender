#include "Render.h"

using namespace gb::render::data;

Render::Render(const gb::render::Element::Type type, const gb::utils::luatable_mapper & mapper) :
	Element(type, mapper)
{}

void Render::from_lua(const gb::utils::luatable_mapper & mapper)
{

}

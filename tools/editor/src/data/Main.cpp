#include "Main.h"
#include "../Main.h"

#include "Element.h"
using namespace gb::editor::data;
using namespace gb;

Main::Main():
	render::data::Element(ElementType::Main)
{
}

gb::render::Element * Main::Instantiate(gb::render::Entity * const owner) const
{
	return new editor::Main(owner);
}

void Main::from_lua(const gb::utils::luatable_mapper & mapper)
{
}
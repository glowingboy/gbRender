#include "Main.h"
#include "../Main.h"


using namespace gb::editor::data;
using namespace gb;
gb::render::Element * Main::Instantiate(gb::render::Entity * const owner)
{
	return new editor::Main(owner);
}

void Main::from_lua(const gb::utils::luatable_mapper & mapper)
{
}
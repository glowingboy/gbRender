#include "Main.h"
#include "data/Element.h"

using namespace gb::editor;
using namespace gb;

Main::Main(gb::render::Entity* owner):
	render::Element(owner)
{

}

void Main::Awake()
{

}

void Main::Start()
{

}

void Main::End()
{

}

std::uint32_t Main::GetType() const
{
	return data::ElementType::Main;
}
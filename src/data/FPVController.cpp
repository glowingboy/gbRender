#include "FPVController.h"
#include "../FPVController.h"

using namespace gb::render::data;
using namespace gb;

FPVController::FPVController():
	Element(render::Element::Type::FPVController)
{
}

gb::render::Element * FPVController::Instantiate(gb::render::Entity * const owner) const
{
	return new render::FPVController(owner);
}

void FPVController::from_lua(const gb::utils::luatable_mapper & mapper)
{

}
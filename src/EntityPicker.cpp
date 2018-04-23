#include "EntityPicker.h"
#include "Input.h"
#include <gbUtils/logger.h>
#include <gbUtils/string.h>

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;

EntityPicker::EntityPicker(Entity * owner):
	Element(owner),
	_Ray(vec3F(0.0f))
{
}

std::uint32_t EntityPicker::GetType() const
{
	return std::uint32_t();
}

void EntityPicker::Awake()
{
	GB_UTILS_CALLBACK_REG(Input::Instance().GetClickedCBs(), EntityPicker::OnClicked);
}

void EntityPicker::Start()
{
}

void EntityPicker::End()
{
	GB_UTILS_CALLBACK_UNREG(Input::Instance().GetClickedCBs(), EntityPicker::OnClicked);
}

void EntityPicker::OnClicked()
{
	logger::Instance().log("EntityPicker::OnClicked");
}

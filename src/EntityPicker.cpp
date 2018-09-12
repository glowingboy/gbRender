#include "EntityPicker.h"
#include "Input.h"
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
#include "Entity.h"
#include "Camera.h"

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;

EntityPicker::EntityPicker(Entity * owner):
	Element(owner),
	_Ray(vec3F(0.0f)),
	_Camera(nullptr)
{
}

std::uint32_t EntityPicker::GetType() const
{
	return std::uint32_t();
}

void EntityPicker::Awake()
{
	
}

void EntityPicker::Start()
{
	_Camera = gb_cast<Camera>(_Owner->GetElement(Element::Type::Camera));

	if (_Camera != nullptr)
	{
		GB_UTILS_CALLBACK_REG(Input::Instance().GetClickedCBs(), EntityPicker::OnClicked);
	}
	else
		logger::Instance().warning("EntityPicker::Start, camera not found _Owner@ " + _Owner->GetName());
}

void EntityPicker::End()
{
	GB_UTILS_CALLBACK_UNREG(Input::Instance().GetClickedCBs());
}

void EntityPicker::OnClicked()
{
	const vec2F cursorPos(Input::Instance().GetCursorPosition());
	
	_Ray.update(_Camera->Screen2World(cursorPos));

	

	//logger::Instance().log(string("EntityPicker::OnClicked,cursorPos: x@ ") + cursorPos.x + ", y@ " + cursorPos.y);
	
}

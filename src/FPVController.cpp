#include "FPVController.h"

#include "Input.h"


#include <gbUtils/logger.h>
#include <gbUtils/string.h>
#include "Entity.h"

using namespace gb::render;
using namespace gb::utils;
using namespace gb::physics;

FPVController::FPVController(Entity * owner):
	Element(owner),
	_PosStep(GB_RENDER_INPUT_DEFAULT_POSITION_MOVE_STEP)
{
}

void FPVController::Awake()
{
	
}

void FPVController::Start()
{
	GB_UTILS_CALLBACK_REG_2P(Input::Instance().GetCBs(), FPVController::OnInput);
}

void FPVController::End()
{
	GB_UTILS_CALLBACK_UNREG(Input::Instance().GetCBs());
}

std::uint32_t FPVController::GetType() const
{
	return Element::Type::FPVController;
}

void FPVController::OnInput(const std::int32_t code, const bool down)
{
	logger::Instance().log(string("code: ") + code + ", down: " + down);

	Transform& trans = _Owner->GetTransform();
	const vec4F oldPosition = trans.GetLocalPosition();
	if (down)
	{
		switch (code)
		{
		case 'W':
			trans.SetPosition(vec3F(oldPosition.x, oldPosition.y, oldPosition.z + _PosStep));
			break;
		case 'S':
			trans.SetPosition(vec3F(oldPosition.x, oldPosition.y, oldPosition.z - _PosStep));
			break;
		case 'A':
			trans.SetPosition(vec3F(oldPosition.x + _PosStep, oldPosition.y, oldPosition.z));
			break;
		case 'D':
			trans.SetPosition(vec3F(oldPosition.x - _PosStep, oldPosition.y, oldPosition.z));
			break;
		default:
			break;
		}
		
	}

}
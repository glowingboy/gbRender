#include "Input.h"
#include "Device.h"
using namespace gb::render;

void Input::Process(const std::int32_t code, const bool down)
{
	_CBs.Trigger(code, down);
	if (code == GB_RENDER_INPUT_MS_L)
	{
		if (_ClickDown && !down)
			_ClickedCBs.Trigger();
	
		_ClickDown = down;
	}
}

gb::physics::vec2<long> Input::GetCursorPosition() const
{
	return Device::Instance().GetCursorPosition();
}

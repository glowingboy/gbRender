#include "Input.h"

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
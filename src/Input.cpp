#include "Input.h"

using namespace gb::render;

void Input::Process(const std::uint8_t code, const bool down)
{
	_CBs.Trigger(code, down);
}


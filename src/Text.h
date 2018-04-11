#pragma once

#include "Sprite.h"

GB_RENDER_NS_BEGIN

class Text : public Sprite
{
public:
	void SetText(const wchar_t* strText);
	void SetText(const std::uint32_t glyphCode);
};

GB_RENDER_NS_END
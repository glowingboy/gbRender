#pragma once

#include "Render.h"
#include "data/Font.h"
#include <gbPhysics/type.h>

#define GB_REDNER_TEXT_DEFAULT_SIZE 20
#define GB_RENDER_TEXT_DEFAULT_LINESPACE 2
GB_RENDER_NS_BEGIN

class Text : public BaseRender
{
public:
	Text(Entity* owner);
public:
	void SetFont(const char* font);
	void SetSize(const std::uint32_t pixelSize);
	void SetLineSpace(const std::uint32_t lineSpace);
	void SetText(const wchar_t* strText, const bool append = false);
	//void SetText(const std::uint32_t glyphCode);

private:
	GB_PROPERTY_R(private, Font, const data::Font*);

	GLVar* _vtxPos;
	GLVar* _vtxUV;
	GLVar* _vtxIdx;

	gb::physics::vec2<std::int32_t> _penPos;

	std::uint32_t _pixelSize;

	//respect to glyph origin size
	float _textScale;

	std::uint32_t _lineSpace;

	std::vector<wchar_t> _text;
};

GB_RENDER_NS_END
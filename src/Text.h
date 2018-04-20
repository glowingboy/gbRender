#pragma once

#include "Render.h"
#include "data/Font.h"
#include <gbPhysics/type.h>

#define GB_REDNER_TEXT_DEFAULT_SIZE 20
#define GB_RENDER_TEXT_DEFAULT_LINESPACE 4
#define GB_RENDER_TEXT_DEFAULT_SPACE 4
GB_RENDER_NS_BEGIN

class Text : public BaseRender
{
public:
	Text(Entity* owner);
	~Text();
public:
	void SetFont(const char* font);
	void SetSize(const std::uint32_t pixelSize);
	void SetLineSpace(const std::uint32_t lineSpace);
	void SetSpace(const std::uint32_t space);
	void SetText(const char* szText, const bool append = false);
	void SetText(const wchar_t* szText, const bool append = false);
	//void SetText(const std::uint32_t glyphCode);

private:
	template <typename T>
	void _setText(const T* szText, const std::size_t count, bool append);
	void _genText(const wchar_t* strText, const std::size_t beginOffset, const std::size_t count);
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
	std::uint32_t _space;

	std::vector<wchar_t> _text;
};

GB_RENDER_NS_END
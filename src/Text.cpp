#include "Text.h"
#include "resource/Resource.h"
using namespace gb::render;
using namespace gb::utils;
using namespace gb;

Text::Text(Entity* owner) :
	BaseRender(owner),
	_Font(nullptr),
	_vtxPos(nullptr),
	_vtxUV(nullptr),
	_vtxIdx(nullptr),
	_lineSpace(GB_RENDER_TEXT_DEFAULT_LINESPACE)
{

	SetSize(GB_REDNER_TEXT_DEFAULT_SIZE);

	_Mesh = new data::Mesh;

	_vtxPos = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_POS, 3 * sizeof(float));
	_vtxUV = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_UV, 2 * sizeof(float));

	_vtxIdx = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_IDX, sizeof(std::uint32_t));
}

void Text::SetFont(const char * font)
{
	_Font = resource::Res<data::Font>::Instance().Get(font);
	if (_Font == nullptr)
		logger::Instance().error(string("Text::SetFont font not found font@ ") + font);

	SetText(_text.data());
}

void Text::SetSize(const std::uint32_t pixelSize)
{
	_pixelSize = pixelSize;

	_textScale = ((float)_pixelSize / _Font->GetGlyphSize());

	SetText(_text.data());
}

void Text::SetLineSpace(const std::uint32_t lineSpace)
{
	_lineSpace = lineSpace;

	SetText(_text.data());
}

void Text::SetText(const wchar_t * strText, const bool append)
{
	const size_t count = std::wcslen(strText);

	std::int32_t xOffset = _penPos.x;
	std::int32_t yOffset = _penPos.x;

	if (!append)
	{
		_vtxPos->clear();
		_vtxUV->clear();
		_vtxIdx->clear();

		_text.clear();

		xOffset = 0;
		yOffset = -(_pixelSize + _lineSpace);
	}

	const std::size_t newPosCount = 4 * count;
	const std::size_t newIdxCount = 6 * count;

	_vtxPos->reserve(_vtxPos->count() + newPosCount);
	_vtxUV->reserve(_vtxUV->count() + newPosCount);
	_vtxIdx->reserve(_vtxIdx->count() + newIdxCount);

	_text.insert(_text.end(), strText, strText + count + 1/*including ending '\0'*/);


	for (std::size_t i = 0; i < count; i++)
	{
		//if == '\n'
		_Font->GetSprite(strText[i]);
	}
}

#include "Text.h"
#include "resource/Resource.h"
#include <gbPhysics/type.h>
#include "data\Text.h"

using namespace gb::render;
using namespace gb::utils;
using namespace gb;

using namespace gb::physics;

Text::Text(Entity* owner) :
	BaseRender(owner),
	_Font(nullptr),
	_vtxPos(nullptr),
	_vtxUV(nullptr),
	_vtxIdx(nullptr),
	_pixelSize(GB_REDNER_TEXT_DEFAULT_SIZE),
	_lineSpace(GB_RENDER_TEXT_DEFAULT_LINESPACE)
{

	SetFont("FZSTK.gbFont");

	//mesh
	_Mesh = new data::Mesh;

	_vtxPos = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_POS, 3 * sizeof(float));
	_vtxUV = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_UV, 2 * sizeof(float));

	_vtxIdx = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_IDX, sizeof(std::uint32_t));

	//matrial
	_Material = resource::Res<data::Material>::Instance().Get("SDFText");

	data::UniformTextureVar texVar = { _Font->GetTarget(), _Font->GetTextureObj() };

	_Material->SetTexture("font", &texVar);
}

void Text::SetFont(const char * font)
{
	_Font = resource::Res<data::Font>::Instance().Get(font);

	SetSize(_pixelSize);
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

template <typename T>
void Text::_setText(const T* szText, const std::size_t count, bool append)
{
	if (!append)
	{
		_vtxPos->clear();
		_vtxUV->clear();
		_vtxIdx->clear();

		_text.clear();

		_penPos.x = 0;
		_penPos.y = -1 * (_pixelSize + _lineSpace);
	}

	const size_t beginIdx = _text.size();

	_text.reserve(beginIdx + count);

	_text.insert(_text.end(), szText, szText + count);

	_setText(_text.data() + beginIdx, count);
}


void Text::_setText(const wchar_t * strText, const std::size_t count)
{
	const std::size_t newPosCount = 4 * count;
	const std::size_t newIdxCount = 6 * count;

	_vtxPos->reserve(_vtxPos->count() + newPosCount);
	_vtxUV->reserve(_vtxUV->count() + newPosCount);
	_vtxIdx->reserve(_vtxIdx->count() + newIdxCount);

	std::int32_t& xOffset = _penPos.x;
	std::int32_t& yOffset = _penPos.y;


	for (std::size_t i = 0; i < count; i++)
	{
		const std::uint32_t code = strText[i];
		if (code == '\n')
		{
			yOffset -= (_pixelSize + _lineSpace);
			xOffset = 0;
		}
		else
		{
			const data::Glyph& gly = _Font->GetSprite(strText[i]);

			const float bottom = yOffset - std::floorf(gly.yDelta * _textScale);
			const float right = xOffset + std::ceilf(gly.width * _textScale);
			const float top = yOffset + std::ceilf(gly.height * _textScale);

			/**
			0--1    4--5
			|  |----|  |
			3--2 |  7--6
			offsetX
			triangle:0-1-2, 2-3-0, 4-5-6, 6-7-4
			*/

			vec3F l_t(xOffset, top, 0.0f);
			vec3F r_t(right, top, 0.0f);
			vec3F r_b(right, bottom, 0.0f);
			vec3F l_b(xOffset, bottom, 0.0f);

			_vtxPos->append(&l_t, 1);
			_vtxPos->append(&r_t, 1);
			_vtxPos->append(&r_b, 1);
			_vtxPos->append(&l_b, 1);

			vec2F uv_l_t(gly.uv_l, gly.uv_t);
			vec2F uv_r_t(gly.uv_r, gly.uv_t);
			vec2F uv_r_b(gly.uv_r, gly.uv_b);
			vec2F uv_l_b(gly.uv_l, gly.uv_b);

			_vtxUV->append(&uv_l_t, 1);
			_vtxUV->append(&uv_r_t, 1);
			_vtxUV->append(&uv_r_b, 1);
			_vtxUV->append(&uv_l_b, 1);

			const std::uint32_t baseIdx = _text.size() * 4;

			std::uint32_t tmp = baseIdx;
			_vtxIdx->append(&tmp, 1);
			tmp = baseIdx + 1;
			_vtxIdx->append(&tmp, 1);
			tmp = baseIdx + 2;
			_vtxIdx->append(&tmp, 1);


			_vtxIdx->append(&tmp, 1);
			tmp = baseIdx + 3;
			_vtxIdx->append(&tmp, 1);
			tmp = baseIdx;
			_vtxIdx->append(&baseIdx, 1);

			xOffset += ((std::int32_t)std::ceilf(gly.advanceX * _textScale));
		}

	}

	_Mesh->UpdateSphereBB();
}

void Text::SetText(const char * szText, const bool append)
{
	_setText(szText, std::strlen(szText), append);
}

void Text::SetText(const wchar_t * szText, const bool append)
{
	_setText(szText, std::wcslen(szText), append);
}


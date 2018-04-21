#include "Text.h"
#include "../Text.h"

using namespace gb::render::data;
using namespace gb;
Text::Text():
	Element(render::Element::Type::Text)
{
}

void Text::from_lua(const gb::utils::luatable_mapper & mapper)
{
	_Material = mapper.get_string_by_key(GB_RENDER_DATA_TEXT_KEY_MATERIAL);
	_Text = mapper.get_string_by_key(GB_RENDER_DATA_TEXT_KEY_TEXT);
}

gb::render::Element * Text::Instantiate(gb::render::Entity * owner) const
{
	render::Text* txt = new render::Text(owner);

	txt->SetMaterial(_Material);

	txt->SetText(_Text);

	return txt;
}
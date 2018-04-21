#include "Texture.h"
#include "../Texture.h"

using namespace gb::render::data;
using namespace gb;

Texture::Texture():
	Element(render::Element::Type::Texture)
{}

gb::render::Element * Texture::Instantiate(gb::render::Entity * owner) const
{
	render::Texture* tex = new render::Texture(owner);
	if (_Material.length() != 0)
		tex->SetMaterial(_Material);

	return tex;
}

void Texture::from_lua(const gb::utils::luatable_mapper & mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_TEXTURE_KEY_MATERIAL))
		_Material = mapper.get_string_by_key(GB_RENDER_DATA_TEXTURE_KEY_MATERIAL);
}
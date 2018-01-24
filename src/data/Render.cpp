#include "Render.h"
#include "../Render.h"

using namespace gb::render::data;
using namespace gb;

Render::Render() :
	Element(render::Element::Type::Render)
{}

void Render::from_lua(const gb::utils::luatable_mapper & mapper)
{
	_Mesh = mapper.get_string_by_key(GB_RENDER_DATA_RENDER_KEY_MESH);
	_Material = mapper.get_string_by_key(GB_RENDER_DATA_RENDER_KEY_MATERIAL);
}


render::Element* Render::Instantiate() const
{
	render::Render* r = new render::Render;
	r->SetMesh(_Mesh);
	r->SetMaterial(_Material);
	return r;
}
#include "Texture.h"

#include "data/Font.h"
#include "resource/Resource.h"

using namespace gb::render;
using namespace gb::physics;

Texture::Texture(Entity * owner):
	BaseRender(owner)
{
	_Mesh = new data::Mesh;
	_originSBB = &(_Mesh->GetSphereBB());

	_vtxPos = _Mesh->GetPosVar();
	_vtxUV = _Mesh->InitializeAVtxVar(GB_RENDER_VTXVAR_UV, 2 * sizeof(float));

	GLVar& _vtxIdx = _Mesh->GetIdxVar();

	vec3F l_t(-GB_RENDER_TEXTURE_DEFAULT_SIDE, -GB_RENDER_TEXTURE_DEFAULT_SIDE, 0.0f);
	vec3F r_t(GB_RENDER_TEXTURE_DEFAULT_SIDE, -GB_RENDER_TEXTURE_DEFAULT_SIDE, 0.0f);
	vec3F r_b(GB_RENDER_TEXTURE_DEFAULT_SIDE, GB_RENDER_TEXTURE_DEFAULT_SIDE, 0.0f);
	vec3F l_b(-GB_RENDER_TEXTURE_DEFAULT_SIDE, GB_RENDER_TEXTURE_DEFAULT_SIDE, 0.0f);

	_vtxPos->append(&l_t, 1);
	_vtxPos->append(&r_t, 1);
	_vtxPos->append(&r_b, 1);
	_vtxPos->append(&l_b, 1);

	//texture origin is at left-bottom corner
	vec2F uv_l_t(0.0f, 1.0f);
	vec2F uv_r_t(1.0f, 1.0f);
	vec2F uv_r_b(1.0f, 0.0f);
	vec2F uv_l_b(0.0f, 0.0f);

	_vtxUV->append(&uv_l_t, 1);
	_vtxUV->append(&uv_r_t, 1);
	_vtxUV->append(&uv_r_b, 1);
	_vtxUV->append(&uv_l_b, 1);

	std::uint32_t tmp = 0;
	_vtxIdx.append(&tmp, 1);
	tmp = 1;
	_vtxIdx.append(&tmp, 1);
	tmp = 2;
	_vtxIdx.append(&tmp, 1);
	tmp = 2;
	_vtxIdx.append(&tmp, 1);
	tmp = 3;
	_vtxIdx.append(&tmp, 1);
	tmp = 0;
	_vtxIdx.append(&tmp, 1);

	_Mesh->UpdateSphereBB();

	//SetMaterial("DefaultTexture.lua");
	SetMaterial("SDFText.lua");
	
	data::Font* font = resource::Res<data::Font>::Instance().Get("FZSTK.gbFont");
	data::UniformTextureVar texVar = { font->GetTarget(), font->GetTextureObj() };

	_Material->SetTexture("font", &texVar);
}

void Texture::SetTexture(const char * szTexture)
{

}

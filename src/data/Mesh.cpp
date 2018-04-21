#include "Mesh.h"
#include <gbUtils/logger.h>
#include <gbPhysics/type.h>
#include "../Type.h"
using namespace gb::render::data;
using namespace gb::utils;
using namespace gb::physics;
using namespace gb;

Mesh::Mesh() :
	_IdxVar(nullptr),
	_VtxAttribByteSize(0)
{
}

template<render::uint8 count>
struct _lua_vec
{
	Float value[count];
	void from_lua(const gb::utils::luatable_mapper & mapper)
	{
		std::vector<lua_Number> vec = mapper.get_numbers();
		assert(count == vec.size());

		std::memcpy(this, vec.data(), count * sizeof(Float));
	}
};
void Mesh::from_lua(const gb::utils::luatable_mapper & mapper)
{

	// pos
	if (mapper.has_key(GB_RENDER_VTXVAR_POS))
	{
		auto itr = _VtxVars.insert(std::pair<const string, render::GLVar>(GB_RENDER_VTXVAR_POS, mapper.get_tables_by_key<_lua_vec<3>>(GB_RENDER_VTXVAR_POS)));

		const render::GLVar& var = itr.first->second;
		_VtxAttribByteSize += var.byteSize();
	}
	else
	{
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_pos key found@ " + mapper.GetData());
		return;
	}


	//idx
	if (mapper.has_key(GB_RENDER_VTXVAR_IDX))
	{
		auto itr = _VtxVars.insert(std::pair<string, render::GLVar>(GB_RENDER_VTXVAR_IDX, mapper.get_integers_by_key(GB_RENDER_VTXVAR_IDX)));

		const render::GLVar& var = itr.first->second;
		_IdxVar = &var;
	}
	else
	{
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_idx key found@ " + mapper.GetData());
		return;
	}


	//uv(optional)
	if (mapper.has_key(GB_RENDER_VTXVAR_UV))
	{
		auto itr = _VtxVars.insert(std::pair<string, render::GLVar>(GB_RENDER_VTXVAR_UV, mapper.get_tables_by_key<_lua_vec<2>>(GB_RENDER_VTXVAR_UV)));

		const render::GLVar& var = itr.first->second;
		_VtxAttribByteSize += (var.count() * var.unitSize());
	}
	_VtxVars.insert(std::pair<string, render::GLVar>(GB_RENDER_VTXVAR_UV, mapper.get_tables_by_key<_lua_vec<2>>(GB_RENDER_VTXVAR_UV)));


	UpdateSphereBB();
}

render::GLVar * Mesh::InitializeAVtxVar(const char * key, const std::size_t unitSize)
{
	auto iter = _VtxVars.find(key);

	if (iter == _VtxVars.end())
	{
		auto ret = _VtxVars.insert(std::make_pair(key, render::GLVar(unitSize)));

		if (string(GB_RENDER_VTXVAR_IDX) == key)
			_IdxVar = &(ret.first->second);

		return &(ret.first->second);
	}
	else
		return &(iter->second);
}

void Mesh::UpdateSphereBB()
{
	const auto iter = _VtxVars.find(GB_RENDER_VTXVAR_POS);
	if (iter != _VtxVars.end())
	{
		const auto& pos = iter->second;

		_SphereBB = genSphereBB((vec3F*)(pos.data()), pos.count());
	}
}

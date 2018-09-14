#include "Mesh.h"
#include <gbUtils/logger.h>
#include <gbPhysics/type.h>
#include "../Type.h"
using namespace gb::render::data;
using namespace gb::utils;
using namespace gb::physics;
using namespace gb;

Mesh::Mesh() :
	_IdxVar(sizeof(std::uint32_t)),
	_PosVar(nullptr)
{
	const auto ret = _VtxVars.insert(std::make_pair(GB_RENDER_VTXVAR_POS, 3 * sizeof(float)));
	_PosVar = &(ret.first->second);
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
		std::vector<_lua_vec<3>> pos;
		mapper.checkout_tables_by_key(GB_RENDER_VTXVAR_POS, pos);
		_PosVar->append(pos.data(), (GLsizei)(pos.size()));
	}
	else
	{
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_pos key found@ " + mapper.GetData());
		return;
	}


	//idx
	if (mapper.has_key(GB_RENDER_VTXVAR_IDX))
	{
		std::vector<std::uint32_t> idx;
		mapper.checkout_integers_by_key(GB_RENDER_VTXVAR_IDX, idx);
		_IdxVar.append(idx.data(), (GLsizei)(idx.size()));
	}
	else
	{
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_idx key found@ " + mapper.GetData());
		return;
	}


	//uv(optional)
	if (mapper.has_key(GB_RENDER_VTXVAR_UV))
	{
		_VtxVars.insert(std::pair<string, render::GLVar>(GB_RENDER_VTXVAR_UV, mapper.get_tables_by_key<_lua_vec<2>>(GB_RENDER_VTXVAR_UV)));
	}
	_VtxVars.insert(std::pair<string, render::GLVar>(GB_RENDER_VTXVAR_UV, mapper.get_tables_by_key<_lua_vec<2>>(GB_RENDER_VTXVAR_UV)));


	UpdateSphereBB();
}

std::size_t Mesh::GetVtxAttribByteSize() const
{
	std::size_t ret = 0;
	std::for_each(_VtxVars.begin(), _VtxVars.end(), [&ret](const std::pair<const string, render::GLVar>& vtxVar)
	{
		ret += vtxVar.second.byteSize();
	});

	return ret;
}
render::GLVar * Mesh::InitializeAVtxVar(const char * key, const std::size_t unitSize)
{
	const auto ret = _VtxVars.insert(std::make_pair(key, render::GLVar(unitSize)));

	return &(ret.first->second);
}

#include <Eigen/Core/Matrix.h>

void Mesh::UpdateSphereBB()
{
	_SphereBB = genSphereBB((vec3<float>*)(_PosVar->data()), _PosVar->count());
}

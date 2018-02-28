#include "Mesh.h"
#include <gbUtils/logger.h>
#include <gbPhysics/type.h>
#include "../Type.h"
using namespace gb::render::data;
using namespace gb::utils;
using namespace gb::physics;
using namespace gb;

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
		_mpVtxVars.insert(std::pair<const string, GLVar>(GB_RENDER_VTXVAR_POS, mapper.get_tables_by_key<_lua_vec<3>>(GB_RENDER_VTXVAR_POS)));
	else
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_pos key found@ " + mapper.GetFile());

	//idx
	if (mapper.has_key(GB_RENDER_VTXVAR_IDX))
		_mpVtxVars.insert(std::pair<string, GLVar>(GB_RENDER_VTXVAR_IDX, mapper.get_integers_by_key(GB_RENDER_VTXVAR_IDX)));
	else
		logger::Instance().error("Mesh::from_lua no gb_vtxVar_idx key found@ " + mapper.GetFile());

	//uv(optional)
	if(mapper.has_key(GB_RENDER_VTXVAR_UV))
		_mpVtxVars.insert(std::pair<string, GLVar>(GB_RENDER_VTXVAR_UV, mapper.get_tables_by_key<_lua_vec<2>>(GB_RENDER_VTXVAR_UV)));
}

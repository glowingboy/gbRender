#pragma once

#include "DataNS.h"
#include <gbUtils/luatable.h>
#include "../VertexVariable.h"
#include <unordered_map>
#include <gbPhysics/type.h>
GB_RENDER_DATA_NS_BEGIN

class Mesh
{
    void from_lua(const gb::utils::luatable_mapper & mapper);

	GB_PROPERTY_R(private, mpVtxVars, std::unordered_map <const gb::utils::string, vtxVar>);
};

GB_RENDER_DATA_NS_END

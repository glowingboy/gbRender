#pragma once

#include "DataNS.h"
#include <gbUtils/luatable.h>
#include "../GLVariable.h"
#include <unordered_map>
#include <gbPhysics/type.h>
#include <gbPhysics/boundingbox.h>
GB_RENDER_DATA_NS_BEGIN

class Mesh
{
public:
	Mesh();
    void from_lua(const gb::utils::luatable_mapper & mapper);

    GB_PROPERTY_R(private, VtxVars, std::unordered_map <const gb::utils::string, GLVar>);
	GB_PROPERTY_R(private, IdxVar, const GLVar*);
	GB_PROPERTY_R(private, SphereBB, gb::physics::spherebb<>);

	GB_PROPERTY_R(private, VtxAttribByteSize, std::size_t);

};

GB_RENDER_DATA_NS_END

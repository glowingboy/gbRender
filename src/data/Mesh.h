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
	gb::render::GLVar* InitializeAVtxVar(const char* key, const std::size_t unitSize);
    GB_PROPERTY_R(private, VtxVars, std::unordered_map <const gb::utils::string, gb::render::GLVar>);
	GB_PROPERTY_R(private, IdxVar, const gb::render::GLVar*);
	GB_PROPERTY_R(private, SphereBB, gb::physics::spherebb<>);

	GB_PROPERTY_R(private, VtxAttribByteSize, std::size_t);

};

GB_RENDER_DATA_NS_END

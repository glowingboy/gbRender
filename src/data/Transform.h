#pragma once

#include "DataNS.h"
#include <gbUtils/luatable.h>
#include <gbPhysics/type.h>

#define GB_RENDER_DATA_TRANSFORM_KEY_SCALE "Scale"
#define GB_RENDER_DATA_TRANSFROM_KEY_ROTATION "Rotation"
#define GB_RENDER_DATA_TRANSFROM_KEY_POSITION "Position"


GB_RENDER_DATA_NS_BEGIN

struct Transform 
{
	Transform();
	void from_lua(const gb::utils::luatable_mapper & mapper);

	physics::vec3f scale;
	physics::vec3f rotation;
	physics::vec3f position;
};

GB_RENDER_DATA_NS_END

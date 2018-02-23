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
	void from_lua(const gb::utils::luatable_mapper & mapper);
	void operator=(const Transform & other);

	gb::physics::vec3F scale;
	gb::physics::vec3F rotation;
	gb::physics::vec3F position;
};

GB_RENDER_DATA_NS_END

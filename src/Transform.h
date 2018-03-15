#pragma once

#include <gbUtils/common.h>
#include "RenderNS.h"

#include <gbPhysics/matrix.h>

#include "data/Transform.h"

GB_RENDER_NS_BEGIN

class Entity;

class Transform
{
public:
	Transform(Entity* owner);
	void operator=(const gb::render::data::Transform & dataTrans);
public:

	GB_PROPERTY_R(private, LocalTransMat, gb::physics::mat4F);
	GB_PROPERTY_R(private, LocalScale, gb::physics::vec3F);
	GB_PROPERTY_R(private, LocalRotation, gb::physics::vec3F);
private:
	gb::physics::vec4F & _localPosition_Ref;
	Entity* _owner;
private:
	void _updateLocalMatrix();
};

GB_RENDER_NS_END
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
	void SetPosition(const gb::physics::vec3f& newPosition);
	void SetRotation(const physics::vec3f& angle);
	const gb::physics::vec4f& GetLocalPosition() const;

	GB_PROPERTY_R(private, LocalTransMat, gb::physics::mat4f);
	GB_PROPERTY_R(private, LocalScale, gb::physics::vec3f);
	GB_PROPERTY_R(private, LocalRotation, gb::physics::vec3f);
	
private:
	gb::physics::vec4f & _localPosition_Ref;
	Entity* _owner;
private:
	void _updateLocalMatrix();
};

GB_RENDER_NS_END
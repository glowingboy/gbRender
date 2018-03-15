#pragma once
#include "Type.h"
#include "Element.h"
#include <gbPhysics/camera.h>

#define GB_RENDER_CAMERA_DEFAULT_INTERESTLAYER 0xffffffffu

GB_RENDER_NS_BEGIN

class Entity;

class Camera: public Element
{
public:
	Camera(Entity * const owner);
public:
	virtual Type GetType() const override;

	bool operator < (const Camera & o) const;
	void Shoot() const;
	void SetRenderQueue(const uint32 rq);

private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;

private:
	GB_PROPERTY_R(private, RenderQueue, uint32);

	GB_PROPERTY_R(private, InterestLayer, uint32);

	GB_PROPERTY_R(private, Frustum, gb::physics::frustum<gb::physics::Float>);
	//const gb::physics::aabb<>& _frustumAABB;
	//using spherebb rather than aabb, for the reason, when transforming frustumAABB 
	//from camera space to world space, the frustumAABB will not be still an aabb,
	//but the sphere not.
	const gb::physics::spherebb<>& _frustumSphereBB;
	gb::physics::spherebb<> _transformedFSBB;
	const gb::physics::mat4F& _projectionMatrix;

private:
	void _onOwnerTransformChanged();
};

GB_RENDER_NS_END
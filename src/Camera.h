#pragma once
#include "Type.h"
#include "Element.h"
#include <gbPhysics/camera.h>
#include "GLDraw.h"

#define GB_RENDER_CAMERA_DEFAULT_INTERESTTAG 0xffffffffu

#define GB_RENDER_CAMERA_RENDERQUEUE_BACKGROUND 0X00
#define GB_RENDER_CAMERA_RENDERQUEUE_OPAQUE 0xf
#define GB_RENDER_CAMERA_RENDERQUEUE_TRANSPARENT 0xff
#define GB_RENDER_CAMERA_RENDERQUEUE_UI 0x8ff
#define GB_RENDER_CAMERA_RENDERQUEUE_GIZMO 0xfff

GB_RENDER_NS_BEGIN

class Entity;
class Director;

class Camera: public Element
{
	friend class Director;
public:
	Camera(Entity * const owner);
public:
	virtual std::uint32_t GetType() const override;

	bool operator < (const Camera & o) const;
	void Shoot();
	void SetRenderQueue(const uint32 rq);
	void SetIsStatic(const bool isStatic);

	gb::physics::vec3F Screen2World(const gb::physics::vec2F& screenPosition) const;
	
private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;
	void _setFrameBufferIdx(const std::uint8_t idx);
private:
	GB_PROPERTY_R(private, RenderQueue, uint32);

	// bitfiled
	GB_PROPERTY_R(private, InterestTag, uint32);

	GB_PROPERTY_R(private, Frustum, gb::physics::frustum<gb::physics::Float>);
	//gb::physics::frustum<gb::physics::Float> _frustum;
	
	//const gb::physics::aabb<>& _frustumAABB;
	//using spherebb rather than aabb, for the reason, when transforming frustumAABB 
	//from camera space to world space, the frustumAABB will not be still an aabb,
	//but the sphere not.
	const gb::physics::spherebb<>& _frustumSphereBB;
	gb::physics::spherebb<> _transformedFSBB;
	const gb::physics::mat4F& _projectionMatrix;

	GB_PROPERTY_R(private, FrameBufferIdx, std::uint8_t);

	//range[0, 1]
	GB_PROPERTY(private, ViewPort, gb::physics::vec4F);

	const gb::physics::vec2<std::int32_t>& _screenSize;

	GB_PROPERTY(private, ClearColor, gb::physics::vec4F);

	GLMultiIndirectDraw _multiIndirectDraw;

	GB_PROPERTY_R(private, IsStatic, bool);

	const gb::physics::mat4F& _worldMat;

	gb::physics::mat4F _projMatProductViewMat;

	GB_PROPERTY_R(private, ViewRangeEntites, std::vector<Entity*>);
	private:
	void _onOwnerTransformChanged();
};

GB_RENDER_NS_END
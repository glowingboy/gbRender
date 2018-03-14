#include "Camera.h"

#include "Director.h"

using namespace gb::render;

Camera::Camera(Entity * const owner):
	Element(owner),
	_InterestLayer(GB_RENDER_CAMERA_DEFAULT_INTERESTLAYER),
	_frustumAABB(_Frustum.AABB),
	_projectionMatrix(_Frustum.projectionMatrix)
{}

Element::Type Camera::GetType() const
{
	return Element::Type::Camera;
}

bool Camera::operator < (const Camera & o) const
{
	return this->_RenderQueue < o._RenderQueue;
}
void Camera::Awake()
{
	Director::Instance().AddCamera(this);
}
void Camera::Start()
{

}
void Camera::End()
{
	Director::Instance().RemoveCamera(this);
}

void Camera::SetRenderQueue(const uint32 rq)
{
	_RenderQueue = rq;

	//refresh order of camera set 
	Director::Instance().RemoveCamera(this);
	Director::Instance().AddCamera(this);
}

void Camera::Shoot() const
{
	const Director::octreeEntity& renderEntities = Director::Instance().GetRenderEntities();
	auto ret = renderEntities.query_intersect();
}

#include "Camera.h"

#include "Director.h"

using namespace gb::render;

Camera::Camera(Entity * const owner):
	Element(owner)
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

}

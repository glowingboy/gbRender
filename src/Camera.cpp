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

#include "Camera.h"

#include "Director.h"

using namespace gb::render;
using namespace gb::physics;

Camera::Camera(Entity * const owner):
	Element(owner),
	_InterestLayer(GB_RENDER_CAMERA_DEFAULT_INTERESTLAYER),
	_frustumSphereBB(_Frustum.sphereBB),
	_transformedFSBB(_Frustum.sphereBB),
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

	GB_UTILS_CALLBACK_REG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, Camera, _onOwnerTransformChanged);

	//_Owner->GetCBs().RegisterCB(GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, (void*)&Camera::_onOwnerTransformChanged, std::bind(&(Camera::_onOwnerTransformChanged), this));


}
void Camera::Start()
{

}
void Camera::End()
{
	Director::Instance().RemoveCamera(this);
	GB_UTILS_CALLBACK_REG(_Owner->GetCBs)
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
	struct intersectMethod
	{
		bool operator()(const aabb<>& octanBB, const spherebb<>& q) const
		{
			return octanBB.intersect(q);
		}
	};
	auto ret = renderEntities.query_intersect<spherebb<>, intersectMethod>(_transformedFSBB);

	std::for_each(ret.begin(), ret.end(), [](Entity* e)
	{

	});
}

void Camera::_onOwnerTransformChanged()
{

}
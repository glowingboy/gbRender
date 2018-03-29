#include "Camera.h"

#include "Director.h"

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;

Camera::Camera(Entity * const owner) :
	Element(owner),
	_InterestTag(GB_RENDER_CAMERA_DEFAULT_INTERESTTAG),
	_frustumSphereBB(_Frustum.sphereBB),
	_transformedFSBB(_Frustum.sphereBB),
	_projectionMatrix(_Frustum.projectionMatrix),
	_screenSize(Director::Instance().GetScreenSize())
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

	GB_UTILS_CALLBACK_REG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, Camera::_onOwnerTransformChanged);

	//_Owner->GetCBs().RegisterCB(GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, (void*)&Camera::_onOwnerTransformChanged, std::bind(&(Camera::_onOwnerTransformChanged), this));


}
void Camera::Start()
{

}
void Camera::End()
{
	Director::Instance().RemoveCamera(this);
	GB_UTILS_CALLBACK_REG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, Camera::_onOwnerTransformChanged);
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
	//viewport setup
	glViewport((GLint)(_screenSize.x * _ViewPort.x),
		(GLint)(_screenSize.y * _ViewPort.y),
		(GLsizei)(_screenSize.x * _ViewPort.z),
		(GLsizei)(_screenSize.y * _ViewPort.w));

	glClearColor(_ClearColor.r, _ClearColor.g, _ClearColor.b, _ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	const Director::octreeEntity& renderEntities = Director::Instance().GetRenderEntities();
	struct intersectMethod//TODO change to _Ele Type
	{
		bool operator()(const aabb<>& octanBB, const spherebb<>& q) const
		{
			return octanBB.intersect(q);
		}
	};
	auto ret = renderEntities.query_intersect<spherebb<>, intersectMethod>(_transformedFSBB);

	std::for_each(ret.begin(), ret.end(), [this](Entity* e)
	{
		if (_InterestTag & e->GetTag())
		{

		}
	});
}

void Camera::_onOwnerTransformChanged()
{
	logger::Instance().log("Camera::_onOwnerTransformChanged");

	_transformedFSBB = _frustumSphereBB * _Owner->GetWorldTransformMatrix();
}

void Camera::_setFrameBufferIdx(const std::uint8_t idx)
{
	_FrameBufferIdx = idx;
}
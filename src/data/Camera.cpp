#include "Camera.h"
#include "../Camera.h"
#include <gbUtils/logger.h>

using namespace gb::utils;
using namespace gb::render::data;
using namespace gb;
using namespace gb::physics;
Camera::Camera() :
	Element(render::Element::Type::Camera),
	_RenderQueue(GB_RENDER_DATA_SHADER_RENDERQUEUE_OPAQUE)
{
}
void Camera::from_lua(const gb::utils::luatable_mapper & mapper)
{
	_RenderQueue = mapper.get_integer_by_key(GB_RENDER_DATA_CAMERA_KEY_RENDERQUEUE);
	
	if (mapper.has_key(GB_RENDER_DATA_CAMERA_KEY_CLEARCOLOR))
		_ClearColor = mapper.get_numbers_by_key(GB_RENDER_DATA_CAMERA_KEY_CLEARCOLOR);

	mapper.checkout_numbers_by_key(GB_RENDER_DATA_CAMERA_KEY_FRUSTUM, _Frustum);
}
gb::render::Element* Camera::Instantiate(gb::render::Entity* owner) const
{
	render::Camera* cam = new render::Camera(owner);

	cam->SetRenderQueue(_RenderQueue);
	cam->SetClearColor(_ClearColor);

	if (_Frustum.size() != 0)
	{
		if (_Frustum.size() == 4) // perspective projection
			cam->GetFrustum().set(_Frustum[0], _Frustum[1], _Frustum[2], _Frustum[3]);
		else if (_Frustum.size() == 6) // orthographic projection
			cam->GetFrustum().set(_Frustum[0], _Frustum[1], _Frustum[2], _Frustum[3], _Frustum[4], _Frustum[5]);
		else
			logger::Instance().error("Camera::Instantiate, Frustum can only has 4 or 6 count of arguments ");
	}

	return cam;
}
#include "Camera.h"
#include "../Camera.h"

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
}
gb::render::Element* Camera::Instantiate(gb::render::Entity* const owner) const
{
	render::Camera* cam = new render::Camera(owner);

	cam->SetRenderQueue(_RenderQueue);
	cam->SetClearColor(_ClearColor);
	return cam;
}
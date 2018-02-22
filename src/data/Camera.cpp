#include "Camera.h"
#include "../Camera.h"

using namespace gb::render::data;
using namespace gb;

Camera::Camera() :
	Element(render::Element::Type::Camera)
{

}
void Camera::from_lua(const gb::utils::luatable_mapper & mapper)
{
	_RenderQueue = mapper.get_integer_by_key(GB_RENDER_DATA_CAMERA_KEY_RENDERQUEUE);

}
gb::render::Element* Camera::Instantiate(gb::render::Entity* const owner) const
{
	render::Camera* cam = new render::Camera(owner);

	cam->SetRenderQueue(_RenderQueue);

	return cam;
}
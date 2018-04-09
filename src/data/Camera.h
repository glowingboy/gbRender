#pragma once

#include "Element.h"
#include <gbPhysics/matrix.h>
#define GB_RENDER_DATA_CAMERA_KEY_RENDERQUEUE "RenderQueue"
#define GB_RENDER_DATA_CAMERA_KEY_CLEARCOLOR "ClearColor"
GB_RENDER_DATA_NS_BEGIN

class Camera : public Element
{
public:
	Camera();
public:
	virtual void from_lua(const gb::utils::luatable_mapper & mapper) override;
	virtual gb::render::Element* Instantiate(gb::render::Entity* const owner) const override;
	
	GB_PROPERTY_R(private, RenderQueue, uint32);
	GB_PROPERTY_R(private, ClearColor, gb::physics::vec4F);
};

GB_RENDER_DATA_NS_END
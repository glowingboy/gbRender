#pragma once

#include "Element.h"
#include <gbUtils/callback.h>
#include <gbPhysics/ray.h>
GB_RENDER_NS_BEGIN

class Camera;

class EntityPicker : public Element
{
public:
	EntityPicker(Entity* owner);

	virtual std::uint32_t GetType() const override;

private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;

private:
	GB_PROPERTY_R(private, CBs, gb::utils::callback<gb::physics::vec3f>);
	GB_PROPERTY_R(private, Ray, gb::physics::ray<float>);

	GB_PROPERTY_W(private, Camera, const Camera*);
private:
	void OnClicked();
};

GB_RENDER_NS_END
#pragma once
#include "Type.h"
#include "Element.h"

GB_RENDER_NS_BEGIN

class Camera: public Element
{
public:
	Camera(Entity * const owner);
public:
	virtual Type GetType() const override;

	bool operator < (const Camera & o) const;
	void Shoot() const;
	void SetRenderQueue(const uint32 rq);
private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;
	GB_PROPERTY_R(private, RenderQueue, uint32);
};

GB_RENDER_NS_END
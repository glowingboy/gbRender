#pragma once

#include "Element.h"

#define GB_RENDER_INPUT_DEFAULT_POSITION_MOVE_STEP 1

GB_RENDER_NS_BEGIN

class FPVController : public Element
{
public:
	FPVController(Entity* owner);

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;
	virtual std::uint32_t GetType() const override;

private:
	void OnInput(const char code, const bool down);

	GB_PROPERTY(private, PosStep, std::int32_t);
};

GB_RENDER_NS_END
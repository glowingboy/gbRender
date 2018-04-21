#pragma once
#include <Element.h>
#include "NS.h"

GB_EDITOR_NS_BEGIN

class Main : public gb::render::Element
{
public:
	Main(gb::render::Entity* owner);

private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;
	virtual std::uint32_t GetType() const;
};

GB_EDITOR_NS_END
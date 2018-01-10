#pragma once 
#include "GBObject.h"
#include "Config.h"
#include "RenderNS.h"
#include <gbUtils/string.h>
#include "data/Entity.h"

GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Entity: protected GBObject
{
public:
	void Instantiate(gb::render::data::Entity& dEntity);
private:
	virtual void Awake() override;
	virtual void Start() override;
private:
	GB_PROPERTY(gb::utils::string, Name);
	std::unordered_map<gb::utils::string, Entity*> _mpChildren;
};

GB_RENDER_NS_END
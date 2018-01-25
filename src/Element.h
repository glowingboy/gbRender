#pragma once
#include "RenderNS.h"
#include <gbUtils/common.h>

GB_RENDER_NS_BEGIN

class Entity;

class Element
{
public:
	enum Type
	{
		Default = 0, 
		Render, Mesh, Text
	};
protected:
	Element(Entity* const owner);
	friend class Entity;

public:
	virtual Type GetType() const = 0;
private:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	
	GB_PROPERTY_R(protected, Owner, Entity*);
};

GB_RENDER_NS_END
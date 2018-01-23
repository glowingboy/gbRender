#pragma once
#include "RenderNS.h"

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
	friend class Entity;
private:
	virtual void Awake() = 0;
	virtual void Start() = 0;
};

GB_RENDER_NS_END
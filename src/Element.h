#pragma once
#include "GBObject.h"
#include <gbUtils/string.h>
GB_RENDER_NS_BEGIN

class Entity;

class Element: public GBObject
{
public:
	enum Type
	{
		Default = 0, 
		Camera, Render, Mesh, Text
	};
	inline static gb::utils::string TypeToString(const Type t)
	{
		if (t == Type::Render)
			return gb::utils::string("Render");
		else if (t == Type::Text)
			return gb::utils::string("Text");
		else
			return gb::utils::string("Unkown");
	}
protected:
	Element(Entity* const owner);
	friend class Entity;

public:
	virtual Type GetType() const = 0;
private:
	virtual void Awake() = 0;
	
	GB_PROPERTY_R(protected, Owner, Entity*);
};

GB_RENDER_NS_END
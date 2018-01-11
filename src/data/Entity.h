#pragma once
#include "DataNS.h"
#include <gbUtils/luatable.h>
#include <gbUtils/string.h>
#include <unordered_map>

#define GB_RENDER_DATA_ENTITY_KEY_NAME "Name"
#define GB_RENDER_DATA_ENTITY_KEY_COMPONENTS "Components"
#define GB_RENDER_DATA_ENTITY_KEY_CHILDREN "Children"
GB_RENDER_DATA_NS_BEGIN

class Entity
{
public:
	inline Entity(){}
	Entity(Entity && o);
	~Entity();
public:
	void from_lua(const gb::utils::luatable_mapper& mapper);

	GB_PROPERTY_R(Name, gb::utils::string);

private:
	GB_PROPERTY_R(Children, std::unordered_multimap<const gb::utils::string, Entity*>);
};

GB_RENDER_DATA_NS_END
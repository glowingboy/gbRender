#include "Entity.h"
#include <algorithm>

using namespace gb::render::data;
using namespace gb::utils;
 
Entity::Entity(Entity && o) :
	_Name(std::move(o._Name)),
	_Children(std::move(o._Children))
{

}
Entity::~Entity()
{
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<string, Entity*> e)
	{
		GB_SAFE_DELETE(e.second);
	});
}


void Entity::from_lua(const luatable_mapper& mapper)
{
	_Name = mapper.get_string_by_key(GB_RENDER_DATA_ENTITY_KEY_NAME);

	mapper.for_each_in([this, &mapper](const size_t idx)
	{
		Entity* e = new Entity(mapper.get_table_by_idx<Entity>(idx));
		_Children.insert(std::pair<string, Entity*>(e->GetName(), e));
	}, GB_RENDER_DATA_ENTITY_KEY_CHILDREN);


}

#include "Entity.h"
#include <algorithm>
#include <gbUtils/logger.h>
#include "..\Entity.h"
using namespace gb::render::data;
using namespace gb::utils;
 
Entity::Entity(Entity && o) :
	_Name(std::move(o._Name)),
	_Transform(o._Transform),
	_Children(std::move(o._Children)),
	_Elements(std::move(o._Elements))
{

}
Entity::~Entity()
{
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<const string, Entity*> e)
	{
		GB_SAFE_DELETE(e.second);
	});
}


void Entity::from_lua(const luatable_mapper& mapper)
{
	//name
	if(mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_NAME))
		_Name = mapper.get_string_by_key(GB_RENDER_DATA_ENTITY_KEY_NAME);
	else
	{
		logger::Instance().error(string("Entity::from_lua broken entity mapper@ ") + mapper.GetData());
		return;
	}

	//transform
	mapper.checkout_table_by_key(GB_RENDER_DATA_ENTITY_KEY_TRANSFORM, _Transform);

	//elements
	if(mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_ELEMENTS))
		mapper.for_each_in([this, &mapper](const size_t idx)
		{
			Element* ele = mapper.get_table_by_idx<ElementAdapter>(idx).GetElement();
			if(ele != nullptr)
				_Elements.insert(std::make_pair(ele->GetType(), ele));
		}, GB_RENDER_DATA_ENTITY_KEY_ELEMENTS);

	//children
	if(mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_CHILDREN))
		mapper.for_each_in([this, &mapper](const size_t idx)
		{
			Entity* e = new Entity(mapper.get_table_by_idx<Entity>(idx));
			_Children.insert(std::pair<string, Entity*>(e->GetName(), e));
		}, GB_RENDER_DATA_ENTITY_KEY_CHILDREN);


}

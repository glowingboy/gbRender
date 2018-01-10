#include "Entity.h"
#include <algorithm>

using namespace gb::render;

void Entity::Instantiate(data::Entity& dEntity)
{
	_Name = dEntity.GetName();

	std::for_each(dEntity.GetName)
}
void Entity::Awake()
{

}

void Entity::Start()
{

}
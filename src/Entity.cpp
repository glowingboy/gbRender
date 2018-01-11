#include "Entity.h"
#include <algorithm>
#include <gbUtils/logger.h>
using namespace gb::render;
using namespace gb::utils;

Entity::~Entity()
{
	std::for_each(_mpChildren.begin(), _mpChildren.end(), [](std::pair<const string, Entity*>& e)
	{
		GB_SAFE_DELETE(e.second);
	});
}

template<typename DataEntity>
void Entity::Instantiate(DataEntity && dEntity)
{
	_Name = std::forward<DataEntity>(dEntity.GetName());

	std::for_each(dEntity.GetChildren().begin(), dEntity.GetChildren().end(), [this](typename DataEntity::value_type& dE)
	{
		Entity* e = new Entity;
		e->Instantiate<DataEntity>(*(dE.second));
		_mpChildren.insert(std::pair<string, Entity*>(e->GetName(), e));
	});
}

void Entity::Instantiate(const char* entityFile)
{
	GB_ASSERT(entityFile != nullptr);

	luatable_mapper mapper(entityFile, luastate_mgr::Instance().getconfig_state());
	if (mapper.validate())
	{
		data::Entity e;
		e.from_lua(mapper);
		Instantiate(e);
	}
	else
		logger::Instance().error(string("Entity::Instantiate mapper validate failed entityFile@ ") + entityFile);
}
void Entity::Awake()
{

}

void Entity::Start()
{

}
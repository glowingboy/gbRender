#include "Entity.h"
#include <algorithm>
#include <gbUtils/logger.h>
using namespace gb::render;
using namespace gb::utils;

Entity::Entity()
{

}
Entity::~Entity()
{
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<const string, Entity*>& e)
	{
		GB_SAFE_DELETE(e.second);
	});
}

template<typename DataEntity>
typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value, void>
::type Entity::Instantiate(DataEntity && dEntity)
{
	_instantiate(std::forward<DataEntity>(dEntity));

	//self Start
	this->Start();
}

template void Entity::Instantiate<const data::Entity & >(const data::Entity & dEntity);
template void Entity::Instantiate<data::Entity &&>(data::Entity && dEntity);

template<typename DataEntity>
typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value, void>
::type Entity::_instantiate(DataEntity && dEntity)
{

	_Name = std::forward<DataEntity>(dEntity).GetName();

	_Transform = dEntity.GetTransform();

	//elements instantiate 
	std::for_each(dEntity.GetElements().begin(), dEntity.GetElements().end(), [this](std::conditional
		<std::is_const<std::remove_reference<DataEntity>::type>::value, const std::pair<const Element::Type, data::Element*>, std::pair<const Element::Type, data::Element*>>
		::type & dE)
	{
		Element* ele = dE.second->Instantiate(this);
		ele->Awake();
		_Elements.insert(std::pair<const Element::Type, Element*>(ele->GetType(), ele));
	});
	//children instantiate
	std::for_each(dEntity.GetChildren().begin(), dEntity.GetChildren().end(), [this](std::conditional
		<std::is_const<std::remove_reference<DataEntity>::type>::value, const std::pair<const string, data::Entity*>, std::pair<const string, data::Entity*>>
		::type & dE)
	{
		Entity* e = new Entity;
		e->_instantiate(std::forward<DataEntity>(*(dE.second)));
		_Children.insert(std::pair<const string, Entity*>(e->GetName(), e));
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
		Instantiate(std::move(e));
	}
	else
		logger::Instance().error(string("Entity::Instantiate mapper validate failed entityFile@ ") + entityFile);
}

void Entity::Start()
{
	//self Start
	logger::Instance().log(string("Start from ") + _Name);

	//self elements Start
	std::for_each(_Elements.begin(), _Elements.end(), [](std::pair<const Element::Type, Element*> & ele)
	{
		ele.second->Start();
	});

	//children Start
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<const string, Entity*> & e)
	{
		e.second->Start();
	});

}

void Entity::End()
{
	logger::Instance().log(string("End from ") + _Name);
}

void Entity::AddElement(Element* const ele)
{
	const Element::Type t = ele->GetType();

	if (_Elements.find(t) != _Elements.end())
	{
		_Elements.insert(std::pair<const Element::Type, Element*>(t, ele));
	}
	else
		logger::Instance().warning("more than one count of same type Element @ " + Element::TypeToString(t));
	

}
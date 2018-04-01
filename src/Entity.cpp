#include "Entity.h"
#include <algorithm>
#include <gbUtils/logger.h>
#include <queue>
using namespace gb::render;
using namespace gb::utils;
using namespace gb::physics;

Entity::Entity(Entity* const parent):
	_Transform(this),
	_Parent(parent),
	_Tag(GB_RENDER_ENTITY_TAG_DEFAULT)
{

}
Entity::~Entity()
{
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<const string, Entity*>& e)
	{
		GB_SAFE_DELETE(e.second);
	});
}

void Entity::Instantiate(const data::Entity* dEntity)
{
	_instantiate(dEntity);

	//self Start
	this->Start();
}

void Entity::_instantiate(const data::Entity* dEntity)
{

	_Name = dEntity->GetName();

	_Transform = dEntity->GetTransform();

	//elements instantiate 
	std::for_each(dEntity->GetElements().begin(), dEntity->GetElements().end(), [this](const std::pair<const Element::Type, data::Element*> & dE)
	{
		Element* ele = dE.second->Instantiate(this);
		ele->Awake();
		_Elements.insert(std::pair<const Element::Type, Element*>(ele->GetType(), ele));
	});
	//children instantiate
	std::for_each(dEntity->GetChildren().begin(), dEntity->GetChildren().end(), [this](const std::pair<const string, data::Entity*>& dE)
	{
		Entity* e = new Entity(this);
		e->_instantiate(dE.second);
		_Children.insert(std::pair<const string, Entity*>(e->GetName(), e));
	});

}


void Entity::Instantiate(const char* entityFile)
{
	GB_ASSERT(entityFile != nullptr);

	Instantiate(resource::Res<data::Entity>::Instance().Get(entityFile));
}

void Entity::Start()
{

	_updateWorldTransform();
	

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

void Entity::_updateWorldTransform()
{
	std::deque<const Entity*> qParents;

	qParents.push_back(this);

	const Entity* parent = _Parent;

	while (parent)
	{
		qParents.push_back(parent);

		parent = parent->_Parent;
	}

	_WorldTransformMatrix = mat4F::make_identity();

	std::for_each(qParents.rbegin(), qParents.rend(), [this](const Entity*& e)
	{
		_WorldTransformMatrix *= e->GetTransform().GetLocalTransMat();
	});

	_CBs.Trigger(GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED);

}

void Entity::_setRender(gb::render::Render* const render)
{
	_Render = render;
}

//void Entity::RegisterCB(const uint32 triggerKey, const std::uintptr_t cbAddr, const std::function<void(void)>& cb)
//{
//	auto triggerItr = _mpCBs.find(triggerKey);
//	if (triggerItr != _mpCBs.end())
//	{
//		triggerItr->second.insert(std::make_pair(cbAddr, cb));
//	}
//	else
//		_mpCBs.insert(std::make_pair(triggerKey, std::unordered_map<std::uintptr_t, std::function<void(void)>>{ {cbAddr, cb}}));
//}
//
//void Entity::UnregisterCB(const uint32 triggerKey, const std::uintptr_t cbAddr)
//{
//	auto triggerItr = _mpCBs.find(triggerKey);
//	if (triggerItr != _mpCBs.end())
//	{
//		triggerItr->second.erase(cbAddr);
//	}
//}
//
//void Entity::Trigger(const uint32 triggerKey)
//{
//	auto triggerItr = _mpCBs.find(triggerKey);
//	if (triggerItr != _mpCBs.end())
//	{
//		const std::unordered_map<std::uintptr_t, std::function<void(void)>>& cbs = triggerItr->second;
//
//		std::for_each(cbs.begin(), cbs.end(), [](const std::pair<std::uintptr_t, std::function<void(void)>>& cb)
//		{
//			cb.second();
//		});
//	}
//}


bool Entity::octreeSBBContain::operator()(const Entity* entity, const gb::physics::aabb<>& o) const
{
	Render* render = entity->GetRender();
	if (render != nullptr)
	{
		return o.contain(render->GetTransformedSphereBB());
	}

	logger::Instance().error("Entity::octreeSBBContain::operator() render is nullptr@ " + entity->GetName());

	return false;
}


static const vec3F __error__apg__('e', 'r', 'r');
const vec3F & Entity::octreeSBBAPG::operator()(const Entity* entity) const
{
	Render* render = entity->GetRender();
	if (render != nullptr)
	{
		return render->GetTransformedSphereBB().centre;
	}

	logger::Instance().error("Entity::octreeSBBAPG::operator() render is nullptr@ " + entity->GetName());

	return __error__apg__;
}

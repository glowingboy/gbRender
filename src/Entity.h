#pragma once 
#include "Element.h"
#include "Config.h"
#include <gbUtils/string.h>
#include "resource/Resource.h"
#include "data/Entity.h"
#include "Render.h"
#include <gbPhysics/boundingbox.h>
#include "Transform.h"
#include <gbUtils/callback.h>

#define GB_RENDER_ENTITY_LAYER_DEFAULT 0x1u

#define GB_RENDER_ENTITY_LAYER_UI 0x1u << 1

#define GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED 1

namespace gb
{
	namespace render
	{
		class Director;
	}
}
GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Entity: public GBObject
{
	friend class gb::render::Director;
	friend class gb::render::Render;
	friend class gb::render::Transform;
private:
	Entity(Entity* parent = nullptr);
	~Entity();
public:
	template<typename DataEntity>
	typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value , void>
		::type Instantiate(DataEntity && dEntity);
	void Instantiate(const char* entityFile);

	void AddElement(Element* const ele);

	//void RegisterCB(const uint32 triggerKey, const std::uintptr_t cbAddr, const std::function<void(void)>& cb);
	//void UnregisterCB(const uint32 triggerKey, const std::uintptr_t cbAddr);
	//void Trigger(const uint32 triggerKey);
private:
	virtual void Start() override;
	virtual void End() override;

	template<typename DataEntity>
	typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value, void>
		::type _instantiate(DataEntity && dEntity);
	GB_PROPERTY(private, Name, gb::utils::string);
	GB_PROPERTY_R(private, Children, std::unordered_multimap<const gb::utils::string, Entity*>);
	GB_PROPERTY_R(private, Elements, std::map<const gb::render::Element::Type, Element*>);
	GB_PROPERTY_R(private, Transform, Transform);

	GB_PROPERTY_R(private, WorldTransformMatrix, gb::physics::mat4F);
	GB_PROPERTY_R(private, Parent, Entity*);

	GB_PROPERTY_R(private, Render, gb::render::Render*);

	GB_PROPERTY_R(private, Layer, uint32);

	//std::unordered_map<uint32, std::unordered_map<std::uintptr_t, std::function<void(void)>>> _mpCBs;

	GB_PROPERTY_R(private, CBs, gb::utils::callback<>);

private:
	void _updateWorldTransform();
	void _setRender(gb::render::Render* const render);


public:
	struct octreeSBBContain
	{
		bool operator()(const Entity* entity, const gb::physics::aabb<>& o) const;
	};

	struct octreeSBBAPG
	{
		const gb::physics::vec3F & operator()(const Entity* entity) const;
	};

};

GB_RENDER_NS_END
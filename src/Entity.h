#pragma once 
#include "Element.h"
#include "Config.h"
#include <gbUtils/string.h>
#include "resource/Resource.h"
#include "data/Entity.h"
#include "Render.h"
#include <gbPhysics/boundingbox.h>
#include "Transform.h"

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
private:
	Entity();
	~Entity();
public:
	template<typename DataEntity>
	typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value , void>
		::type Instantiate(DataEntity && dEntity);
	void Instantiate(const char* entityFile);

	void AddElement(Element* const ele);
private:
	virtual void Start() override;
	virtual void End() override;

	template<typename DataEntity>
	typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value, void>
		::type _instantiate(DataEntity && dEntity);
	GB_PROPERTY(private, Name, gb::utils::string);
	GB_PROPERTY_R(private, Children, std::unordered_multimap<const gb::utils::string, Entity*>);
	GB_PROPERTY_R(private, Elements, std::map<const gb::render::Element::Type, Element*>);
	GB_PROPERTY_R(private, SBB, gb::physics::spherebb<>*);
	GB_PROPERTY_R(private, Transform, Transform);
};

GB_RENDER_NS_END
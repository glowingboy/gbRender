#pragma once 
#include "GBObject.h"
#include "Config.h"
#include "RenderNS.h"
#include <gbUtils/string.h>
#include "data/Entity.h"

namespace gb
{
	namespace render
	{
		class Director;
	}
}
GB_RENDER_NS_BEGIN

GB_RENDER_CLASS Entity: protected GBObject
{
	friend class gb::render::Director;
private:
	inline Entity() {};
	~Entity();
public:
	template<typename DataEntity>
	typename std::enable_if<data::Entity::is_entity<typename gb::rm_cv_ref<DataEntity>::type>::value , void>
		::type Instantiate(DataEntity && dEntity);
	void Instantiate(const char* entityFile);
private:
	virtual void Awake() override;
	virtual void Start() override;
private:
	GB_PROPERTY(Name, gb::utils::string)
	std::unordered_map<const gb::utils::string, Entity*> _mpChildren;
};

GB_RENDER_NS_END
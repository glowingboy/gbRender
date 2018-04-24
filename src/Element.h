#pragma once
#include "GBObject.h"
#include <gbUtils/string.h>
GB_RENDER_NS_BEGIN

class Element: public GBObject
{
public:
	struct Type
	{
		static constexpr std::uint32_t Default = 0x00;
		static constexpr std::uint32_t Camera = 0x01;
		static constexpr std::uint32_t Render = 0x02;
		static constexpr std::uint32_t Mesh = 0x03;
		static constexpr std::uint32_t Texture = 0x04;
		static constexpr std::uint32_t Text = 0x05;
		static constexpr std::uint32_t FPVController = 0x06;
		static constexpr std::uint32_t EntityPicker = 0x07;
	};
	inline static gb::utils::string TypeToString(const std::uint32_t t)
	{
		if (t == Type::Render)
			return gb::utils::string("Render");
		else if (t == Type::Text)
			return gb::utils::string("Text");
		else
			return gb::utils::string("Unkown");
	}
protected:
	Element(Entity* const owner);
	friend class Entity;

public:
	virtual std::uint32_t GetType() const = 0;
private:
	virtual void Awake() = 0;
	
	GB_PROPERTY_R(protected, Owner, Entity*);
};

GB_RENDER_NS_END
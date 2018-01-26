#pragma once
#include <gbUtils/common.h>
#include "Type.h"

#include <gbUtils/string.h>
#include "Config.h"
#include <gbPhysics/algorithm.h>

#include "Device.h"

#include "Entity.h"

// utils
#include <gbUtils/logger.h>
#include <gbUtils/filesystem.h>

#include <set>
#include "Camera.h"

GB_RENDER_NS_BEGIN
GB_RENDER_CLASS Director
{
	friend class Camera;
public:
	struct Argument
	{
		Argument(const char* fileOfRootEntity, const gb::algorithm::vec2<gb::render::uint32>& sizeOfScreen);
		Argument(Argument && other);
		gb::utils::string rootEntity;
		gb::algorithm::vec2<gb::render::uint32> screenSize;
	};
	GB_SINGLETON(Director);
public:
	bool Ready(const Argument & arg);
	void Action();
private:
	void AddCamera(Camera * const cam);
	bool _directing();
private:
	std::set<Camera*, gb::utils::less_ptr<Camera>> _Cameras;
	GB_PROPERTY_R(private, Root, Entity);
};
GB_RENDER_NS_END

#pragma once
#include <gbUtils/common.h>
#include "Type.h"

#include <gbUtils/string.h>
#include "Config.h"
#include <gbPhysics/type.h>
#include <gbPhysics/sptree.h>

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
		Argument(const char* fileOfRootEntity, const gb::physics::vec2<gb::render::uint32>& sizeOfScreen);
		Argument(Argument && other);
		gb::utils::string rootEntity;
		gb::physics::vec2<gb::render::uint32> screenSize;
	};
	GB_SINGLETON(Director);
public:
	bool Ready(const Argument & arg);
	void Action();
private:
	void AddCamera(const Camera * const cam);
	void RemoveCamera(const Camera* const cam);
	bool _directing();
private:
	typedef std::set<const Camera *, gb::utils::less_ptr<Camera>>::iterator _camItr;
	std::set<const Camera *, gb::utils::less_ptr<Camera>> _Cameras;
	GB_PROPERTY_R(private, Root, Entity);
	GB_PROPERTY_R(private, RenderEntities, gb::physics::octree<)
};
GB_RENDER_NS_END

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
#include <queue>
#include "Camera.h"

#define GB_RENDER_DIRECTOR_SCENE_SIZE 100u

#define GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT 5u
//#define GB_RENDER_DIRECTOR_CAMERA_TEXTURE_LEVEL 6

static_assert(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT > 0, "GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT must greater than 0");

GB_RENDER_NS_BEGIN
GB_RENDER_CLASS Director
{
	friend class Camera;
	friend class Render;

	~Director();
public:
	struct Argument
	{
		Argument(const char* fileOfRootEntity, const gb::physics::vec2<gb::render::uint32>& sizeOfScreen, const std::map<gb::utils::string, gb::utils::string>& resCfg);
		Argument(Argument && other);
		gb::utils::string rootEntity;
		gb::physics::vec2<gb::render::uint32> screenSize;
		std::map<gb::utils::string, gb::utils::string> resCfgs;
	};
	GB_SINGLETON_NO_CTORDEF(Director);
public:
	bool Ready(const Argument & arg);
	void Action();
private:
	void AddCamera(Camera * const cam);
	void RemoveCamera(Camera* const cam);

	void AddRenderEntity(Entity* const entity);
	void RemoveRenderEntity(Entity* const entity);
	//void
	bool _directing();
private:
	typedef std::set<const Camera *, gb::utils::less_ptr<Camera>>::iterator _camItr;
	std::set<const Camera *, gb::utils::less_ptr<Camera>> _Cameras;
	GB_PROPERTY_R(private, Root, Entity);
	GB_PROPERTY_R(private, ScreenSize, gb::physics::vec2<gb::render::uint32>);
	//GB_PROPERTY_R(private, RenderEntities, gb::physics::octree<Entity*>)

	GLuint _frameBuffers[GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT];
	GLuint _cameraTextures;
	GLuint _cameraDepthBuffers[GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT];
	//GLuint _cameraTexIndices[GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT];
	std::queue<GLuint> _cameraFBOIndices;

	GB_PROPERTY(private, ClearColor, gb::physics::vec4F);

	GLInstancedDraw _screenDraw;
	data::Material* _screenMat;
	std::uint32_t _instVar[GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT];

public:
	typedef gb::physics::octree<Entity*, Entity::octreeSBBContain, Entity::octreeSBBAPG, gb::physics::Float> octreeEntity;

	/*
		in world space
	*/
	GB_PROPERTY_R(private, RenderEntities, octreeEntity);
};
GB_RENDER_NS_END

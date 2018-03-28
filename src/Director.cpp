#include "Director.h"
#include "Device.h"
#include <gbUtils/luastate.h>

using namespace gb::render;
using namespace gb::utils;
using namespace gb::physics;

Director::Argument::Argument(const char* fileOfRootEntity, const gb::physics::vec2<gb::render::uint32>& sizeOfScreen):
	rootEntity(fileOfRootEntity),
	screenSize(sizeOfScreen)
{

}

Director::Argument::Argument(Argument && other) :
	rootEntity(std::move(other.rootEntity)),
	screenSize(other.screenSize)
{
}

Director::Director():
	_RenderEntities(aabb<>(vec3F(-GB_RENDER_DIRECTOR_SCENE_SIZE, -GB_RENDER_DIRECTOR_SCENE_SIZE, -GB_RENDER_DIRECTOR_SCENE_SIZE),
		vec3F(GB_RENDER_DIRECTOR_SCENE_SIZE, GB_RENDER_DIRECTOR_SCENE_SIZE, GB_RENDER_DIRECTOR_SCENE_SIZE))),
	_frameBuffers{0},
	_cameraTextures{0}
{
	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		_cameraTexIndices.push(i);
	}
}
bool Director::Ready(const Argument& arg)
{
	if (!Device::Instance().Initialize(arg.screenSize))
	{
		logger::Instance().error("Director::Ready Device has not been Initialized");
		return false;
	}

	_ScreenSize = arg.screenSize;
	
	if (arg.rootEntity.length() != 0)
		_Root.Instantiate(arg.rootEntity);

	//framebuffer setup
	glGenFramebuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _frameBuffers);

	glGenTextures(1, &_cameraTextures);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _cameraTextures);
	glTextureStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _ScreenSize.x, _ScreenSize.y, GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffers[i]);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _cameraTextures, 0, i);


	}



	return true;
}

void Director::Action()
{
//	try
//	{
		for (;;)
		{
			if (!_directing())
				break;

		}
//	}
	//catch (const string& error)
	//{
	//	logger::Instance().error("director::Action quit@" + error);
	//}
	//catch (...)
	//{
	//	logger::Instance().error("director::Action quit unknown error");
	//}
}

bool Director::_directing()
{
	//camera shooting
	std::for_each(_Cameras.begin(), _Cameras.end(), [](const Camera* const & cam)
	{
		cam->Shoot();
	});

	return Device::Instance().Update();
}

void Director::AddCamera(const Camera * const cam)
{
	_Cameras.insert(cam);
}

void Director::RemoveCamera(const Camera* const cam)
{
	_camItr i = _Cameras.find(cam);
	if (i != _Cameras.end())
		_Cameras.erase(i);
}

void Director::AddRenderEntity(Entity* const entity)
{
	_RenderEntities.insert(entity);
}
void Director::RemoveRenderEntity(Entity* const entity)
{
	_RenderEntities.remove(entity);
}
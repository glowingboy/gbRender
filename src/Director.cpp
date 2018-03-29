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
	_RenderEntities(aabb<>(vec3F(-(signed)GB_RENDER_DIRECTOR_SCENE_SIZE, -(signed)GB_RENDER_DIRECTOR_SCENE_SIZE, -(signed)GB_RENDER_DIRECTOR_SCENE_SIZE),
		vec3F(GB_RENDER_DIRECTOR_SCENE_SIZE, GB_RENDER_DIRECTOR_SCENE_SIZE, GB_RENDER_DIRECTOR_SCENE_SIZE))),
	_frameBuffers{0},
	_cameraTextures{0},
	_cameraDepthBuffers{0},
	_ClearColor{0, 0, 0, 0}
{
	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		_cameraFBOIndices.push(i);
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

	//texture
	glGenTextures(1, &_cameraTextures);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _cameraTextures);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _ScreenSize.x, _ScreenSize.y, GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	//depth
	glGenRenderbuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _cameraDepthBuffers);

	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffers[i]);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _cameraTextures, 0, i);

		//depth buffer
		glBindRenderbuffer(GL_RENDERBUFFER, _cameraDepthBuffers[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _ScreenSize.x, _ScreenSize.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _cameraDepthBuffers[i]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			logger::Instance().error("glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE");
			return false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	//clear
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, _ScreenSize.x, _ScreenSize.y);
	glClearColor(_ClearColor.r, _ClearColor.g, _ClearColor.b, _ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	//camera shooting
	std::for_each(_Cameras.begin(), _Cameras.end(), [this](const Camera* const cam)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffers[cam->GetFrameBufferIdx()]);
		cam->Shoot();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _ScreenSize.x, _ScreenSize.y);
	});

	return Device::Instance().Update();
}

void Director::AddCamera(Camera * const cam)
{
	if (_cameraFBOIndices.size() == 0)
	{
		logger::Instance().error("_cameraFBOIndices.size() == 0");
		return;
	}
	_Cameras.insert(cam);

	cam->_setFrameBufferIdx(_cameraFBOIndices.front());
	_cameraFBOIndices.pop();
}

void Director::RemoveCamera(Camera* const cam)
{
	_camItr i = _Cameras.find(cam);
	if (i != _Cameras.end())
	{
		_Cameras.erase(i);

		_cameraFBOIndices.push(cam->GetFrameBufferIdx());
	}
		
}

void Director::AddRenderEntity(Entity* const entity)
{
	_RenderEntities.insert(entity);
}
void Director::RemoveRenderEntity(Entity* const entity)
{
	_RenderEntities.remove(entity);
}
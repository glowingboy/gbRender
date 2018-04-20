#include "Director.h"
#include "Device.h"
#include <gbUtils/luastate.h>

using namespace gb::render;
using namespace gb::utils;
using namespace gb::physics;

Director::Argument::Argument(const char* fileOfRootEntity, const gb::physics::vec2<gb::render::uint32>& sizeOfScreen, const std::map<gb::utils::string, gb::utils::string>& resCfg):
	rootEntity(fileOfRootEntity),
	screenSize(sizeOfScreen),
	resCfgs(resCfg)
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
	//_cameraTextures{0},
	_cameraDepthBuffers{0},
	_ClearColor{0, 0, 0, 0},
	_instVar{0}
{
	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		_cameraFBOIndices.push(i);
	}
}

Director::~Director()
{
	glDeleteFramebuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _frameBuffers);

	//glDeleteTextures(1, &_cameraTextures);

	glDeleteRenderbuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _cameraDepthBuffers);

}
bool Director::Ready(const Argument& arg)
{
	if (!Device::Instance().Initialize(arg.screenSize))
	{
		logger::Instance().error("Director::Ready Device has not been Initialized");
		return false;
	}

	data::Element::Recipe::Instance().PreReg();

	const auto& resCfgs = arg.resCfgs;
	auto itr = resCfgs.find(GB_RENDER_RESOURCE_CFG_SHADER);
	if (itr != resCfgs.end())
		if (!resource::Res<data::Shader>::Instance().Initialize(itr->second))
			return false;

	itr = resCfgs.find(GB_RENDER_RESOURCE_CFG_MATERIAL);
	if (itr != resCfgs.end())
		if (!resource::Res<data::Material>::Instance().Initialize(itr->second))
			return false;

	itr = resCfgs.find(GB_RENDER_RESOURCE_CFG_MESH);
	if (itr != resCfgs.end())
		if (!resource::Res<data::Mesh>::Instance().Initialize(itr->second))
			return false;

	itr = resCfgs.find(GB_RENDER_RESOURCE_CFG_ENTITY);
	if (itr != resCfgs.end())
		if (!resource::Res<data::Entity>::Instance().Initialize(itr->second))
			return false;

	itr = resCfgs.find(GB_RENDER_RESOURCE_CFG_FONT);
	if(itr != resCfgs.end())
		if (!resource::Res<data::Font>::Instance().Initialize(itr->second))
			return false;

	_ScreenSize = arg.screenSize;
	
	if (arg.rootEntity.length() != 0)
		_Root.Instantiate(arg.rootEntity);

	//framebuffer setup
	glGenFramebuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _frameBuffers);

	//texture
	GLTexture::ImageData imgData;
	imgData.data = nullptr;
	imgData.depth = GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT;
	imgData.height = _ScreenSize.y;
	imgData.internalFormat = GL_RGBA8;
	imgData.levels = 1;
	imgData.target = GL_TEXTURE_2D_ARRAY;
	imgData.width = _ScreenSize.x;

	_cameraTextures.SetData(imgData);

	//glGenTextures(1, &_cameraTextures);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, _cameraTextures);
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _ScreenSize.x, _ScreenSize.y, GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	//depth
	glGenRenderbuffers(GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT, _cameraDepthBuffers);

	const GLuint texObj = _cameraTextures.GetTextureObj();

	for (std::uint8_t i = 0; i < GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffers[i]);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texObj, 0, i);

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

	//draw setup
	_screenMat = resource::Res<data::Material>::Instance().Get("MultiCameras.lua");
	if (_screenMat == nullptr)
	{
		logger::Instance().error("Director::Ready _screenMat nullptr");
		return false;
	}
	const int texUnit = 0;

	data::UniformTextureVar texVar{ _cameraTextures.GetTarget(), _cameraTextures.GetTextureObj() };

	_screenMat->SetTexture("cameraTexs", &texVar);
	
	const data::Mesh* square = resource::Res<data::Mesh>::Instance().Get("Square.lua");
	
	std::array<GLBuffer::CtorParam, 3> drawParam;

	//1 vtx
	GLBuffer::CtorParam& vtxParam = drawParam[0];
	vtxParam.type = GLBuffer::Type::Static;
	vtxParam.size = square->GetVtxAttribByteSize();

	//2 idx
	GLBuffer::CtorParam& idxParam = drawParam[1];
	idxParam.type = GLBuffer::Type::Static;
	const GLVar* idxVar = square->GetIdxVar();
	idxParam.size = idxVar->byteSize();

	//inst
	GLBuffer::CtorParam& instParam = drawParam[2];
	instParam.type = GLBuffer::Type::Dynamic;
	instParam.size = GB_RENDER_DIRECTOR_MAX_CAMERA_COUNT * sizeof(std::uint32_t);

	_screenDraw.Initialize(drawParam);

	//set data
	//vtx
	_screenDraw.GetVtxBuffer().SetData(_screenMat->GetShader()->GetVtxVarInfo(0), square->GetVtxVars());

	//idx
	if (idxVar == nullptr)
	{
		logger::Instance().error("Director::Ready idxVar nullptr");
		return false;
	}
	_screenDraw.GetIdxBuffer().SetData(0, idxVar->data(), idxVar->byteSize());

	//_screenDraw.SetCount(idxVar->count());

	_screenDraw.VtxAttribPointerSetup(_screenMat->GetShader());

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
	glClearColor(_ClearColor.r, _ClearColor.g, _ClearColor.b, _ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	//camera shooting
	std::uint8_t instCount = 0;
	
	std::for_each(_Cameras.begin(), _Cameras.end(), [this, &instCount](Camera* cam)
	{
		const std::uint8_t frameBufferIdx = cam->GetFrameBufferIdx();
		_instVar[instCount] = frameBufferIdx;
		instCount++;

		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffers[frameBufferIdx]);
		cam->Shoot();

	});

	//director draw
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _ScreenSize.x, _ScreenSize.y);

	static constexpr std::size_t uint32_size = sizeof(std::uint32_t);

	_screenDraw.GetInstBuffer().SetData(0, _instVar, instCount * uint32_size);

	GL::applyShader(_screenMat->GetShader());

	_screenMat->Update();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, _cameraTextures);

	_screenDraw.Draw(6, instCount);

	return Device::Instance().Update();
}

void Director::AddCamera(Camera * const cam)
{
	if (_cameraFBOIndices.size() == 0)
	{
		logger::Instance().error("_cameraFBOIndices.size() == 0");
		return;
	}
	const auto ret = _Cameras.insert(cam);
	if (ret.second)
	{
		cam->_setFrameBufferIdx(_cameraFBOIndices.front());
		_cameraFBOIndices.pop();
	}
	
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
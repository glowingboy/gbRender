#include "Camera.h"

#include "Director.h"

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;

Camera::Camera(Entity * const owner) :
    Element(owner),
    _InterestTag(GB_RENDER_CAMERA_DEFAULT_INTERESTTAG),
    _frustumSphereBB(_Frustum.sphereBB),
    _transformedFSBB(_Frustum.sphereBB),
    _projectionMatrix(_Frustum.projectionMatrix),
    _ViewPort(0.0f, 0.0f, 1.0f, 1.0f),
    _screenSize(Director::Instance().GetScreenSize()),
    _IsStatic(false),
    _worldMat(owner->GetWorldTransformMatrix())
{
    _Frustum.set(60.0f, ((float)_screenSize.x) / _screenSize.y, 10, 100);
}

std::uint32_t Camera::GetType() const
{
    return Element::Type::Camera;
}

bool Camera::operator < (const Camera & o) const
{
    return this->_RenderQueue < o._RenderQueue;
}
void Camera::Awake()
{
    Director::Instance().AddCamera(this);

    GLBuffer::CtorParam param[4];

    //1 vtx
    param[0].type = GLBuffer::Type::Dynamic;
    param[0].size = 1000 * 1024 * 1024;
    //2 idx
    param[1].type = GLBuffer::Type::Dynamic;
    param[1].size = 500 * 1024 * 1024;
    //3 inst
    param[2].type = GLBuffer::Type::Dynamic;
    param[2].size = 100 * 1024 * 1024;
    //5 indirect
    param[3].type = GLBuffer::Type::Dynamic;
    param[3].size = 10240 * 1024;

    _multiIndirectDraw.Initialize(param);

    GB_UTILS_MULTI_CALLBACK_REG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, Camera::_onOwnerTransformChanged);

    //_Owner->GetCBs().RegisterCB(GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, (void*)&Camera::_onOwnerTransformChanged, std::bind(&(Camera::_onOwnerTransformChanged), this));


}
void Camera::Start()
{

}
void Camera::End()
{
    Director::Instance().RemoveCamera(this);
    GB_UTILS_MULTI_CALLBACK_UNREG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED);
}

void Camera::SetRenderQueue(const uint32 rq)
{
    _RenderQueue = rq;

    //refresh order of camera set 
    Director::Instance().RemoveCamera(this);
    Director::Instance().AddCamera(this);
}

void Camera::SetIsStatic(const bool isStatic)
{
    //TODO
    if (isStatic != _IsStatic)
	{
	    if (!_IsStatic)
		{
		    _multiIndirectDraw.Release();
		}
	}

}

gb::physics::vec3F Camera::Screen2World(const gb::physics::vec2F & screenPosition) const
{
    //map form screen space to NDC space
    vec4F retPos(1.0f);
    math::interval_mapper<float, float> mapper(_screenSize.x * _ViewPort[0], _screenSize.x * _ViewPort[2], -1.0f, 1.0f);
    retPos.x = mapper.map(screenPosition.x);

    mapper.reset(_screenSize.y * _ViewPort[1], _screenSize.y * _ViewPort[3], -1.0f, 1.0f);
    retPos.y = mapper.map(screenPosition.y);

    //world space
    mat4F inverseVP;
    if(_projMatProductViewMat.inverse(inverseVP))
	retPos = inverseVP * retPos;
    else
	{
	    GB_ASSERT(false, "Camera::Screen2World _projMatProductViewMat inverse failed");
	}
    // homogeneous to cartesian space
    retPos = retPos / retPos.w;

    return vec3F(retPos.x, retPos.y, retPos.z);
}

std::size_t triCount = 0; //TODO

std::size_t Camera::Shoot()
{
    glClearColor(_ClearColor.r, _ClearColor.g, _ClearColor.b, _ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //viewport setup
    glViewport((GLint)(_screenSize.x * _ViewPort[0]),
	       (GLint)(_screenSize.y * _ViewPort[1]),
	       (GLsizei)(_screenSize.x * _ViewPort[2]),
	       (GLsizei)(_screenSize.y * _ViewPort[3]));

    const Director::octreeEntity& renderEntities = Director::Instance().GetRenderEntities();
    struct intersectMethod//TODO change to _Ele Type?
    {
	bool operator()(const aabb<>& octanBB, const spherebb<>& q) const
	{
	    //return octanBB.intersect(q);
		return true;
	}
    };
    _ViewRangeEntites = renderEntities.query_intersect<spherebb<>, intersectMethod>(_transformedFSBB);

    //1st classify according to renderQueue
    //2nd classify according to shader
    //3rd classify according to material

    //classify according to renderQueue
    std::map<std::uint32_t, std::vector<BaseRender*>> renderQueueRenders;
    std::for_each(_ViewRangeEntites.begin(), _ViewRangeEntites.end(), [this, &renderQueueRenders](Entity* e)
								      {
									  if (_InterestTag & e->GetTag())
									      {
										  BaseRender* r = e->GetRender();

										  r->SetInstVar(GB_RENDER_INSTVAR_MVP, &(_projMatProductViewMat  * e->GetWorldTransformMatrix()));

										  const std::uint32_t renderQueue = r->GetMaterial()->GetShader()->GetMisc().renderQueue;

										  auto iter = renderQueueRenders.find(renderQueue);
										  if (iter != renderQueueRenders.end())
										      {
											  iter->second.push_back(r);
										      }
										  else
										      renderQueueRenders.insert(std::make_pair(renderQueue, std::vector<BaseRender*>{r}));
									      }
								      });


    std::for_each(renderQueueRenders.begin(), renderQueueRenders.end(), [this](std::pair<const std::uint32_t, std::vector<BaseRender*>>& pr)
									{
									    const std::vector<BaseRender*>& renders = pr.second;
									    std::unordered_map<const data::Shader*, std::vector<BaseRender*>> shaderRenders;
									    //classify according to shader
									    std::for_each(renders.begin(), renders.end(), [&shaderRenders](BaseRender* r)
															  {
															      const data::Shader* s = r->GetMaterial()->GetShader();

															      auto iter = shaderRenders.find(s);
															      if (iter != shaderRenders.end())
																  {
																      iter->second.push_back(r);
																  }
															      else
																  shaderRenders.insert(std::make_pair(s, std::vector<BaseRender*>{r}));
															  });


									    std::for_each(shaderRenders.begin(), shaderRenders.end(), [this](const std::pair<const data::Shader*, std::vector<BaseRender*>>& sr)
																      {
																	  const data::Shader* shader = sr.first;
																	  GL::applyShader(shader);
																	  _multiIndirectDraw.VtxAttribPointerSetup(shader);

																	  const std::vector<BaseRender*>& vRs = sr.second;
																	  std::unordered_map<data::Material*, std::vector<BaseRender*>> materialRenders;
																	  //classify according to material
																	  std::for_each(vRs.begin(), vRs.end(), [&materialRenders](BaseRender* r)
																						{
																						    data::Material* m = r->GetMaterial();
																						    auto iter = materialRenders.find(m);
																						    if (iter != materialRenders.end())
																							iter->second.push_back(r);
																						    else
																							materialRenders.insert(std::make_pair(m, std::vector<BaseRender*>{r}));
																						});
			
																	  std::for_each(materialRenders.begin(), materialRenders.end(), [this](const std::pair<data::Material*, std::vector<BaseRender*>>& mr)
																									{
																									    mr.first->Update();
																									    //dynamic draw
																										if(triCount == 0)
																											triCount +=_multiIndirectDraw.SetData(mr.second);
																									    _multiIndirectDraw.Draw();
																									});
																      });
									});


	return triCount;
}

void Camera::_onOwnerTransformChanged()
{
    //logger::Instance().log("Camera::_onOwnerTransformChanged");

    _transformedFSBB = _frustumSphereBB * _worldMat;

    _projMatProductViewMat = _projectionMatrix * _worldMat;
}

void Camera::_setFrameBufferIdx(const std::uint8_t idx)
{
    _FrameBufferIdx = idx;
}

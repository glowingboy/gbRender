#include "Render.h"
#include "Entity.h"
#include <gbUtils/logger.h>

#include "resource/Resource.h"

#include "Director.h"

using namespace gb::render;
using namespace gb::utils;
using namespace gb;
using namespace gb::physics;

BaseRender::BaseRender(Entity* const owner):
	Element(owner),
	_Mesh(nullptr),
	_originSBB(nullptr)
	//_instVar_mvp(nullptr)
{
	owner->_setRender(this);
}

void BaseRender::Awake()
{
	logger::Instance().log("render::awake @ " + _Owner->GetName());

	Director::Instance().AddRenderEntity(_Owner);

	GB_UTILS_MULTI_CALLBACK_REG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED, BaseRender::_onOwnerTransformChanged);
}

void BaseRender::Start()
{
	logger::Instance().log("render::start @ " + _Owner->GetName());
}

void BaseRender::End()
{
	logger::Instance().log("render::End @ " + _Owner->GetName());

	Director::Instance().RemoveRenderEntity(_Owner);
	GB_UTILS_MULTI_CALLBACK_UNREG(_Owner->GetCBs(), GB_RENDER_ENTITY_MSG_TRANSFORM_CHANGED);
}

std::uint32_t BaseRender::GetType() const
{
	return Element::Type::Render;
}
void BaseRender::SetInstVar(const char * name, const void * data)
{
	auto iter = _InstVar.find(name);
	if (iter != _InstVar.end())
		iter->second.set(data);
	else
	{
		logger::Instance().warning(string("Render::SetInstVar unknown instVar name@ ") + name + "owner.name@ " + _Owner->GetName());
	}
}

void BaseRender::SetMaterial(const string & material)
{
	_Material = resource::Res<data::Material>::Instance().Get(material);

	const std::vector<data::VtxVarStubInfo>& instVarStub = _Material->GetShader()->GetVtxVarInfo(1);

	_InstVar.clear();

	std::for_each(instVarStub.begin(), instVarStub.end(), [this](const data::VtxVarStubInfo& info)
	{
		auto iter = _InstVar.find(info.name);
		if (iter == _InstVar.end())
			_InstVar.insert(std::make_pair(info.name, GLVar(info)));
	});

	//auto iter = _InstVar.find(GB_RENDER_INSTVAR_MVP);
	//if (iter != _InstVar.end())
	//{
	//	_instVar_mvp = &(iter->second);

	//	_instVar_mvp->set(&(_Owner->GetWorldTransformMatrix()));
	//}
	//	
	//else
	//	_instVar_mvp = nullptr;
	logger::Instance().log("render::setmat @ " + material);
}

void BaseRender::_onOwnerTransformChanged()
{
	const mat4f& worldTransMat = _Owner->GetWorldTransformMatrix();
	_TransformedSphereBB = (*_originSBB) * worldTransMat;

	//if (_instVar_mvp != nullptr)
	//{
	//	_instVar_mvp->set(&worldTransMat);
	//}
}


Render::Render(Entity * const owner):
	BaseRender(owner)
{
}

void Render::SetMesh(const gb::utils::string & mesh)
{
	_Mesh = resource::Res<data::Mesh>::Instance().Get(mesh);

	_originSBB = &(_Mesh->GetSphereBB());

	_TransformedSphereBB = (*_originSBB) * _Owner->GetWorldTransformMatrix();

	logger::Instance().log("render::setMesh @ " + mesh);
}

void Render::tmpSetMesh(gb::render::data::Mesh * mesh)
{
	_Mesh = mesh;

	_originSBB = &(_Mesh->GetSphereBB());

	_TransformedSphereBB = (*_originSBB) * _Owner->GetWorldTransformMatrix();

}

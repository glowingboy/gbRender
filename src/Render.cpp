#include "Render.h"
#include "Entity.h"
#include <gbUtils/logger.h>

#include "resource/Resource.h"

#include "Director.h"

using namespace gb::render;
using namespace gb::utils;
using namespace gb;
Render::Render(Entity* const owner):
	Element(owner),
	_Mesh(nullptr)
{
	owner->_setRender(this);
}

void Render::Awake()
{
	logger::Instance().log("render::awake @ " + _Owner->GetName());

	Director::Instance().AddRenderEntity(_Owner);
}

void Render::Start()
{
	logger::Instance().log("render::start @ " + _Owner->GetName());
}

void Render::End()
{
	logger::Instance().log("render::End @ " + _Owner->GetName());

	Director::Instance().RemoveRenderEntity(_Owner);
}

Element::Type Render::GetType() const
{
	return Element::Type::Render;
}
void Render::SetMesh(const string & mesh)
{
	_Mesh = &(resource::Res<data::Mesh>::Instance().Get(mesh));

	_originSBB = &(_Mesh->GetSphereBB());

	_TransformedSphereBB = (*_originSBB) * _Owner->GetWorldTransformMatrix();

	logger::Instance().log("render::setMesh @ " + mesh);
}
void Render::SetMaterial(const string & material)
{
	logger::Instance().log("render::setmat @ " + material);
}
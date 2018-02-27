#include "Render.h"
#include "Entity.h"
#include <gbUtils/logger.h>

#include "resource/Resource.h"
#include "data/Mesh.h"

using namespace gb::render;
using namespace gb::utils;
using namespace gb;
Render::Render(Entity* const owner):
	Element(owner)
{}

void Render::Awake()
{
	logger::Instance().log("render::awake @ " + _Owner->GetName());
}

void Render::Start()
{
	logger::Instance().log("render::start @ " + _Owner->GetName());
}

void Render::End()
{
	logger::Instance().log("render::End @ " + _Owner->GetName());
}

Element::Type Render::GetType() const
{
	return Element::Type::Render;
}
void Render::SetMesh(const string & mesh)
{
	const data::Mesh & m = resource::Res<data::Mesh>::Instance().Get(mesh);
	logger::Instance().log("render::setMesh @ " + mesh);
}
void Render::SetMaterial(const string & material)
{
	logger::Instance().log("render::setmat @ " + material);
}
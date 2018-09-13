#pragma once
#include "Element.h"
#include <gbUtils/string.h>
#include "data/Mesh.h"
#include "data/Material.h"

GB_RENDER_NS_BEGIN

class BaseRender : public Element
{
public:
	BaseRender(Entity* const owner);

public:
	void SetMaterial(const gb::utils::string & material);
	virtual std::uint32_t GetType() const override;

	void SetInstVar(const char* name, const void* data);
protected:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;

protected:

	GB_PROPERTY_R(protected, Mesh, gb::render::data::Mesh*);
	GB_PROPERTY_R(protected, Material, gb::render::data::Material*);
protected:
	const gb::physics::spherebb<>* _originSBB;
	GB_PROPERTY_R(protected, TransformedSphereBB, gb::physics::spherebb<>);

	GB_PROPERTY_R(protected, InstVar, std::unordered_map <const gb::utils::string, GLVar>);
	//GLVar* _instVar_mvp;
private:
	void _onOwnerTransformChanged();
};

class Render : public BaseRender
{

public:
	Render(Entity* const owner);
public:
	void SetMesh(const gb::utils::string & mesh);
	void tmpSetMesh(gb::render::data::Mesh * mesh); //TODO
};
GB_RENDER_NS_END


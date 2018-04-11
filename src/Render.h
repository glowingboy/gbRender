#pragma once
#include "Element.h"
#include <gbUtils/string.h>
#include "data/Mesh.h"
#include "data/Material.h"

GB_RENDER_NS_BEGIN

class Render : public Element
{
public:
	Render(Entity* const owner);

public:
	void SetMesh(const gb::utils::string & mesh);
	void SetMaterial(const gb::utils::string & material);
	virtual Element::Type GetType() const override;

	void SetInstVar(const char* name, const void* data);
private:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void End() override;

	GB_PROPERTY_R(private, Mesh, const gb::render::data::Mesh*);
	GB_PROPERTY_R(private, Material, gb::render::data::Material*);

	const gb::physics::spherebb<>* _originSBB;
	GB_PROPERTY_R(private, TransformedSphereBB, gb::physics::spherebb<>);

	GB_PROPERTY_R(private, InstVar, std::unordered_map <const gb::utils::string, GLVar>);
	//GLVar* _instVar_mvp;
private:
	void _onOwnerTransformChanged();
};

GB_RENDER_NS_END


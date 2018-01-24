#pragma once
#include "Element.h"
#include <gbUtils/string.h>
#include "resource/Mesh.h"
GB_RENDER_NS_BEGIN

class Render : public Element
{
public:
	inline Render() {};

public:
	void SetMesh(const gb::utils::string & mesh);
	void SetMaterial(const gb::utils::string & material);
private:
	virtual void Awake() override;
	virtual void Start() override;
	GB_PROPERTY_R(private, Mesh, gb::render::resource::Mesh*);
};

GB_RENDER_NS_END


#include "Material.h"
#include <gbUtils/logger.h>
#include <gbUtils/string.h>
#include "../resource/Resource.h"
using namespace gb::render::data;
using namespace gb::utils;
using namespace gb::render;

void Material::from_lua(const luatable_mapper& mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_MATERIAL_KEY_SHADER))
	{
		_Shader = resource::Res<Shader>::Instance().Get(mapper.get_string_by_key(GB_RENDER_DATA_MATERIAL_KEY_SHADER));

		_uniformVars = _Shader->GenUniformVars();

		std::for_each(_uniformVars.begin(), _uniformVars.end(), [&mapper](std::pair<const string, UniformVar>& uniVar)
		{
			uniVar.second.GetFromMaterial(mapper, uniVar.first);
		});
	}
	else
	{
		logger::Instance().error("Material::from_lua no GB_RENDER_DATA_MATERIAL_KEY_SHADER found @ " + mapper.GetData());
		return;
	}
}

void Material::SetTexture(const char * name, const UniformTextureVar* textures, const GLsizei count)
{
	assert(name != nullptr && textures != nullptr);

	auto iter = _uniformVars.find(name);

	if (iter != _uniformVars.cend())
	{
		iter->second.SetTextureObjs(textures, count);
	}
	else
		logger::Instance().error(string("Material::SetTexture no specified texture uniform var found, name@ ") + name);
}

void Material::Update() const
{
	std::for_each(_uniformVars.cbegin(), _uniformVars.cend(), [](const std::pair<const gb::utils::string, UniformVar>& var)
	{
		var.second.Update();
	});
}
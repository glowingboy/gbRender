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
	}
	else
	{
		logger::Instance().error("Material::from_lua no GB_RENDER_DATA_MATERIAL_KEY_SHADER found @ " + mapper.GetData());
		return;
	}
}

void Material::Update() const
{
	std::for_each(_uniformVars.cbegin(), _uniformVars.cend(), [](const std::pair<const gb::utils::string, UniformVar>& var)
	{
		var.second.Update();
	});
}
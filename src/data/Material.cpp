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

template<typename T>
void Material::SetUniform(const char* name, const T* data, const GLsizei count)
{
	auto iter = _uniformVars.find(name);

	if (iter != _uniformVars.cend())
	{
		if (!(iter->second.SetData(data, sizeof(T) * count)))
			logger::Instance().error("Material::SetUniform mismatched byteSize name@ " + iter->first);
	}
	else
		logger::Instance().error("Material::SetUniform no specified uniform var name@ " + string(name));
}

void Material::Update() const
{
	std::for_each(_uniformVars.cbegin(), _uniformVars.cend(), [](const std::pair<const gb::utils::string, UniformVar>& var)
	{
		var.second.Update();
	});
}
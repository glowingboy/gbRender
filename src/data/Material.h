#pragma once

#include "Shader.h"

#define GB_RENDER_DATA_MATERIAL_KEY_SHADER "Shader"
#define GB_RENDER_DATA_MATERIAL_KEY_UNIFORMVARS "UniformVars"



GB_RENDER_DATA_NS_BEGIN

class Material
{
public:
	void from_lua(const gb::utils::luatable_mapper& mapper);
	
	template<typename T>
	void SetUniform(const char* name, const T* data, const GLsizei count = 1)
	{
		assert(name != nullptr && data != nullptr);

		auto iter = _uniformVars.find(name);

		if (iter != _uniformVars.cend())
		{
			if (!(iter->second.SetData(data, sizeof(T) * count)))
				logger::Instance().error("Material::SetUniform mismatched byteSize name@ " + iter->first);
		}
		else
			logger::Instance().error("Material::SetUniform no specified uniform var name@ " + string(name));
	}

	void SetTexture(const char* name, const UniformTextureVar* textures, const GLsizei count = 1);

	void Update() const;
private:
	GB_PROPERTY_R(private, Shader, const Shader*);

	std::unordered_map<gb::utils::string, UniformVar> _uniformVars;

	struct _unitTexture
	{
		std::uint32_t unit;
		GLuint texture;
	};

	std::unordered_map<gb::utils::string, _unitTexture> _unitTexture;
};

GB_RENDER_DATA_NS_END
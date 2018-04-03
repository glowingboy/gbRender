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
	void SetUniform(const char* name, const T* data, const GLsizei count = 1);

	void Update() const;
private:
	GB_PROPERTY_R(private, Shader, const Shader*);

	std::unordered_map<gb::utils::string, UniformVar> _uniformVars;
};

GB_RENDER_DATA_NS_END
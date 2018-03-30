#pragma once

#include "Shader.h"

#define GB_RENDER_DATA_MATERIAL_KEY_SHADER "Shader"
#define GB_RENDER_DATA_MATERIAL_KEY_UNIFORMVARS "UniformVars"



GB_RENDER_DATA_NS_BEGIN

class Material
{
public:
	void from_lua(const gb::utils::luatable_mapper& mapper);
	
	GB_PROPERTY_R(private, Shader, const Shader*);
};

GB_RENDER_DATA_NS_END
#pragma once
#include "DataNS.h"
#include <gbUtils/common.h>
#include <unordered_map>
#include <gbUtils/string.h>
#include <gbUtils/luatable.h>
#include "../GL.h"

GB_RENDER_DATA_NS_BEGIN

enum class VtxVarType
{
	Reserved = 0,
	Int, UInt, Float, Vec2, Vec3, Vec4, Mat4, Handleui64
};

struct VtxVarStub
{
	VtxVarType type;

};

enum class VtxVarBriefType
{
	Float = 0,
	Integer, Double
};
struct VtxVarInfo
{
	GLuint index;
	GLint size;
	GLenum type;
	GLsizei stride;
	GLsizei offset;
	std::uint32_t count;
	std::uint32_t typeSize;

	VtxVarBriefType briefType;
};

class Shader
{
public:
	void from_lua(gb::utils::luatable_mapper & mapper);
	void VtxPointerSetup();
	GB_PROPERTY_R(private, VtxVarInfos, std::unordered_map<gb::utils::string, VtxVarInfo>);
};

GB_RENDER_DATA_NS_END

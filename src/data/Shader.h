#pragma once
#include "DataNS.h"
#include <gbUtils/common.h>
#include <unordered_map>
#include <gbUtils/string.h>
#include <gbUtils/luatable.h>
#include "../GL.h"
#include <type_traits>

#define GB_RENDER_DATA_SHADER_INFO_BEGIN "GB_RENDER_DATA_SHADER_INFO_BEGIN"
#define GB_RENDER_DATA_SHADER_INFO_END "GB_RENDER_DATA_SHADER_INFO_END"

#define GB_RENDER_DATA_SHADER_VERT_BEGIN "GB_RENDER_DATA_SHADER_VERT_BEGIN"
#define GB_RENDER_DATA_SHADER_VERT_END "GB_RENDER_DATA_SHADER_VERT_END"

#define GB_RENDER_DATA_SHADER_FRAG_BEGIN "GB_RENDER_DATA_SHADER_FRAG_BEGIN"
#define GB_RENDER_DATA_SHADER_FRAG_END "GB_RENDER_DATA_SHADER_FRAG_END"

#define GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS "VtxVar"
#define GB_RENDER_DATA_SHADER_INFO_KEY_GLCFG "GLCfg"

GB_RENDER_DATA_NS_BEGIN

enum class VtxVarStubType : int
{
	Reserved = 0,
	Int, UInt, Float, Vec2, Vec3, Vec4, Mat4, Handleui64
};


enum class VtxVarBriefType : int
{
	Float = 0,
	Integer, Double
};

#define GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_NAME "Name"
#define GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_TYPE "Type"
#define GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT "Count"
#define GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR "Divisor"

struct VtxVarStubInfo
{
	GLuint index;
	GLint size;
	GLenum type;
	GLsizei stride;
	GLsizei offset;
	std::uint32_t count;
	std::uint32_t typeSize;
	GLuint divisor;

	VtxVarBriefType briefType;
};

struct VtxVarStub
{
	VtxVarStub();
	GLuint index;
	VtxVarStubType type;
	std::uint32_t count;
	std::uint32_t divisor;
	gb::utils::string name;
	void from_lua(const gb::utils::luatable_mapper& mapper);
	static GLint componentCount(const VtxVarStubType type);
	static GLenum glType(const VtxVarStubType type);
	static std::uint32_t typeCount(const VtxVarStubType type);
	static std::size_t typeSize(const VtxVarStubType type);
	static std::size_t byteSize(const VtxVarStubType type);

	VtxVarStubInfo genInfo(const GLsizei stride, const GLsizei offset) const;
};


class Shader
{
public:
	
public:
	bool from_lua(gb::utils::luatable_mapper & mapper, const char* shaderName);
	void VtxPointerSetup();

	GLint GetVtxAttribLocation(const char* name);
	

private:
	GB_PROPERTY_R(private, Name, gb::utils::string);
	GLuint _programObj;
	static const std::vector<std::string> _blockDelimiter;
	//0: vtxVar, 1: instVar
	std::unordered_map<gb::utils::string, VtxVarStubInfo> _vtxVarInfos[2];

private:
	bool _compile(const char* vert, const char* frag);
	static GLint _checkShaderCompile(GLuint shader);
	static GLint _checkShaderLink(GLuint program);
};

template <typename T>
struct is_shader : public std::false_type {};

template <>
struct is_shader <Shader> : public std::true_type {};

GB_RENDER_DATA_NS_END

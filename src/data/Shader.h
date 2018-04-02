#pragma once
#include "DataNS.h"
#include <gbUtils/common.h>
#include <vector>
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

#define GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS "VtxVars"
#define GB_RENDER_DATA_SHADER_INFO_KEY_UNIFORMVARS "UniformVars"
#define GB_RENDER_DATA_SHADER_INFO_KEY_GLCFG "GLCfg"

GB_RENDER_DATA_NS_BEGIN

enum class slType : int
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
	gb::utils::string name;

	VtxVarBriefType briefType;
};

struct VtxVarStub
{
	VtxVarStub();
	slType type;
	std::uint32_t count;
	std::uint32_t divisor;
	gb::utils::string name;
	GLuint index;
	std::size_t byteSize;

	void from_lua(const gb::utils::luatable_mapper& mapper);
	static GLint componentCount(const slType type);
	static GLenum glType(const slType type);
	static std::uint32_t typeCount(const slType type);
	static std::size_t typeSize(const slType type);
	//static std::size_t byteSize(const VtxVarStubType type);

	VtxVarStubInfo genInfo(const GLsizei stride, const GLsizei offset) const;
};

struct UniformVarStub
{
	gb::utils::string name;

};

#define GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND "Blend"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_ENABLED "Enabled"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_SFACTOR "SFactor"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_DFACTOR "DFactor"

#define GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST "DepthTest"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_ENABLED "Enabled"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_FUNC "Func"

#define GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST "AlphaTest"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_ENABLED "Enabled"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_FUNC "Func"
#define GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_REF "Ref"

#define GB_RENDER_DATA_SHADER_GLCFG_KEY_POLYGONMODE "PolygonMode"

struct GLCfg
{
	GLCfg();
	void from_lua(const gb::utils::luatable_mapper& mapper);
	struct Blend
	{
		void from_lua(const gb::utils::luatable_mapper& mapper);
		bool enabled;

		GLenum sfactor;
		GLenum dfactor;
	}blend;
	
	struct DepthTest
	{
		void from_lua(const gb::utils::luatable_mapper& mapper);
		bool enabled;
		GLenum func;
	}depthTest;

	struct AlphaTest
	{
		void from_lua(const gb::utils::luatable_mapper& mapper);
		bool enabled;

		GLenum func;
		GLclampf ref;
	}alphaTest;

	GLenum polygonMode;
};

class Shader
{
public:
	
public:
	bool from_lua(gb::utils::luatable_mapper & mapper, const char* shaderName);
	void Use() const;
	void VtxPointerSetup() const;

	GLint GetVtxAttribLocation(const char* name) const;
	

private:
	GB_PROPERTY_R(private, Name, gb::utils::string);
	GLuint _programObj;
	static const std::vector<std::string> _blockDelimiter;
	//0: vtxVar, 1: instVar
	std::vector<VtxVarStubInfo> _vtxVarInfos[2];

	GB_PROPERTY_R(private, GLCfg, GLCfg);
private:
	bool _compile(const char* vert, const char* frag);
	static GLint _checkShaderCompile(GLuint shader);
	static GLint _checkShaderLink(GLuint program);
};

GB_RENDER_DATA_NS_END

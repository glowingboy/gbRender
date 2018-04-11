#pragma once
#include "DataNS.h"
#include <gbUtils/common.h>
#include <vector>
#include <gbUtils/string.h>
#include <gbUtils/luatable.h>
#include "../GL.h"
#include <type_traits>
#include <unordered_map>

#define GB_RENDER_DATA_SHADER_INFO_BEGIN "GB_RENDER_DATA_SHADER_INFO_BEGIN"
#define GB_RENDER_DATA_SHADER_INFO_END "GB_RENDER_DATA_SHADER_INFO_END"

#define GB_RENDER_DATA_SHADER_VERT_BEGIN "GB_RENDER_DATA_SHADER_VERT_BEGIN"
#define GB_RENDER_DATA_SHADER_VERT_END "GB_RENDER_DATA_SHADER_VERT_END"

#define GB_RENDER_DATA_SHADER_FRAG_BEGIN "GB_RENDER_DATA_SHADER_FRAG_BEGIN"
#define GB_RENDER_DATA_SHADER_FRAG_END "GB_RENDER_DATA_SHADER_FRAG_END"

#define GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS "VtxVars"
#define GB_RENDER_DATA_SHADER_INFO_KEY_UNIFORMVARS "UniformVars"
#define GB_RENDER_DATA_SHADER_INFO_KEY_GLCFG "GLCfg"
#define GB_RENDER_DATA_SHADER_INFO_KEY_MISC "Misc"


#define GB_RENDER_DATA_SHADER_RENDERQUEUE_BACKGROUND 0X00
#define GB_RENDER_DATA_SHADER_RENDERQUEUE_OPAQUE 0xf
#define GB_RENDER_DATA_SHADER_RENDERQUEUE_TRANSPARENT 0xff
#define GB_RENDER_DATA_SHADER_RENDERQUEUE_UI 0x8ff
#define GB_RENDER_DATA_SHADER_RENDERQUEUE_GIZMO 0xfff

GB_RENDER_DATA_NS_BEGIN

struct slType
{
	static constexpr std::uint8_t Reserved = 0u;
	static constexpr std::uint8_t Int = 1u;
	static constexpr std::uint8_t UInt = 2u;
	static constexpr std::uint8_t Float = 3u;
	static constexpr std::uint8_t Vec2 = 4u;
	static constexpr std::uint8_t Vec3 = 5u;
	static constexpr std::uint8_t Vec4 = 6u;
	static constexpr std::uint8_t Mat4 = 7u;
	static constexpr std::uint8_t Handleui64 = 8u;

	static GLint componentCount(const std::uint8_t type);
	static GLenum glType(const std::uint8_t type);
	static std::uint32_t typeCount(const std::uint8_t type);
	static std::size_t typeSize(const std::uint8_t type);

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

	//component count
	GLint size;

	GLenum type;
	GLsizei stride;
	GLsizei offset;

	//array count
	std::uint32_t count;

	//mat4 typeSize = typeSize(vec4)
	std::uint32_t typeSize;

	GLuint divisor;
	gb::utils::string name;

	VtxVarBriefType briefType;
};

struct VtxVarStub
{
	VtxVarStub();
	std::uint8_t sl_type;
	std::uint32_t count;
	std::uint32_t divisor;
	gb::utils::string name;
	GLuint index;
	std::size_t byteSize;

	void from_lua(const gb::utils::luatable_mapper& mapper);

	//static std::size_t byteSize(const VtxVarStubType type);

	VtxVarStubInfo genInfo(const GLsizei stride, const GLsizei offset) const;
};


struct UniformVar
{
	UniformVar(const GLint index_, const std::size_t typeSize, const GLsizei count_, const std::uint8_t sl_type_);
	~UniformVar();
	UniformVar(UniformVar && other);
	bool SetData(const void* data_, const std::size_t size);
	void Update() const;
	GLint index;
	std::size_t byteSize;
	GLsizei count;
	void* data;

	inline void GetFromMaterial(const gb::utils::luatable_mapper& mapper, const char* name)
	{
		(this->*_lua_getter)(mapper, name);
	}
private:
	typedef void(*setter)(const GLint location, const GLsizei count, const void* value);
	typedef void(UniformVar::*lua_getter)(const gb::utils::luatable_mapper& mapper, const char* name);
	//TODO lua_setter
	void _lua_getter_integer(const gb::utils::luatable_mapper& mapper, const char* name);
	void _lua_getter_integers(const gb::utils::luatable_mapper& mapper, const char* name);
	void _lua_getter_number(const gb::utils::luatable_mapper& mapper, const char* name);
	void _lua_getter_numbers(const gb::utils::luatable_mapper& mapper, const char* name);
	setter _setter;
	lua_getter _lua_getter;
};

#define GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_NAME "Name"
#define GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_TYPE "Type"
#define GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_COUNT "Count"

struct UniformVarStub
{
	UniformVarStub();
	
	void from_lua(const gb::utils::luatable_mapper& mapper);
	UniformVar genUniformVar() const;
	gb::utils::string name;
	GLint index;
	std::uint8_t sl_type;
	GLsizei count;

	std::size_t typeSize;
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
	#define	GB_RENDER_DATA_SHADER_MISC_KEY_RENDERQUEUE "RenderQueue"
	struct Misc
	{
		Misc();
		void from_lua(const gb::utils::luatable_mapper& mapper);
		std::uint32_t renderQueue;
	};
public:
	bool from_lua(gb::utils::luatable_mapper & mapper, const char* shaderName);
	void Use() const;
	//0: vtx, 1: inst
	void VtxPointerSetup(const std::uint8_t idx, const GLuint vbo) const;

	GLint GetVtxAttribLocation(const char* name) const;
	GLint GetUniformLocation(const char* name) const;
	std::unordered_map<gb::utils::string, UniformVar> GenUniformVars() const;
	//0: vtx, 1: inst
	const std::vector<VtxVarStubInfo>& GetVtxVarInfo(std::uint8_t idx) const;

#define _GB_RENDER_DATA_SHADER_SET_UNIFORM_DCLR_(type) \
	static void SetUniform1##type(const GLint location, const GLsizei count, const void* value); \
	static void SetUniform2##type(const GLint location, const GLsizei count, const void* value); \
	static void SetUniform3##type(const GLint location, const GLsizei count, const void* value); \
	static void SetUniform4##type(const GLint location, const GLsizei count, const void* value); \

	_GB_RENDER_DATA_SHADER_SET_UNIFORM_DCLR_(f);
	_GB_RENDER_DATA_SHADER_SET_UNIFORM_DCLR_(i);
	_GB_RENDER_DATA_SHADER_SET_UNIFORM_DCLR_(ui);


	static void SetUniformMatrix4f(const GLint location, const GLsizei count, const void* value);

private:
	GB_PROPERTY_R(private, Name, gb::utils::string);
	GLuint _programObj;
	static const std::vector<std::string> _blockDelimiter;
	//0: vtxVar, 1: instVar
	std::vector<VtxVarStubInfo> _vtxVarInfos[2];

	GB_PROPERTY_R(private, GLCfg, GLCfg);

	std::vector<UniformVarStub> _uniformVarSubs;

	GB_PROPERTY_R(private, Misc, Misc);
private:
	bool _compile(const char* vert, const char* frag);
	static GLint _checkShaderCompile(GLuint shader);
	static GLint _checkShaderLink(GLuint program);
};

GB_RENDER_DATA_NS_END

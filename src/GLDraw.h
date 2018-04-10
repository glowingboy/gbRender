#pragma once

#include "RenderNS.h"
#include <gbUtils/common.h>
#include "data/Shader.h"
#include "GLVariable.h"
#include <vector>
#include <array>

GB_RENDER_NS_BEGIN

class GLBuffer
{
public:
	enum class Type : int
	{
		Reserved = 0,
		Static, Dynamic
	};

	struct CtorParam
	{
		CtorParam();
		Type type;
		std::size_t size;
		void* data;
	};

	GLBuffer();
	GLBuffer(const GLenum target, const CtorParam& param);
	~GLBuffer();
public:
	void Initialize(const GLenum target, const CtorParam& param);
	void SetData(const std::size_t offset, const void* data, const std::size_t size);
	void SetData(const std::vector<data::VtxVarStubInfo>& rule, const std::unordered_map <const gb::utils::string, GLVar>& data, const std::size_t offset = 0);
	GB_PROPERTY_R(protected, Type, Type);
	GB_PROPERTY_R(protected, BufferO, GLuint);
	GB_PROPERTY_R(protected, Size, GLsizeiptr);
private:
	char* _pData;
private:
	void _initialize(const GLenum target, const void* data);
};

class GLDraw
{
protected:

	GLDraw();
	//0: vtx, 1: idx
	GLDraw(const GLBuffer::CtorParam * bufParams);
	~GLDraw();
	void Initialize(const GLBuffer::CtorParam * bufParams);

	GB_PROPERTY_R(protected, VAO, GLuint);

	GB_PROPERTY_R(protected, VtxBuffer, GLBuffer);
	GB_PROPERTY_R(protected, IdxBuffer, GLBuffer);
};

namespace data
{
	class Shader;
}

class GLInstancedDraw : public GLDraw
{
public:
	GLInstancedDraw();
	//0: vtx, 1: idx, 2: inst
	GLInstancedDraw(const std::array<GLBuffer::CtorParam, 3>& param);
	void Initialize(const std::array<GLBuffer::CtorParam, 3>& param);
	void VtxAttribPointerSetup(const data::Shader* shader);
	void Draw(const GLsizei count, const GLsizei instanceCount) const ;
protected:
	GLInstancedDraw(const GLBuffer::CtorParam * const& param);//*& for decreasing rank when overload resolution happens (GLInstancedDraw(const GLBuffer::CtorParam (&param) [3]) has same rank)
	void Initialize(const GLBuffer::CtorParam * const& param);
private:
	GB_PROPERTY_R(protected, InstBuffer, GLBuffer);
	GB_PROPERTY(protected, Mode, GLenum);
};

class Render;

class GLIndirectDraw : public GLInstancedDraw
{
public:
	struct IndirectCommand
	{
		IndirectCommand();
		GLuint  count;
		GLuint  instanceCount;
		GLuint  firstIndex;
		GLuint  baseVertex;
		GLuint  baseInstance;
	};

	GLIndirectDraw();

	GLIndirectDraw(const GLBuffer::CtorParam(&param)[4]);
	void Initialize(const GLBuffer::CtorParam(&param)[4]);
	void SetData(const std::vector<Render*> renders);
	void Draw() const;
	void Release();
private:
	//0: vtx, 1: idx, 2: inst, 3: indirectCmd
	GB_PROPERTY_R(private, IndirectCmdBuffer, GLBuffer);
	//GB_PROPERTY_R(private, CmdCount, std::uint32_t);
	//GB_PROPERTY_R(private, IndirectCmd, )

private:
	IndirectCommand * _pCmd;
};

GB_RENDER_NS_END
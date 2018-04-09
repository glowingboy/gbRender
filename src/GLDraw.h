#pragma once

#include "RenderNS.h"
#include <gbUtils/common.h>
#include "data/Shader.h"
#include "GLVariable.h"

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
	void SetData(const std::vector<data::VtxVarStubInfo>& rule, const std::unordered_map <const gb::utils::string, GLVar>& data);
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
	GLInstancedDraw(const GLBuffer::CtorParam (&param) [3]);
	void Initialize(const GLBuffer::CtorParam(&param)[3]);
	void VtxAttribPointerSetup(const data::Shader* shader);
	void Draw();
private:
	GB_PROPERTY_R(private, InstBuffer, GLBuffer);
	GB_PROPERTY(private, Mode, GLenum);
	GB_PROPERTY(private, Count, GLsizei);
	GB_PROPERTY(private, InstanceCount, GLsizei);
};
GB_RENDER_NS_END
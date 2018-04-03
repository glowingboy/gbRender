#include "GLDraw.h"
#include <gbUtils/logger.h>
#include <algorithm>

using namespace gb::render;
using namespace gb::utils;

GLBuffer::CtorParam::CtorParam():
	type(Type::Reserved),
	size(0),
	data(nullptr)
{}

GLBuffer::GLBuffer():
	_Type(Type::Reserved),
	_BufferO(0),
	_Size(0),
	_pData(0)
{}

GLBuffer::GLBuffer(const GLenum target, const CtorParam& param):
	_Type(param.type),
	_BufferO(0),
	_Size(param.size)
{
	_initialize(target, param.data);
}
void GLBuffer::_initialize(const GLenum target, const void* data)
{
	if (_BufferO != 0)
		glDeleteBuffers(1, &_BufferO);

	glGenBuffers(1, &_BufferO);
	glBindBuffer(target, _BufferO);
	GLbitfield vtxAttribFlag = GL_MAP_WRITE_BIT;
	if (_Type == Type::Dynamic)
		vtxAttribFlag |= GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(target, _Size, data, vtxAttribFlag);

	if (_Type == Type::Dynamic)
	{
		_pData = (char*)glMapBufferRange(target, 0, _Size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
		if (_pData == nullptr)
		{
			logger::Instance().error("GLBuffer::GLBuffer glMapBufferRange return nullptr");
		}
	}

	glBindBuffer(target, 0);
}
void GLBuffer::Initialize(const GLenum target, const CtorParam& param)
{
	_Type = param.type;
	_Size = param.size;

	_initialize(target, param.data);
}

GLBuffer::~GLBuffer()
{
	if (_Type == Type::Dynamic)
		glUnmapNamedBuffer(_BufferO);

	glDeleteBuffers(1, &_BufferO);
}

void GLBuffer::SetData(const std::size_t offset, const void* data, const std::size_t size)
{
	if (_Type == Type::Static)
	{
		_pData = (char*)glMapNamedBufferRange(_BufferO, 0, _Size, GL_MAP_WRITE_BIT);
		if (_pData == nullptr)
		{
			logger::Instance().error("GLBuffer::SetData glMapNamedBufferRange return nullptr");
			return;
		}
	}

	if (size <= (_Size - offset))
		std::memcpy(_pData + offset, data, size);
	else
		logger::Instance().error("GLBuffer::SetData size <= (_Size - offset)");

	if (_Type == Type::Static)
	{
		glUnmapNamedBuffer(_BufferO);
		_pData = nullptr;
	}
}

void GLBuffer::SetData(const std::vector<data::VtxVarStubInfo>& rule, const std::unordered_map <const gb::utils::string, GLVar>& data)
{
	if (_Type == Type::Static)
	{
		_pData = (char*)glMapNamedBufferRange(_BufferO, 0, _Size, GL_MAP_WRITE_BIT);
		if (_pData == nullptr)
		{
			logger::Instance().error("GLBuffer::SetData glMapNamedBufferRange return nullptr");
			return;
		}
	}

	std::for_each(rule.cbegin(), rule.cend(), [this, &data](const data::VtxVarStubInfo& info)
	{
		auto iter = data.find(info.name);
		if (iter != data.cend())
		{
			const GLVar& var = iter->second;
			const std::uint8_t unitSize = info.typeSize * info.count;
			if (unitSize == var.unitSize())
			{
				const std::size_t stride = info.stride;
				char* curData = _pData + info.offset;


				const std::size_t count = var.count();
				const char* srcData = var.data();
				const std::size_t typeSize = info.size;

				for (std::size_t i = 0; i < count; i++)
				{
					std::memcpy(curData + i * stride, srcData + i * unitSize, unitSize);
				}
			}
			else
			{
				logger::Instance().error("GLBuffer::SetData unitSize mismathced name@" + info.name);
			}
	
		}
		else
			logger::Instance().error("GLBuffer::SetData no var fouund in data name@" + info.name);

	});

	if (_Type == Type::Static)
	{
		glUnmapNamedBuffer(_BufferO);
		_pData = nullptr;
	}
}
GLDraw::GLDraw():
	_VAO(0)
{}

GLDraw::GLDraw(const GLBuffer::CtorParam(&bufParam)[2]) :
	_VAO(0),
	_VtxBuffer(GL_ARRAY_BUFFER, bufParam[0]),
	_IdxBuffer(GL_ELEMENT_ARRAY_BUFFER, bufParam[1])
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VtxBuffer.GetBufferO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IdxBuffer.GetBufferO());

	glBindVertexArray(0);
}

void GLDraw::Initialize(const GLBuffer::CtorParam(&bufParam)[2])
{
	_VtxBuffer.Initialize(GL_ARRAY_BUFFER, bufParam[0]);
	_IdxBuffer.Initialize(GL_ELEMENT_ARRAY_BUFFER, bufParam[1]);

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VtxBuffer.GetBufferO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IdxBuffer.GetBufferO());

	glBindVertexArray(0);
}

GLDraw::~GLDraw()
{
	glDeleteVertexArrays(1, &_VAO);
}

GLInstancedDraw::GLInstancedDraw():
	_Mode(GL_TRIANGLES),
	_Count(1),
	_InstanceCount(1)
{}

GLInstancedDraw::GLInstancedDraw(const GLBuffer::CtorParam(&param)[3]):
	GLDraw(gb_cast<GLBuffer::CtorParam (&) [2]>( param)),
	_InstBuffer(GL_ELEMENT_ARRAY_BUFFER, param[2]),
	_Mode(GL_TRIANGLES),
	_Count(1),
	_InstanceCount(1)
{
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _InstBuffer.GetBufferO());

	glBindVertexArray(0);

}

void GLInstancedDraw::Initialize(const GLBuffer::CtorParam(&param)[3])
{
	GLDraw::Initialize(gb_cast<GLBuffer::CtorParam(&)[2]>(param));
	_InstBuffer.Initialize(GL_ELEMENT_ARRAY_BUFFER, param[2]);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _InstBuffer.GetBufferO());

	glBindVertexArray(0);

}

void GLInstancedDraw::Draw()
{
	glBindVertexArray(_VAO);

	glDrawElementsInstanced(_Mode, _Count, GL_UNSIGNED_INT, 0, _InstanceCount);

	glBindVertexArray(0);
}
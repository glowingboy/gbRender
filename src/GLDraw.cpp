#include "GLDraw.h"
#include <gbUtils/logger.h>
#include <algorithm>
#include <unordered_map>
#include "Render.h"

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

void GLBuffer::SetData(const std::vector<data::VtxVarStubInfo>& rule, const std::unordered_map <const gb::utils::string, GLVar>& data, const std::size_t offset)
{
	if (rule.size() > 0)
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


		const std::size_t stride = rule.begin()->stride;

		std::for_each(rule.cbegin(), rule.cend(), [this, &offset, &data, &stride](const data::VtxVarStubInfo& info)
		{
			auto iter = data.find(info.name);
			if (iter != data.cend())
			{
				const GLVar& var = iter->second;
				const std::uint8_t unitSize = info.typeSize * info.count;
				if (unitSize == var.unitSize())
				{
					assert(stride == info.stride);

					char* curData = _pData + offset + info.offset;


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
				logger::Instance().error("GLBuffer::SetData no var found in data name@" + info.name);

		});


		if (_Type == Type::Static)
		{
			glUnmapNamedBuffer(_BufferO);
			_pData = nullptr;
		}
	}
}
GLDraw::GLDraw():
	_VAO(0)
{}

GLDraw::GLDraw(const GLBuffer::CtorParam * bufParams) :
	_VAO(0),
	_VtxBuffer(GL_ARRAY_BUFFER, bufParams[0]),
	_IdxBuffer(GL_ELEMENT_ARRAY_BUFFER, bufParams[1])
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VtxBuffer.GetBufferO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IdxBuffer.GetBufferO());

	glBindVertexArray(0);
}

void GLDraw::Initialize(const GLBuffer::CtorParam * bufParams)
{
	_VtxBuffer.Initialize(GL_ARRAY_BUFFER, bufParams[0]);
	_IdxBuffer.Initialize(GL_ELEMENT_ARRAY_BUFFER, bufParams[1]);

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
	_Mode(GL_TRIANGLES)
	//_Count(1),
	//_InstanceCount(1)
{}

GLInstancedDraw::GLInstancedDraw(const GLBuffer::CtorParam * const & param) :
	GLDraw(param),
	_InstBuffer(GL_ELEMENT_ARRAY_BUFFER, param[2]),
	_Mode(GL_TRIANGLES)
	//_Count(1),
	//_InstanceCount(1)
{
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _InstBuffer.GetBufferO());

	glBindVertexArray(0);

}

void GLInstancedDraw::Initialize(const GLBuffer::CtorParam * const & param)
{
	GLDraw::Initialize(param);
	_InstBuffer.Initialize(GL_ELEMENT_ARRAY_BUFFER, param[2]);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _InstBuffer.GetBufferO());

	glBindVertexArray(0);
}

GLInstancedDraw::GLInstancedDraw(const std::array<GLBuffer::CtorParam, 3>& param):
	GLInstancedDraw(&(param[0]))
{
}

void GLInstancedDraw::Initialize(const std::array<GLBuffer::CtorParam, 3>& param)
{
	Initialize(&(param[0]));
}

void GLInstancedDraw::VtxAttribPointerSetup(const data::Shader* shader)
{
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VtxBuffer.GetBufferO());
	shader->VtxPointerSetup(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, _InstBuffer.GetBufferO());
	shader->VtxPointerSetup(1);

	glBindVertexArray(0);
}

void GLInstancedDraw::Draw(const GLsizei count, const GLsizei instanceCount) const
{
	glBindVertexArray(_VAO);

	glDrawElementsInstanced(_Mode, count, GL_UNSIGNED_INT, 0, instanceCount);

	glBindVertexArray(0);
}



GLIndirectDraw::GLIndirectDraw()
{
}

GLIndirectDraw::GLIndirectDraw(const GLBuffer::CtorParam(&param)[4]):
	GLInstancedDraw(param),
	_IndirectCmdBuffer(GL_DRAW_INDIRECT_BUFFER, param[3])
{
	glBindVertexArray(_VAO);

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _IndirectCmdBuffer.GetBufferO());

	glBindVertexArray(0);
}

void GLIndirectDraw::Initialize(const GLBuffer::CtorParam(&param)[4])
{
	GLInstancedDraw::Initialize(param);

	_IndirectCmdBuffer.Initialize(GL_DRAW_INDIRECT_BUFFER, param[3]);
}

void GLIndirectDraw::SetData(const std::vector<Render*> renders)
{
	if (renders.size() > 0)
	{
		const data::Shader* shader = renders[0]->GetMaterial()->GetShader();

		std::unordered_map<const data::Mesh*, std::vector<Render*>> meshRenders;

		std::for_each(renders.begin(), renders.end(), [&meshRenders](Render* r)
		{
			const data::Mesh* m = r->GetMesh();
			auto iter = meshRenders.find(m);
			if (iter != meshRenders.end())
				iter->second.push_back(r);
			else
				meshRenders.insert(std::make_pair(m, std::vector<Render*>{r}));
		});

		IndirectCommand cmd;

		GLuint & count = cmd.count;
		GLuint & instanceCount = cmd.instanceCount;
		GLuint& firstIndex = cmd.firstIndex;
		GLuint & baseVertex = cmd.baseVertex;
		GLuint & baseInstance = cmd.baseInstance;

		std::size_t cmdCount = 0;
		std::for_each(meshRenders.begin(), meshRenders.end(), [this, &shader, &cmd, &count, &instanceCount, &firstIndex, &baseVertex, &baseInstance, &cmdCount](const std::pair<const data::Mesh*, std::vector<Render*>>& mr)
		{
			const data::Mesh* m = mr.first;

			//vtx
			_VtxBuffer.SetData(shader->GetVtxVarInfo(0), m->GetVtxVars(), baseVertex);
			baseVertex += m->GetVtxAttribByteSize();

			//idx
			const GLVar* idxVar = m->GetIdxVar();
			_IdxBuffer.SetData(firstIndex, idxVar, idxVar->byteSize());
			firstIndex += idxVar->byteSize();
			count = idxVar->count();

			//inst
			const auto instVarInfo = shader->GetVtxVarInfo(1);
			if (instVarInfo.size() > 0)
			{
				const std::vector<Render*>& vRs = mr.second;
				const std::size_t stride = instVarInfo.begin()->stride;
				for (std::size_t i = 0; i < vRs.size(); i++)
				{
					baseInstance += i * stride;
					_InstBuffer.SetData(instVarInfo, vRs[i]->GetInstVar(), baseInstance);
				}

				instanceCount = vRs.size();
			}

			//indirect
			static constexpr std::size_t IndirectCommand_size = sizeof(IndirectCommand);
			_IndirectCmdBuffer.SetData(cmdCount * IndirectCommand_size, &cmd, IndirectCommand_size);
			cmdCount++;

		});
	}
}

void GLIndirectDraw::Release()
{
	//TODO
}

void GLIndirectDraw::Draw() const
{
	glBindVertexArray(_VAO);

	glDrawElementsIndirect(_Mode, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

GLIndirectDraw::IndirectCommand::IndirectCommand():
	count(0),
	instanceCount(0),
	firstIndex(0),
	baseVertex(0),
	baseInstance(0)
{
}

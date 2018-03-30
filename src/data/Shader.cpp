#include "Shader.h"
#include <algorithm>

using namespace gb::render::data;
using namespace gb::utils;


void Shader::from_lua(luatable_mapper & mapper)
{

}

void Shader::VtxPointerSetup()
{
	std::for_each(_VtxVarInfos.begin(), _VtxVarInfos.end(),
		[](const std::pair<string, VtxVarInfo>& vtxInfo)
	{
		const VtxVarInfo& info = vtxInfo.second;

		const GLuint baseIndex = info.index;
		const GLint size = info.size;
		const GLenum type = info.type;
		const GLsizei stride = info.stride;
		const GLsizei offset = info.offset;
		const std::uint32_t count = info.count;
		const std::uint32_t typeSize = info.type;
		const VtxVarBriefType briefType = info.briefType;
		//enum _varType
		//{
		//	Float = 0,
		//	Integer, Double
		//};
		//_varType vt = _varType::Float;
		//if (type != GL_DOUBLE)
		//{
		//	if(type != GL_BYTE && type != GL_UNSIGNED_BYTE 
		//		&& type != GL_SHORT && type != GB_UNSIGNED)
		//}
		//else
		//	vt = _varType::Double;
		for (std::uint32_t i = 0; i < count; i++)
		{
			const GLuint index = baseIndex + i;

			glEnableVertexAttribArray(index);
			if (briefType != VtxVarBriefType::Double)
			{
				if (briefType != VtxVarBriefType::Integer)
					glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)(offset + i * typeSize));
				else
					glVertexAttribIPointer(index, size, type, stride, (void*)(offset + i * typeSize));
			}
			else
				glVertexAttribLPointer(index, size, type, stride, (void*)(offset + i * typeSize));
		}
	});
}
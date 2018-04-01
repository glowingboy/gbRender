#include "Shader.h"
#include <algorithm>
#include <gbUtils/file.h>
#include <gbUtils/logger.h>

using namespace gb::render::data;
using namespace gb::utils;

VtxVarStub::VtxVarStub() :
	count{ 1 },
	divisor{ 0 }
{}

void VtxVarStub::from_lua(const gb::utils::luatable_mapper& mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_NAME))
	{
		name = mapper.get_string_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_NAME);
	}
	else
		logger::Instance().error("VtxVarStub::from_lua no Name found mapper@" + mapper.GetData());

	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_TYPE))
	{
		type = (VtxVarStubType)(mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_TYPE));
	}
	else
		logger::Instance().error("VtxVarStub::from_lua no Type found mapper@" + mapper.GetData());

	//optonal
	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT))
		count = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT);


	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR))
		divisor = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR);
}

GLint VtxVarStub::componentCount(const VtxVarStubType type)
{
	if (type == VtxVarStubType::Int)
		return 1;
	else if (type == VtxVarStubType::UInt)
		return 1;
	else if (type == VtxVarStubType::Float)
		return 1;
	else if (type == VtxVarStubType::Vec2)
		return 2;
	else if (type == VtxVarStubType::Vec3)
		return 3;
	else if (type == VtxVarStubType::Vec4)
		return 4;
	else if (type == VtxVarStubType::Mat4)
		return 4;
	else if (type == VtxVarStubType::Handleui64)
		return 1;

	assert(false);
	return 0;
}

GLenum VtxVarStub::glType(const VtxVarStubType type)
{
	if (type == VtxVarStubType::Int)
		return GL_INT;
	else if (type == VtxVarStubType::UInt)
		return GL_UNSIGNED_INT;
	else if (type == VtxVarStubType::Float)
		return GL_FLOAT;
	else if (type == VtxVarStubType::Vec2)
		return GL_FLOAT;
	else if (type == VtxVarStubType::Vec3)
		return GL_FLOAT;
	else if (type == VtxVarStubType::Vec4)
		return GL_FLOAT;
	else if (type == VtxVarStubType::Mat4)
		return GL_FLOAT;
	else if (type == VtxVarStubType::Handleui64)
		return GL_UNSIGNED_INT64_ARB;

	assert(false);
	return 0;
}

std::uint32_t VtxVarStub::typeCount(const VtxVarStubType type)
{
	if (type == VtxVarStubType::Int)
		return 1;
	else if (type == VtxVarStubType::UInt)
		return 1;
	else if (type == VtxVarStubType::Float)
		return 1;
	else if (type == VtxVarStubType::Vec2)
		return 1;
	else if (type == VtxVarStubType::Vec3)
		return 1;
	else if (type == VtxVarStubType::Vec4)
		return 1;
	else if (type == VtxVarStubType::Mat4)
		return 4;
	else if (type == VtxVarStubType::Handleui64)
		return 1;

	assert(false);
	return 0;
}

std::size_t VtxVarStub::typeSize(const VtxVarStubType type)
{
	size_t size = 0;
	if (type == VtxVarStubType::Int)
		size = sizeof(int);
	else if (type == VtxVarStubType::UInt)
		size = sizeof(unsigned int);
	else if (type == VtxVarStubType::Float)
		size = sizeof(float);
	else if (type == VtxVarStubType::Vec2)
		size = 2 * sizeof(float);
	else if (type == VtxVarStubType::Vec3)
		size = 3 * sizeof(float);
	else if (type == VtxVarStubType::Vec4)
		size = 4 * sizeof(float);
	else if (type == VtxVarStubType::Mat4)
		size = 4 * sizeof(float);
	else if (type == VtxVarStubType::Handleui64)
		size = sizeof(GLuint64);
	return size;
}

std::size_t VtxVarStub::byteSize(const VtxVarStubType type)
{
	size_t trueTypeSize = 0;
	if (type == VtxVarStubType::Int)
		trueTypeSize = sizeof(int);
	else if (type == VtxVarStubType::UInt)
		trueTypeSize = sizeof(unsigned int);
	else if (type == VtxVarStubType::Float)
		trueTypeSize = sizeof(float);
	else if (type == VtxVarStubType::Vec2)
		trueTypeSize = sizeof(float);
	else if (type == VtxVarStubType::Vec3)
		trueTypeSize = sizeof(float);
	else if (type == VtxVarStubType::Vec4)
		trueTypeSize = sizeof(float);
	else if (type == VtxVarStubType::Mat4)
		trueTypeSize = sizeof(float);
	else if (type == VtxVarStubType::Handleui64)
		trueTypeSize = sizeof(GLuint64);
	return trueTypeSize * componentCount(type) * ;
}
VtxVarStubInfo VtxVarStub::genInfo(const GLsizei stride, const GLsizei offset) const
{
	VtxVarStubInfo info;
	info.index = index;
	info.size = componentCount(type);
	const GLenum detailedType = glType(type);
	info.type = detailedType;
	info.stride = stride;
	info.offset = offset;
	info.count = count * typeCount(type);
	info.typeSize = typeSize(type);
	info.divisor = divisor;

	if (detailedType != GL_DOUBLE)
	{
		if (detailedType != GL_BYTE && detailedType != GL_UNSIGNED_BYTE
			&& detailedType != GL_SHORT && detailedType != GL_UNSIGNED_SHORT
			&& detailedType != GL_INT && detailedType != GL_UNSIGNED_INT)
			info.briefType = VtxVarBriefType::Integer;
		else
			info.briefType = VtxVarBriefType::Float;

	}
	else
		info.briefType = VtxVarBriefType::Double;

	return info;
}

const std::vector<std::string> Shader::_blockDelimiter
{
	{ 
		GB_RENDER_DATA_SHADER_INFO_BEGIN ,
		GB_RENDER_DATA_SHADER_INFO_END,
		GB_RENDER_DATA_SHADER_VERT_BEGIN,
		GB_RENDER_DATA_SHADER_VERT_END,
		GB_RENDER_DATA_SHADER_FRAG_BEGIN,
		GB_RENDER_DATA_SHADER_FRAG_END
	}
};

bool Shader::from_lua(luatable_mapper & mapper, const char* shaderName)
{
	assert(shaderName != nullptr);

	_Name = shaderName;

	file f(shaderName);
	const size_t size = f.size();
	char* buffer = new char[size + 1]{ '\0' };
	f.read(buffer, size);
	string s(buffer);

	const std::map<const std::string, std::string> blocks = s.extract_blocks(_blockDelimiter);

	std::map<const std::string, std::string>::const_iterator citr = blocks.find(GB_RENDER_DATA_SHADER_INFO_END);
	std::map<gb::utils::string, VtxVarStub> mpVtxVarStubs[2];
	if (citr != blocks.cend())
	{
		mapper.map_string(citr->second.c_str());
		if (mapper.has_key(GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS))
		{
			mapper.for_each_in([&mapper, &mpVtxVarStubs](const std::size_t idx)
			{
				VtxVarStub stub = mapper.get_table_by_idx<VtxVarStub>(idx);
				if (stub.divisor != 0)
					mpVtxVarStubs[0].insert(std::make_pair(std::move(stub.name), stub));
				else
					mpVtxVarStubs[1].insert(std::make_pair(std::move(stub.name), stub));

			}, GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS);
		}
		else
		{
			logger::Instance().error(string("Shader::from_lua no VtxVars Found shaderName@ ") + shaderName);
			return false;
		}
	}
	else
	{
		logger::Instance().error(string("Shader::from_lua no GB_RENDER_DATA_SHADER_INFO block found shaderName@") + shaderName);
		return false;
	}

	if (_compile(mapper.get_string_by_key(GB_RENDER_DATA_SHADER_VERT_END), mapper.get_string_by_key(GB_RENDER_DATA_SHADER_FRAG_END)))
	{
		for (std::uint8_t i = 0; i < 2; i++)
		{
			std::map<gb::utils::string, VtxVarStub>& stub = mpVtxVarStubs[i];

			GLsizei stride = 0;
			std::for_each(stub.begin(), stub.end(), [this](std::pair<gb::utils::string, VtxVarStub> & s)
			{
				VtxVarStub& vs = s.second;

				vs.index = GetVtxAttribLocation(s.first);

				stride += 
			});
		}
	}
	else
		return false;

	return true;
}

void Shader::VtxPointerSetup()
{
	for (std::uint8_t i = 0; i < 2; i++)
	{
		const std::unordered_map<gb::utils::string, VtxVarStubInfo>& info = _vtxVarInfos[i];

		std::for_each(info.begin(), info.end(),
			[](const std::pair<string, VtxVarStubInfo>& vtxInfo)
		{
			const VtxVarStubInfo& info = vtxInfo.second;

			const GLuint baseIndex = info.index;
			const GLint size = info.size;
			const GLenum type = info.type;
			const GLsizei stride = info.stride;
			const GLsizei offset = info.offset;
			const std::uint32_t count = info.count;
			const std::uint32_t typeSize = info.type;
			const VtxVarBriefType briefType = info.briefType;

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

}


bool Shader::_compile(const char* vert, const char* frag)
{
	GLint vertObj = -1;
	GLint fragObj = -1;
	vertObj = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertObj, 1, (const GLchar**)&vert, nullptr);

	glCompileShader(vertObj);
	if (_checkShaderCompile(vertObj) == GL_FALSE)
	{
		logger::Instance().error(string("Shader::_compile vert compiled error vert@ ") + vert + ", shader name" + _Name);
		return false;
	}


	fragObj = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragObj, 1, (const GLchar**)&frag, nullptr);

	glCompileShader(fragObj);
	if (_checkShaderCompile(fragObj) == GL_FALSE)
	{
		logger::Instance().error(string("Shader::_compile farg compiled error frag@ ") + frag + ", shader name" + _Name);
		return false;
	}

	_programObj = glCreateProgram();
	glAttachShader(_programObj, vertObj);
	glAttachShader(_programObj, fragObj);

	glLinkProgram(_programObj);
	if (_checkShaderLink(_programObj) == GL_FALSE)
	{
		logger::Instance().error(string("Shader::_compile program linked error vert@ ") + vert + ", frag@ " + frag + ", shader name" + _Name);
		return false;
	}

	return true;
}

GLint Shader::_checkShaderCompile(GLuint shader)
{
	GLint result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen != 0)
		{
			char* pInfo = new char[infoLen + 1]{ '\0' };
			glGetShaderInfoLog(shader, infoLen, NULL, pInfo);

			logger::Instance().error(pInfo);
			GB_SAFE_DELETE_ARRAY(pInfo);
		}
	}
	return result;
}
GLint Shader::_checkShaderLink(GLuint program)
{
	GLint result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen != 0)
		{
			char* pInfo = new char[infoLen + 1]{ '\0' };
			glGetProgramInfoLog(program, infoLen, NULL, pInfo);
			logger::Instance().error(pInfo);
			GB_SAFE_DELETE_ARRAY(pInfo);
		}
	}
	return result;
}

GLint Shader::GetVtxAttribLocation(const char* name)
{
	GLint location = glGetAttribLocation(_programObj, name);
	if (location != -1)
		return location;
	else
	{
		logger::Instance().error(string("Shader::GetVtxAttribLocation error. name@") + name + ", shader name" + _Name);
		return -1;
	}
	
}
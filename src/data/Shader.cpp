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
		type = (slType)(mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_TYPE));
	}
	else
		logger::Instance().error("VtxVarStub::from_lua no Type found mapper@" + mapper.GetData());

	//optional
	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT))
		count = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT);


	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR))
		divisor = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR);

	byteSize = typeSize(type) * typeCount(type) * count;
}

GLint VtxVarStub::componentCount(const slType type)
{
	if (type == slType::Int)
		return 1;
	else if (type == slType::UInt)
		return 1;
	else if (type == slType::Float)
		return 1;
	else if (type == slType::Vec2)
		return 2;
	else if (type == slType::Vec3)
		return 3;
	else if (type == slType::Vec4)
		return 4;
	else if (type == slType::Mat4)
		return 4;
	else if (type == slType::Handleui64)
		return 1;

	assert(false);
	return 0;
}

GLenum VtxVarStub::glType(const slType type)
{
	if (type == slType::Int)
		return GL_INT;
	else if (type == slType::UInt)
		return GL_UNSIGNED_INT;
	else if (type == slType::Float)
		return GL_FLOAT;
	else if (type == slType::Vec2)
		return GL_FLOAT;
	else if (type == slType::Vec3)
		return GL_FLOAT;
	else if (type == slType::Vec4)
		return GL_FLOAT;
	else if (type == slType::Mat4)
		return GL_FLOAT;
	else if (type == slType::Handleui64)
		return GL_UNSIGNED_INT64_ARB;

	assert(false);
	return 0;
}

std::uint32_t VtxVarStub::typeCount(const slType type)
{
	if (type == slType::Int)
		return 1;
	else if (type == slType::UInt)
		return 1;
	else if (type == slType::Float)
		return 1;
	else if (type == slType::Vec2)
		return 1;
	else if (type == slType::Vec3)
		return 1;
	else if (type == slType::Vec4)
		return 1;
	else if (type == slType::Mat4)
		return 4;
	else if (type == slType::Handleui64)
		return 1;

	assert(false);
	return 0;
}

std::size_t VtxVarStub::typeSize(const slType type)
{
	size_t size = 0;
	if (type == slType::Int)
		size = sizeof(int);
	else if (type == slType::UInt)
		size = sizeof(unsigned int);
	else if (type == slType::Float)
		size = sizeof(float);
	else if (type == slType::Vec2)
		size = 2 * sizeof(float);
	else if (type == slType::Vec3)
		size = 3 * sizeof(float);
	else if (type == slType::Vec4)
		size = 4 * sizeof(float);
	else if (type == slType::Mat4)
		size = 4 * sizeof(float);
	else if (type == slType::Handleui64)
		size = sizeof(GLuint64);
	return size;
}

//std::size_t VtxVarStub::byteSize(const VtxVarStubType type)
//{
//	size_t trueTypeSize = 0;
//	if (type == VtxVarStubType::Int)
//		trueTypeSize = sizeof(int);
//	else if (type == VtxVarStubType::UInt)
//		trueTypeSize = sizeof(unsigned int);
//	else if (type == VtxVarStubType::Float)
//		trueTypeSize = sizeof(float);
//	else if (type == VtxVarStubType::Vec2)
//		trueTypeSize = sizeof(float);
//	else if (type == VtxVarStubType::Vec3)
//		trueTypeSize = sizeof(float);
//	else if (type == VtxVarStubType::Vec4)
//		trueTypeSize = sizeof(float);
//	else if (type == VtxVarStubType::Mat4)
//		trueTypeSize = sizeof(float);
//	else if (type == VtxVarStubType::Handleui64)
//		trueTypeSize = sizeof(GLuint64);
//	return trueTypeSize * componentCount(type) * typesize;
//}

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
	info.name = std::move(name);

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
	std::map<const std::string, std::string>::const_iterator info = blocks.find(GB_RENDER_DATA_SHADER_INFO_END);
	std::map<const std::string, std::string>::const_iterator vert = blocks.find(GB_RENDER_DATA_SHADER_VERT_END);
	std::map<const std::string, std::string>::const_iterator frag = blocks.find(GB_RENDER_DATA_SHADER_FRAG_END);

	if (info != blocks.cend() && vert != blocks.cend() && frag != blocks.cend())
	{
		if (!_compile(vert->second.c_str(), frag->second.c_str()))
		{
			logger::Instance().error(string("shader compiled error shaderName@ ") + shaderName);
			return false;
		}


		if (mapper.map_string(info->second.c_str()))
		{
			std::vector<VtxVarStub> vtxVarStubs[2];
			GLsizei strides[2] = { 0 };

			//VtxVars
			if (mapper.has_key(GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS))
			{
				mapper.for_each_in([&mapper, &vtxVarStubs, &strides, this](const std::size_t idx)
				{
					VtxVarStub stub = mapper.get_table_by_idx<VtxVarStub>(idx);
					stub.index = GetVtxAttribLocation(stub.name);
					if (stub.divisor != 0)
					{
						vtxVarStubs[0].push_back(stub);
						strides[0] += stub.byteSize;
					}
					else
					{
						vtxVarStubs[1].push_back(stub);
						strides[1] += stub.byteSize;
					}


				}, GB_RENDER_DATA_SHADER_INFO_KEY_VTXVARS);
			}
			else
			{
				logger::Instance().error(string("Shader::from_lua no VtxVars Found shaderName@ ") + shaderName);
				return false;
			}

			//UniformVars

			//GLCfg
		}
		

		mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_INFO_KEY_GLCFG, _GLCfg);
		

		if (_compile(vert->second.c_str(), frag->second.c_str()))
		{
			for (std::uint8_t i = 0; i < 2; i++)
			{
				std::vector<VtxVarStub>& stub = vtxVarStubs[i];
				const GLsizei stride = strides[i];
				std::vector<VtxVarStubInfo>& info = _vtxVarInfos[i];
				GLsizei offset = 0;
				std::for_each(stub.begin(), stub.end(), [this, &offset, &stride, &info](VtxVarStub & vs)
				{
					info.push_back(vs.genInfo(GetVtxAttribLocation(vs.name), stride, offset));

					offset += vs.byteSize;
				});
			}
		}
		else
			return false;

	}
	else
	{
		logger::Instance().error(string("Shader::from_lua shader incomplete, missing one or more blocks of {info, vert, frag} shaderName@ ") + shaderName);
		return false;
	}
	

	

	return true;
}

void Shader::VtxPointerSetup() const
{
	for (std::uint8_t i = 0; i < 2; i++)
	{
		const std::vector<VtxVarStubInfo>& info = _vtxVarInfos[i];

		std::for_each(info.begin(), info.end(),
			[](const VtxVarStubInfo& info)
		{

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

void Shader::Use() const
{
	glUseProgram(_programObj);
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

GLint Shader::GetVtxAttribLocation(const char* name) const
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

void GLCfg::Blend::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_ENABLED, enabled);
	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_SFACTOR, gb_cast<lua_Integer>(sfactor));

		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_DFACTOR, gb_cast<lua_Integer>(dfactor));
	}
}

void GLCfg::DepthTest::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_ENABLED, enabled);

	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_FUNC, gb_cast<lua_Integer>(func));
	}
}

void GLCfg::AlphaTest::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_ENABLED, enabled);

	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_FUNC, gb_cast<lua_Integer>(func));

		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_REF, gb_cast<lua_Integer>(ref));
	}
}

GLCfg::GLCfg():
	blend{false, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
	depthTest{ true, GL_LEQUAL },
	alphaTest{ false, GL_GREATER, 0.0f },
	polygonMode(GL_FILL)
{}

void GLCfg::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND, blend);
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST, depthTest);
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST, alphaTest);
	mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_POLYGONMODE, gb_cast<lua_Integer>(polygonMode));

}
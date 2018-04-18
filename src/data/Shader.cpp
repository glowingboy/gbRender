#include "Shader.h"
#include <algorithm>
#include <gbUtils/file.h>
#include <gbUtils/logger.h>
#include "../resource/Resource.h"

using namespace gb::render::data;
using namespace gb::utils;
using namespace gb;

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
		sl_type = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_TYPE);
	}
	else
		logger::Instance().error("VtxVarStub::from_lua no Type found mapper@" + mapper.GetData());

	//optional
	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT))
		count = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_COUNT);


	if (mapper.has_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR))
		divisor = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_VTXVARSTUB_KEY_DIVISOR);

	byteSize = slType::typeSize(sl_type) * slType::typeCount(sl_type) * count;
}

GLint slType::componentCount(const std::uint8_t type)
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
	else if (type == slType::Sampler)
		return 1;
	assert(false);
	return 0;
}

GLenum slType::glType(const std::uint8_t type)
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
	else if (type == slType::Sampler)
		return GL_INT;
	assert(false);
	return 0;
}

std::uint32_t slType::typeCount(const std::uint8_t type)
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
	else if (type == slType::Sampler)
		return 1;
	assert(false);
	return 0;
}

std::size_t slType::typeSize(const std::uint8_t type)
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
	else if (type == slType::Sampler)
		size = sizeof(int);
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
	info.size = slType::componentCount(sl_type);
	const GLenum detailedType = slType::glType(sl_type);
	info.type = detailedType;
	info.stride = stride;
	info.offset = offset;
	info.count = count * slType::typeCount(sl_type);
	info.typeSize = slType::typeSize(sl_type);
	info.divisor = divisor;
	info.name = std::move(name);

	if (detailedType != GL_DOUBLE)
	{
		if (detailedType != GL_BYTE && detailedType != GL_UNSIGNED_BYTE
			&& detailedType != GL_SHORT && detailedType != GL_UNSIGNED_SHORT
			&& detailedType != GL_INT && detailedType != GL_UNSIGNED_INT)
			info.briefType = VtxVarBriefType::Float;
		else
			info.briefType = VtxVarBriefType::Integer;

	}
	else
		info.briefType = VtxVarBriefType::Double;

	return info;
}

UniformVarStub::UniformVarStub() :
	index(-1),
	typeSize(0),
	count(1)
{}



void UniformVarStub::from_lua(const gb::utils::luatable_mapper& mapper)
{
	if (mapper.has_key(GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_NAME))
		name = mapper.get_string_by_key(GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_NAME);
	else
	{
		logger::Instance().error("UniformVarStub::from_lua no Key 'Name' found" + mapper.GetData());
		return;
	}

	if (mapper.has_key(GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_TYPE))
	{
		sl_type = mapper.get_integer_by_key(GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_TYPE);
		typeSize = slType::typeSize(sl_type);
	}
	else
	{
		logger::Instance().error("UniformVarStub::from_lua no Key 'Name' found" + mapper.GetData());
		return;
	}

	mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_UNIFORMVARSTUB_KEY_COUNT, count);
}

UniformVar UniformVarStub::genUniformVar() const
{
	return UniformVar(index, typeSize, count * slType::typeCount(sl_type), sl_type);
}

UniformVar::UniformVar(const GLint index_, const std::size_t typeSize, const std::size_t count_, const std::uint8_t sl_type) :
	index(index_),
	byteSize(typeSize * count_),
	count(count_),
	_setter(nullptr),
	_lua_getter(nullptr),
	textureObjs(nullptr)
{
	data = new char[byteSize];

	if (sl_type == slType::Float)
	{
		_setter = Shader::SetUniform1f;
		_lua_getter = &UniformVar::_lua_getter_numbers;
	}
	else if (sl_type == slType::Int)
	{
		_setter = Shader::SetUniform1i;
		_lua_getter = &UniformVar::_lua_getter_integers;
	}
	else if (sl_type == slType::UInt)
	{
		_setter = Shader::SetUniform1ui;
		_lua_getter = &UniformVar::_lua_getter_integers;
	}
	else if (sl_type == slType::Vec2)
	{
		_setter = Shader::SetUniform2f;
		_lua_getter = &UniformVar::_lua_getter_numbers;
	}
	else if (sl_type == slType::Vec3)
	{
		_setter = Shader::SetUniform3f;
		_lua_getter = &UniformVar::_lua_getter_numbers;
	}
	else if (sl_type == slType::Vec4)
	{
		_setter = Shader::SetUniform4f;
		_lua_getter = &UniformVar::_lua_getter_numbers;
	}
	else if (sl_type == slType::Mat4)
	{
		_setter = Shader::SetUniform4f;
		_lua_getter = &UniformVar::_lua_getter_numbers;
	}
	else if (sl_type == slType::Handleui64)
	{
		_setter = Shader::SetUniform1i;
		_lua_getter = &UniformVar::_lua_getter_integers;
	}
	else if (sl_type == slType::Sampler)
	{
		_setter = Shader::SetUniform1i;
		_lua_getter = &UniformVar::_lua_getter_samplers;
		textureObjs = new UniformTextureVar{ {0} };
	}
		
}
UniformVar::~UniformVar()
{
	GB_SAFE_DELETE_ARRAY(data);
	GB_SAFE_DELETE_ARRAY(textureObjs);
}
UniformVar::UniformVar(UniformVar && other) :
	index(other.index),
	byteSize(other.byteSize),
	count(other.count),
	data(other.data),
	textureObjs(other.textureObjs),
	_setter(other._setter),
	_lua_getter(other._lua_getter)
{
	other.data = nullptr;
	other.textureObjs = nullptr;
}

void UniformVar::SetData(const void* data_, const std::size_t size)
{
	const std::size_t minSize = byteSize > size ? size : byteSize;

	std::memcpy(data, data_, minSize);
}

void UniformVar::SetTextureObjs(const UniformTextureVar* texObjs, const std::size_t count_)
{
	const std::size_t minCount = count > count_ ? count_ : count;

	std::memcpy(textureObjs, texObjs, minCount * sizeof(UniformTextureVar));
}

void UniformVar::Update() const
{
	if (_setter != nullptr)
	{
		_setter(index, count, data);

		if (textureObjs != nullptr)
		{
			std::int32_t* texUnit = (std::int32_t*)data;
			for (std::uint32_t i = 0; i < count; i++)
			{
				glActiveTexture(GL_TEXTURE0 + texUnit[i]);
				const UniformTextureVar& texVar = textureObjs[i];
				glBindTexture(texVar.target, texVar.obj);
			}
			
		}
	}
	else
		logger::Instance().error("UniformVar::Update _setter is nullptr");
}
//void UniformVar::_lua_getter_integer(const gb::utils::luatable_mapper & mapper, const char * name)
//{
//	if (mapper.has_key(name))
//	{
//		lua_Integer ret = mapper.get_integer_by_key(name);
//		if (!SetData(&ret, sizeof(lua_Integer)))
//			logger::Instance().error(string("UniformVar::_lua_getter_integer name@ ") + name + ", mapper@ " + mapper.GetData());
//	}
//}
void UniformVar::_lua_getter_integers(const gb::utils::luatable_mapper & mapper, const char * name)
{
	if (mapper.has_key(name))
	{
		std::vector<lua_Integer> ret = mapper.get_integers_by_key(name);
		SetData(ret.data(), sizeof(lua_Integer) * ret.size());	
	}
}
//void UniformVar::_lua_getter_number(const gb::utils::luatable_mapper & mapper, const char * name)
//{
//	if (mapper.has_key(name))
//	{
//		lua_Number ret = mapper.get_number_by_key(name);
//		if (!SetData(&ret, sizeof(lua_Number)))
//			logger::Instance().error(string("UniformVar::_lua_getter_number name@ ") + name + ", mapper@ " + mapper.GetData());
//	}
//}
void UniformVar::_lua_getter_numbers(const gb::utils::luatable_mapper & mapper, const char * name)
{
	if (mapper.has_key(name))
	{
		std::vector<lua_Number> ret = mapper.get_numbers_by_key(name);
		SetData(ret.data(), sizeof(lua_Number) * ret.size());
	}
}

void UniformVar::_lua_getter_samplers(const gb::utils::luatable_mapper & mapper, const char * name)
{
	std::vector<string> ret = mapper.get_strings_by_key(name);
	const std::size_t minCount = ret.size() > count ? count : ret.size();

	for (std::size_t i = 0; i < minCount; i++)
	{
		Texture * tex = resource::Res<Texture>::Instance().Get(ret[i]);
		textureObjs[i] = { tex->GetTarget(), tex->GetTextureObj() };
	}
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

	utils::file f(shaderName);
	const size_t size = f.size();
	char* buffer = new char[size + 1]{ '\0' };
	f.read(buffer, size);
	string s(buffer);
	GB_SAFE_DELETE_ARRAY(buffer);

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

					if (stub.divisor == 0)
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

				for (std::uint8_t i = 0; i < 2; i++)
				{
					std::vector<VtxVarStub>& stub = vtxVarStubs[i];
					const GLsizei stride = strides[i];
					std::vector<VtxVarStubInfo>& info = _vtxVarInfos[i];
					GLsizei offset = 0;
					std::for_each(stub.begin(), stub.end(), [this, &offset, &stride, &info](VtxVarStub & vs)
					{
						info.push_back(vs.genInfo(stride, offset));

						offset += vs.byteSize;
					});
				}
			}
			else
			{
				logger::Instance().error(string("Shader::from_lua no VtxVars Found shaderName@ ") + shaderName);
				return false;
			}

			//UniformVars
			if (mapper.has_key(GB_RENDER_DATA_SHADER_INFO_KEY_UNIFORMVARS))
				mapper.for_each_in([&mapper, &vtxVarStubs, &strides, this](const std::size_t idx)
				{
					UniformVarStub stub = mapper.get_table_by_idx<UniformVarStub>(idx);
					stub.index = GetUniformLocation(stub.name);

					_uniformVarSubs.push_back(std::move(stub));

				}, GB_RENDER_DATA_SHADER_INFO_KEY_UNIFORMVARS);


			//GLCfg
			mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_INFO_KEY_GLCFG, _GLCfg);

			//Misc
			mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_INFO_KEY_MISC, _Misc);
		}
	}
	else
	{
		logger::Instance().error(string("Shader::from_lua shader incomplete, missing one or more blocks of {info, vert, frag} shaderName@ ") + shaderName);
		return false;
	}

	return true;
}

void Shader::VtxPointerSetup(const std::uint8_t idx, const GLuint vbo) const
{
	assert(idx < 2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const std::vector<VtxVarStubInfo>& info = _vtxVarInfos[idx];

	std::for_each(info.begin(), info.end(),
		[](const VtxVarStubInfo& info)
	{

		const GLuint baseIndex = info.index;
		const GLint size = info.size;
		const GLenum type = info.type;
		const GLsizei stride = info.stride;
		const GLsizei offset = info.offset;
		const std::uint32_t count = info.count;
		const std::uint32_t typeSize = info.typeSize;
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

			glVertexAttribDivisor(index, info.divisor);
		}
	});

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	assert(name != nullptr);

	GLint location = glGetAttribLocation(_programObj, name);
	if (location != -1)
		return location;
	else
	{
		logger::Instance().error(string("Shader::GetVtxAttribLocation error. name@") + name + ", shader name" + _Name);
		return -1;
	}

}

GLint Shader::GetUniformLocation(const char* name) const
{
	assert(name != nullptr);

	GLint location = glGetUniformLocation(_programObj, name);
	if (location != -1)
		return location;
	else
	{
		logger::Instance().error("Shader::GetUniformLocation no specified uniform found name@ " + string(name));
		return -1;
	}
}

std::unordered_map<gb::utils::string, UniformVar> Shader::GenUniformVars() const
{
	auto _getMaxUnits = []()->GLint
	{
		GLint maxTexUnits = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);
		return maxTexUnits;
	};
	static const GLuint maxTexUnits = _getMaxUnits();

	std::unordered_map<gb::utils::string, UniformVar> ret;

	std::uint32_t baseTexUnit = 0;
	std::for_each(_uniformVarSubs.begin(), _uniformVarSubs.end(), [&ret, &baseTexUnit](const UniformVarStub& stub)
	{
		auto var = ret.insert(std::make_pair(stub.name, stub.genUniformVar()));
		UniformVar & uVar = var.first->second;
		std::int32_t* texUnits = (std::int32_t*)uVar.data;
		if (uVar.textureObjs != nullptr)
		{
			for (std::uint32_t i = 0; i < uVar.count; i++)
			{
				if (baseTexUnit <= maxTexUnits)
				{
					texUnits[i] = baseTexUnit;
					baseTexUnit++;
				}
				else
					assert(false);
			
			}	
		}		
	});

	return ret;
}

const std::vector<VtxVarStubInfo>& Shader::GetVtxVarInfo(std::uint8_t idx) const
{
	assert(idx < 2);

	return _vtxVarInfos[idx];
}

#define _GB_RENDER_DATA_SHADER_SET_UNIFORM_DEF_(type, valueType) \
	void Shader::SetUniform1##type(const GLint location, const GLsizei count, const void* value) \
	{ \
		assert(location != -1); \
		glUniform1##type##v(location, count, (valueType*)value); \
	} \
	void Shader::SetUniform2##type(const GLint location, const GLsizei count, const void* value) \
	{ \
		assert(location != -1); \
		glUniform2##type##v(location, count, (valueType*)value); \
	} \
	void Shader::SetUniform3##type(const GLint location, const GLsizei count, const void* value) \
	{ \
		assert(location != -1); \
		glUniform3##type##v(location, count, (valueType*)value); \
	} \
	void Shader::SetUniform4##type(const GLint location, const GLsizei count, const void* value) \
	{ \
		assert(location != -1); \
		glUniform4##type##v(location, count, (valueType*)value); \
	} 

_GB_RENDER_DATA_SHADER_SET_UNIFORM_DEF_(f, GLfloat);
_GB_RENDER_DATA_SHADER_SET_UNIFORM_DEF_(i, GLint);
_GB_RENDER_DATA_SHADER_SET_UNIFORM_DEF_(ui, GLuint);

void Shader::SetUniformMatrix4f(const GLint location, const GLsizei count, const void* value)
{
	assert(location != -1);
	glUniformMatrix4fv(location, count, GL_FALSE, (GLfloat*)value);
}

void GLCfg::Blend::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_ENABLED, enabled);
	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_SFACTOR, sfactor);

		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND_DFACTOR, dfactor);

	}
}

void GLCfg::DepthTest::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_ENABLED, enabled);

	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST_FUNC, func);
	}
}

void GLCfg::AlphaTest::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_boolean_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_ENABLED, enabled);

	if (enabled)
	{
		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_FUNC, func);

		mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST_REF, ref);
	}
}

GLCfg::GLCfg() :
	blend{ false, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA },
	depthTest{ true, GL_LEQUAL },
	alphaTest{ false, GL_GREATER, 0.0f },
	polygonMode(GL_FILL)
{}

void GLCfg::from_lua(const gb::utils::luatable_mapper& mapper)
{
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_BLEND, blend);
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_DEPTHTEST, depthTest);
	mapper.checkout_table_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_ALPHATEST, alphaTest);
	mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_GLCFG_KEY_POLYGONMODE, polygonMode);

}

Shader::Misc::Misc():
	renderQueue(GB_RENDER_DATA_SHADER_RENDERQUEUE_OPAQUE)
{
}

void Shader::Misc::from_lua(const gb::utils::luatable_mapper & mapper)
{
	mapper.checkout_integer_by_key(GB_RENDER_DATA_SHADER_MISC_KEY_RENDERQUEUE, renderQueue);
}

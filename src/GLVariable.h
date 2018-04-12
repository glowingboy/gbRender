#pragma once

#include "RenderNS.h"
#include <cstring>
#include <vector>
#include "Type.h"
#include "../data/Shader.h"
// vertex variable
#define GB_RENDER_VTXVAR_POS "gb_vtxVar_pos"
#define GB_RENDER_VTXVAR_IDX "gb_vtxVar_idx"
#define GB_RENDER_VTXVAR_UV "gb_vtxVar_uv"
#define GB_RENDER_VTXVAR_COLOR "gb_vtxVar_color"

// instance variable
#define GB_RENDER_INSTVAR_MVP "gb_instVar_mvp"

// uniform variable
#define GB_RENDER_UNIFORMVAR_LIGHTNESS "gb_uniformVar_lightness"

GB_RENDER_NS_BEGIN

struct GLVar
{
public:
	GLVar(const std::uint8_t unitSize);
    GLVar(const std::uint8_t unitSize, const std::size_t capacity);
	GLVar(GLVar && o);
	GLVar(const GLVar & o);
	template <typename T>
	GLVar(const std::vector<T> & vec) :
		_unitSize(sizeof(T)),
		_count(vec.size()),
		_capacity(_count)
	{
		_byteSize = _unitSize * _capacity;
		_data = new char[_byteSize];

		std::memcpy(_data, vec.data(), _byteSize);
	}
    
	GLVar(const gb::render::data::VtxVarStubInfo& info);
    ~GLVar();
public:
    void append(const void* data, const std::size_t count);
	
	void set(const void* data, const std::size_t idx = 0);
    const char* data() const;
	std::uint8_t unitSize() const;
    std::size_t count() const;
	std::size_t byteSize()const;
	void clear();
	void reserve(const std::size_t capacity);
	
private:
    uint8 _unitSize;
	std::size_t _count;
	std::size_t _capacity;
    char* _data;

	std::size_t _byteSize;
};


GB_RENDER_NS_END

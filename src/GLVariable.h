#pragma once

#include "RenderNS.h"
#include <cstring>
#include <vector>
#include "Type.h"
// vertex variable
#define GB_RENDER_VTXVAR_POS "gb_vtxVar_pos"
#define GB_RENDER_VTXVAR_IDX "gb_vtxVar_idx"
#define GB_RENDER_VTXVAR_UV "gb_vtxVar_uv"
#define GB_RENDER_VTXVAR_COLOR "gb_vtxVar_color"

// instance variable
#define GB_RENDER_INSTVAR_MVP "gb_instVar_mvp"

// uniform variable
#define GB_RENDER_UNIVAR_LIGHTNESS "gb_uniVar_lightness"

GB_RENDER_NS_BEGIN

class GLVar
{
public:
    GLVar(const uint8 unitSize, const size_t capacity);
	GLVar(GLVar && o);
	GLVar(const GLVar & o);
	template <typename T>
	GLVar(const std::vector<T> & vec) :
		_unitSize(sizeof(T)),
		_count(vec.size()),
		_capacity(_count)
	{
		const size_t size = _unitSize * _capacity;
		_data = new char[size];

		std::memcpy(_data, vec.data(), size);
	}
    
    ~GLVar();
public:
    void append(void* data, const size_t count);
    const char* data() const;
    uint8 unitSize() const;
    size_t count() const;
private:
    uint8 _unitSize;
    size_t _count;
    size_t _capacity;
    
    char* _data;
};


GB_RENDER_NS_END

#pragma once

#include "RenderNS.h"
#include <cstring>
#include <gbUtils/vector.h>
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

class vtxVar
{
public:
    vtxVar(const uint8 unitSize, const size_t capacity);
    template <typename T>
    vtxVar(gb::utils::vector<T> && vec):
	_unitSize(sizeof(T)),
	_count(vec.count()),
	_capacity(vec.capacity()),
	_data(vec.move_data())
	{}
    ~vtxVar();
public:
    void append(void* data, const size_t count);
    const void* data() const;
    uint8 unitSize() const;
    size_t count() const;
private:
    uint8 _unitSize;
    size_t _count;
    size_t _capacity;
    
	void* _data;
};


GB_RENDER_NS_END

#include "GLVariable.h"

using namespace gb::render;

GLVar::GLVar(const uint8 unitSize, const std::size_t capacity):
    _unitSize(unitSize),
    _count(0),
    _capacity(capacity),
	_byteSize(0)
{
    _data = new char[unitSize * capacity];
}

GLVar::GLVar(GLVar && o) :
	_unitSize(o._unitSize),
	_count(o._count),
	_capacity(o._capacity),
	_data(o._data),
	_byteSize(o._byteSize)
{
	o._data = nullptr;
}

GLVar::GLVar(const GLVar & o):
	_unitSize(o._unitSize),
	_count(o._count),
	_capacity(o._count),
	_byteSize(o._byteSize)
{
	const std::size_t size = _unitSize * _capacity;
	_data = new char[size];

	std::memcpy(_data, o._data, size);
}

GLVar::GLVar(const gb::render::data::VtxVarStubInfo & info):
	_unitSize(info.count * info.typeSize),
	_count(1),
	_capacity(1),
	_byteSize(info.count * info.typeSize)
{
	_data = new char[_unitSize] {0};
}

GLVar::~GLVar()
{
    delete _data;
    _data = nullptr;
}

void GLVar::append(const void* data, const std::size_t count)
{
	const std::size_t newCount = _count + count;
	if (newCount <= _capacity)
	{
		std::memcpy(_data + _count * _unitSize, data, count * _unitSize);
		_count = newCount;
		_byteSize = _count * _unitSize;
	}
    else
    {
		_capacity = 2 * _count;
		char * newData = new char[ _capacity * _unitSize];

		// old data
		std::memcpy(newData, _data, _count * _unitSize);

		delete _data;
		_data = newData;

		append(data, count);
    }

	
}

void GLVar::set(const void * data, const std::size_t idx)
{
	assert(idx < _count);
	std::memcpy(_data + idx * _unitSize, data, _unitSize);
}

const char* GLVar::data() const
{
    return _data;
}


uint8 GLVar::unitSize() const
{
    return _unitSize;
}
std::size_t GLVar::count() const
{
    return _count;
}

std::size_t GLVar::byteSize() const
{
	return _byteSize;
}

#include "GLVariable.h"

using namespace gb::render;

GLVar::GLVar(const uint8 unitSize, const std::size_t capacity):
    _unitSize(unitSize),
    _count(0),
    _capacity(capacity)
{
    _data = new char[unitSize * capacity];
}

GLVar::GLVar(GLVar && o) :
	_unitSize(o._unitSize),
	_count(o._count),
	_capacity(o._capacity),
	_data(o._data)
{
	o._data = nullptr;
}

GLVar::GLVar(const GLVar & o):
	_unitSize(o._unitSize),
	_count(o._count),
	_capacity(o._count)
{
	const std::size_t size = _unitSize * _capacity;
	_data = new char[size];

	std::memcpy(_data, o._data, size);
}

GLVar::~GLVar()
{
    delete _data;
    _data = nullptr;
}

void GLVar::append(void* data, const std::size_t count)
{
    const std::size_t oldSize = _count * _unitSize;
    _count += count;
    
    if(_count <= _capacity)
	std::memcpy(_data + oldSize, data, count * _unitSize);
    else
    {
	_capacity = 2 * _count;
	char * newData = new char[ _capacity * _unitSize];

	// old data
	std::memcpy(newData, _data, oldSize);

	// new data
	std::memcpy(newData + oldSize, data, count * _unitSize);

	delete _data;
	_data = newData;
    }
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

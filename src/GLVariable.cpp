#include "GLVariable.h"

using namespace gb::render;


GLVar::GLVar(const std::size_t unitSize):
	_unitSize(unitSize),
	_count(0),
	_capacity(1),
	_data(nullptr),
	_byteSize(0)
	
{
	_data = new char[unitSize * _capacity];
}

GLVar::GLVar(const std::size_t unitSize, const std::size_t capacity) :
	_unitSize(unitSize),
	_count(0),
	_capacity(capacity),
	_byteSize(0)
{
    _data = new char[unitSize * capacity]{0};
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
	_capacity(o._capacity),
	_byteSize(o._byteSize)
{
	const std::size_t size = _unitSize * _capacity;
	_data = new char[size];

	std::memcpy(_data, o._data, _byteSize);
}

GLVar::GLVar(const gb::render::data::VtxVarStubInfo & info):
	_unitSize(info.count * info.typeSize),
	_count(1),
	_capacity(1),
	_byteSize(info.count * info.typeSize)
{
	_data = new char[_unitSize] {};
}

GLVar::~GLVar()
{
	GB_SAFE_DELETE_ARRAY(_data);
}

static std::size_t _findLeastCapacity(std::size_t oldCapacity, const std::size_t count)
{
	if (oldCapacity < count)
	{
		oldCapacity *= 2;
		return _findLeastCapacity(oldCapacity, count);
	}
	else
		return oldCapacity;
}

void GLVar::append(const void* data, const GLsizei count)
{
	assert(data != nullptr);
	const GLsizei newCount = _count + count;
	if (newCount <= _capacity)
	{
		std::memcpy(_data + _count * _unitSize, data, count * _unitSize);
		_count = newCount;
		_byteSize = _count * _unitSize;
	}
    else
    {
		assert(_capacity != 0);
		_capacity = _findLeastCapacity(_capacity, count);
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
	assert(data != nullptr && idx < _count);
	std::memcpy(_data + idx * _unitSize, data, _unitSize);
}

const char* GLVar::data() const
{
    return _data;
}


std::size_t GLVar::unitSize() const
{
    return _unitSize;
}
GLsizei GLVar::count() const
{
    return _count;
}

std::size_t GLVar::byteSize() const
{
	return _byteSize;
}

void GLVar::clear()
{
	GB_SAFE_DELETE_ARRAY(_data);

	_count = 0;
	_capacity = 0;
}

void GLVar::reserve(const std::size_t capacity)
{
	if (_capacity < capacity)
	{
		char* newData = new char[capacity * _unitSize] {};
		if (_data != nullptr)
		{
			std::memcpy(newData, _data, _count * _unitSize);
			GB_SAFE_DELETE_ARRAY(_data);
		}

		_data = newData;
		_capacity = capacity;
	}
}

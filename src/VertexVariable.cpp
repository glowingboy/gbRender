#include "VertexVariable.h"

using namespace gb::render;

vtxVar::vtxVar(const uint8 unitSize, const size_t capacity):
    _unitSize(unitSize),
    _count(0),
    _capacity(capacity)
{
    size_t size = unitSize * capacity;
    _data = new char[size]{0};
}

vtxVar::~vtxVar()
{
    delete _data;
    _data = nullptr;
}

void vtxVar::append(void* data, const size_t count)
{
    const std::size_t oldSize = _count * _unitSize;
    _count += count;
    
    if(_count <= _capacity)
	std::memcpy((char*)_data + oldSize, data, count * _unitSize);
    else
    {
	_capacity = 2 * _count;
	char * newData = new char[ _capacity * _unitSize]{0};

	// old data
	std::memcpy(newData, _data, oldSize);

	// new data
	std::memcpy(newData + oldSize, data, count * _unitSize);

	delete _data;
	_data = newData;
    }
}

const void* vtxVar::data() const
{
    return _data;
}


std::uint8_t vtxVar::unitSize() const
{
    return _unitSize;
}
std::size_t vtxVar::count() const
{
    return _count;
}

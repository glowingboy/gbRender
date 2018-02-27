#include "Transform.h"

using namespace gb::utils;
using namespace gb::render::data;
using namespace gb;

Transform::Transform():
	scale(1.0f)
{

}
void Transform::from_lua(const gb::utils::luatable_mapper & mapper)
{
	vector<lua_Number> s = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFORM_KEY_SCALE);
	scale = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFORM_KEY_SCALE);

	rotation = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFROM_KEY_ROTATION);

	position = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFROM_KEY_POSITION);
}

//void Transform::operator=(const Transform & other)
//{
//	scale = other.scale;
//	rotation = other.rotation;
//	position = other.position;
//}
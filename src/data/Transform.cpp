#include "Transform.h"

using namespace gb::utils;
using namespace gb::render::data;
using namespace gb;
using namespace gb::physics;
Transform::Transform():
	scale(1.0f)
{

}
void Transform::from_lua(const gb::utils::luatable_mapper & mapper)
{
	std::vector<float> ret = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFORM_KEY_SCALE);
	if (ret.size() == 3)
		scale = ret;

	ret = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFROM_KEY_ROTATION);
	if (ret.size() == 3)
		rotation = ret;

	ret = mapper.get_numbers_by_key(GB_RENDER_DATA_TRANSFROM_KEY_POSITION);
	if (ret.size() == 3)
		position = ret;
}

//void Transform::operator=(const Transform & other)
//{
//	scale = other.scale;
//	rotation = other.rotation;
//	position = other.position;
//}
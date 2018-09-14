#include "Transform.h"
#include "Entity.h"
using namespace gb::render;
using namespace gb;
using namespace gb::physics;
Transform::Transform(Entity* owner):
	_localPosition_Ref(_LocalTransMat[3]),
	_owner(owner)
{
	_LocalTransMat = mat4f::make_identity();
}


void Transform::operator=(const render::data::Transform & dataTrans)
{
	_localPosition_Ref = dataTrans.position;

	_LocalScale = dataTrans.scale;
	_LocalRotation = dataTrans.rotation;

	_updateLocalMatrix();
}

void Transform::SetPosition(const gb::physics::vec3f & newPosition)
{
	_localPosition_Ref = newPosition;

	_updateLocalMatrix();
}

void Transform::SetRotation(const physics::vec3f& angle)
{
	_LocalRotation = angle;

	_updateLocalMatrix();
}

const gb::physics::vec4f & Transform::GetLocalPosition() const
{
	return _localPosition_Ref;
}

void Transform::_updateLocalMatrix()
{
	/*
	Transform Sequence Rule
	1st, scale
	2nd, rotate(extrinsic)
	3rd, translate(intrinsic)

	//Rotation Sequence Convention
	//1st, x-axis
	//2nd, y-axis
	//3rd, z-axis

	each transform is based on local coordinate system(parent transformed coordinate system).
	*/
	vec4f position = _localPosition_Ref;

	_LocalTransMat = mat4f::make_identity();

	_localPosition_Ref = position;

	_LocalTransMat *= scaleMat(_LocalScale);

	_LocalTransMat *= rotateXAxisMat<float>(_LocalRotation[0]);
	_LocalTransMat *= rotateYAxisMat<float>(_LocalRotation[1]);
	_LocalTransMat *= rotateZAxisMat<float>(_LocalRotation[2]);

	_owner->_updateWorldTransform();

}

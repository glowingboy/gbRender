#include "Transform.h"

using namespace gb::render;
using namespace gb;
using namespace gb::physics;
Transform::Transform():
	_localPosition_Ref(_LocalTransMat[3])
{}


void Transform::operator=(const render::data::Transform & dataTrans)
{
	_localPosition_Ref = dataTrans.position;

	_LocalScale = dataTrans.scale;
	_LocalRotation = dataTrans.rotation;

	_updateLocalMatrix();
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

	_LocalTransMat *= scaleMat(_LocalScale);

	_LocalTransMat *= rotateXAxisMat<Float>(_LocalRotation[0]);
	_LocalTransMat *= rotateXAxisMat<Float>(_LocalRotation[1]);
	_LocalTransMat *= rotateXAxisMat<Float>(_LocalRotation[2]);

}

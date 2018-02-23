#include "Transform.h"

using namespace gb::render;
using namespace gb;
Transform::Transform():
	_localPosition_Ref(_LocalTransMat[3])
{}


void Transform::operator=(const render::data::Transform & dataTrans)
{
	_localPosition_Ref = dataTrans.position;

	_LocalScale = dataTrans.scale;
	_LocalRotation = dataTrans.rotation;

	//update localTransMat
}

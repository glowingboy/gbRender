#pragma once
#include "../../gbUtils/Log/gbLog.h"
#include "../../gbUtils/Math/gbMath.h"
#include "../../gbUtils/Memory/gbBinaryData.h"
#include "../../gbUtils/Data/gbTextureData.h"

#define SampleScale 64
class gbSDF
{
	SingletonDeclare(gbSDF)
public:
	inline ~gbSDF() {};
	bool Gen(gbBinaryData& data, const unsigned int width, const unsigned int height, gbTextureData& sdf);
	bool Gen(unsigned char* data, const unsigned int width, const unsigned int height, gbTextureData& sdf);
};


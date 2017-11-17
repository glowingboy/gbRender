#include "gbSDF.h"
//SingletonDefine(gbSDF)

/**
	optimal dir:
		pmovmskb, multi-thread
*/

bool gbSDF::Gen(gbBinaryData & data, const unsigned int width, const unsigned int height, gbTextureData& sdf)
{
	if (width == 0 || height == 0)
		return false;
	std::vector<gbKdNode_2d::package*> discret_contour;
	gbMath::BinaryImageContour(&data, width, height, discret_contour);
	
	//kd tree
	gbKdNode_2d root(&discret_contour[0], 0, discret_contour.size() - 1);

	sdf.Width = width / SampleScale;
	sdf.Height = height / SampleScale;
	sdf.Data = new unsigned char[sdf.Width*sdf.Height];
	int maxDistance = sqrt(pow(width, 2) + pow(height, 2));//out of range int ?
	int indexI = 0;
	for (int i = 0; indexI < sdf.Height; i += SampleScale)
	{
		int indexJ = 0;
		for (int j = 0; indexJ < sdf.Width; j += SampleScale)
		{
			//1.compute distance field
			gbKdNode_2d::package* nn = root.NearestNeighbourSearch(glm::ivec2(j, i));
			float dis = sqrtf(gbMath::SquareDistance(glm::ivec2(j, i), glm::ivec2(nn->coord.x, nn->coord.y)));
			//2.compute sign with origin binaryImage(inside 255, outside 0)
			if (data.Out(i*width + j) != 0)
				dis = -dis;
			//clamp to 0-255 form (-maxDistance ~ maxDistance)
			sdf.Data[indexI*sdf.Width + indexJ] = gbMath::clamp(dis, -maxDistance, maxDistance, 0, 255);
			indexJ++;
		}
		std::string strLog;
		char buf[16] = { 0 };
		float per = (float)i / height;
		sprintf(buf, "%.2f", per);
		strLog += "_i:";
		strLog += buf;
		gbLog::Instance().Log(strLog.c_str());
		indexI++;
	}

	for (std::vector<gbKdNode_2d::package*>::iterator i = discret_contour.begin(); i != discret_contour.end(); i++)
	{
		gbSAFE_DELETE((*i));
	}
	return true;
}

bool gbSDF::Gen(unsigned char * data, const unsigned int width, const unsigned int height, gbTextureData & sdf)
{
	if (width == 0 || height == 0)
		return false;
	std::vector<gbKdNode_2d::package*> discret_contour;
	gbMath::BinaryImageContour(data, width, height, discret_contour);
	//kd tree
	gbKdNode_2d root(&discret_contour[0], 0, discret_contour.size() - 1);

	sdf.Width = width / SampleScale;
	sdf.Height = height / SampleScale;
	sdf.Data = new unsigned char[sdf.Width*sdf.Height];
	int maxDistance = sqrt(pow(width, 2) + pow(height, 2));//out of range int ?
	int indexI = 0;
	for (int i = 0; indexI < sdf.Height; i += SampleScale)
	{
		int indexJ = 0;
		for (int j = 0; indexJ < sdf.Width; j += SampleScale)
		{
			//1.compute distance field
			gbKdNode_2d::package* nn = root.NearestNeighbourSearch(glm::ivec2(j, i));
			float dis = sqrtf(gbMath::SquareDistance(glm::ivec2(j, i), glm::ivec2(nn->coord.x, nn->coord.y)));
			//2.compute sign with origin binaryImage(inside 255, outside 0)
			if (data[i*width + j] != 0)
				dis = -dis;
			//clamp to 0-255 form (-maxDistance ~ maxDistance)
			sdf.Data[indexI*sdf.Width + indexJ] = gbMath::clamp(dis, -maxDistance, maxDistance, 0, 255);
			indexJ++;
		}
		std::string strLog;
		char buf[16] = { 0 };
		float per = (float)i / height;
		sprintf(buf, "%.2f", per);
		strLog += "_i:";
		strLog += buf;
		gbLog::Instance().Log(strLog.c_str());
		indexI++;
	}
	return true;
}

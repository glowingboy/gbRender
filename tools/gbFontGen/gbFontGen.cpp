// gbFontGen.cpp : Defines the entry point for the console application.
//

#include "SignedDistanceFieldGen\gbSDF.h"
#include "FreeTypeLoader\gbFreeTypeLoader.h"

#include <fstream>
#ifdef _DEBUG
//#include <vld.h>
#endif // _DEBUG


int main(int argc, char* argv[])
{
	//p:packing, rep:resize and repacking
	std::cout << "what can do for you? p:packing or rep:resize and repacking ?" << std::endl;
	std::string strCmd;
	std::cin >> strCmd;
	if (!strCmd.compare("p"))
	{
		std::cout << "font file name:";
		char fontName[_MAX_PATH] = { '\0' };
		std::cin >> fontName;

		std::cout << std::endl << "gbFont file name:";
		char gbFontName[_MAX_PATH] = { '\0' };
		std::cin >> gbFontName;

		gbFreeTypeLoader::Instance().Load(/*"Data/Font/msyh.ttc"*/fontName, /*"../gbRender/Data/test"*/gbFontName);
	}
	else if (!strCmd.compare("rep"))
	{
		std::cout << "gbFont file name:"<<std::endl;
		char gbFontName[_MAX_PATH] = { '\0' };
		std::cin >> gbFontName;

		std::cout << "new fixed width:" << std::endl;
		char gbFixedWidth[64] = { '\0' };
		std::cin >> gbFixedWidth;
		unsigned int fixedWidth = atoi(gbFixedWidth);
		gbFontFile ff;
		if (!ff.Open(gbFontName))
			return -1;
		std::vector<gbGlyph*> glyphs;
		gbTextureData textureData;
		unsigned int glyphPixelSize = 0;
		if (!ff.Decode(glyphs, textureData, glyphPixelSize))
			return -1;
		std::vector<gbTextureUV*> uvs;
		for (std::vector<gbGlyph*>::const_iterator i = glyphs.begin(); i != glyphs.end(); i++)
		{
			gbTextureUV* uv = new gbTextureUV;
			uv->uv_b = (*i)->uv_b;
			uv->uv_t = (*i)->uv_t;
			uv->uv_l = (*i)->uv_l;
			uv->uv_r = (*i)->uv_r;
			uvs.push_back(uv);
		}
		std::vector<gbTextureData*> elements;
		gbTextureData::Unpacking(textureData, uvs, elements);
		for (std::vector<gbTextureUV*>::iterator i = uvs.begin(); i != uvs.end(); i++)
		{
			gbSAFE_DELETE((*i));
		}
		uvs.clear();
		gbTextureData bin;
		gbTextureData::Packing(elements, bin, uvs, fixedWidth);
		for (std::vector<gbTextureData*>::iterator i = elements.begin(); i != elements.end(); i++)
		{
			gbSAFE_DELETE((*i));
		}
		for (int i = 0; i < uvs.size(); i++)
		{
			gbGlyph* const glyph = glyphs[i];
			gbTextureUV* const uv = uvs[i];
			glyph->uv_b = uv->uv_b;
			glyph->uv_l = uv->uv_l;
			glyph->uv_r = uv->uv_r;
			glyph->uv_t = uv->uv_t;
		}
		std::string newFontName(gbFontName);
		int pos = newFontName.find('.');
		newFontName.insert(pos, "_new");
		gbFontFile::Create(glyphs, &bin, newFontName.c_str(), glyphPixelSize);
		for (std::vector<gbGlyph*>::iterator i = glyphs.begin(); i != glyphs.end(); i++)
		{
			gbSAFE_DELETE((*i));
		}
		for (std::vector<gbTextureUV*>::iterator i = uvs.begin(); i != uvs.end(); i++)
		{
			gbSAFE_DELETE((*i));
		}
	}
	else
	{
		std::cout << "wrong cmd!!!" << std::endl;
		std::cout << "what can do for you? p:packing or rep:resize and repacking ?";
	}
	

	

	std::cout << "complete !!!" << std::endl;
	::system("pause");
	
	//char testBuf[100] = { 0 };
	//gbBinaryData bd;
	//bd.Reserve(100);
	//for (int i = 0; i < 100; i++)
	//{
	//	testBuf[i] = rand() % 2;
	//	bd.In(testBuf[i], i);
	//}
	//gbLog::Instance().Log(testBuf, 100);
	//char bdBuf[100] = { 0 };
	//for (int i = 0; i < 100; i++)
	//{
	//	bdBuf[i] = bd.Out(i);
	//}
	//gbLog::Instance().Log(bdBuf, 100);
	//getchar();
    return 0;
}


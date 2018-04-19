#include <gbUtils/args.h>
#include <gbUtils/logger.h>
#include <gbUtils/filesystem.h>

#include "FreetypeLoader.h"

using gb::utils::logger;
using gb::utils::string;
#include <type_traits>
int main(int argc, char** argv)
{
	gb::utils::args arg;

	string srcFontFile, dstFontFile;
	try
	{
		arg.parse(argc, argv);

		srcFontFile = arg.unnamed_arg(1);
		dstFontFile = arg.unnamed_arg(2);
	}
	catch (const string& err)
	{
		logger::Instance().error("args::parse error: " + err);
		return 1;
	}

	srcFontFile = gb::utils::filesystem::Instance().get_absolute_path(srcFontFile);
	dstFontFile = gb::utils::filesystem::Instance().get_absolute_path(dstFontFile);

	logger::Instance().log("\nsrcFontfile: " + srcFontFile + "\n" + "dstFontFile: " + dstFontFile);

	try
	{
		freetypeLoader::Instance().load2gbFont(srcFontFile, dstFontFile);
	}
	catch (string& err)
	{
		logger::Instance().error("freetypeLoader::load2gbfont error: " + err);
		return 1;
	}



	return 0;
}


#include <gbUtils/args.h>
#include <gbUtils/logger.h>

using gb::utils::logger;
int main(int argc, char** argv)
{
    gb::utils::args arg;

    try
    {
	arg.parse(argc, argv);

	const char* srcFontFilePath = arg.unnamed_arg(1);
	const char* dstFontFilePath = arg.unnamed_arg(2);
	logger::Instance().log(srcFontFilePath);
	logger::Instance().log(dstFontFilePath);
    
    }
    catch(const gb::utils::string& err)
    {
	logger::Instance().error("args::parse error: " + err);

	gb::utils::string test = "hello";
	gb::utils::string r = "world" + test;
	logger::Instance().error(r);
    }

    return 0;
}


#include <Render.h>
using namespace gb::render;
using namespace gb::algorithm;
int main(int argc, char** argv)
{
	Director::Instance().Ready(vec2<uint32>(640, 480));
	Director::Instance().Action();
    return 0;
}

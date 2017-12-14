#include <Device.h>
#include <Director.h>
using namespace gb::render;
using namespace gb::algorithm;
int main(int argc, char** argv)
{
	vec2<uint32> screenSize(640, 480);
	Device::Instance().Initialize(&screenSize);
	Scene* curScene = Director::Instance().Ready();
	if (curScene == nullptr)
		return 1;
	Director::Instance().Action();
	::system("pause");
    return 0;
}

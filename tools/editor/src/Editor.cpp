#include "../../../src/Director.h"

using namespace gb::render;
using namespace gb::algorithm;
using namespace gb::utils;

template<typename T>
void func(T a) { a.b = 1; }

void func(int a){}

class A {public: int b; };

int main(int argc, char** argv)
{
	func(1);
	if(!Director::Instance().Ready(Director::Argument(filesystem::Instance().get_absolute_path("../tools/editor/resource/test_entity.lua"), vec2<uint32>(640, 480))))
		return 1;
	Director::Instance().Action();
	::system("pause");
    return 0;
}

#include "../../../src/Director.h"

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;


int main(int argc, char** argv)
{
	resource::Res<data::Entity>::Instance().SetPath(filesystem::Instance().get_absolute_path("../tools/editor/resource/"));
	luastate_mt& entity_ls = resource::Res<data::Entity>::Instance().GetLuaStates();
	entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/ElementType.lua"));
	entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/RenderQueue.lua"));

	resource::Res<data::Mesh>::Instance().SetPath(filesystem::Instance().get_absolute_path("../tools/editor/resource/"));


	if(!Director::Instance().Ready(Director::Argument("test_entity.lua", vec2<uint32>(640, 480))))
		return 1;
	Director::Instance().Action();
	::system("pause");
    return 0;
}

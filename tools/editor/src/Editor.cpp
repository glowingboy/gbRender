#include "../../../src/Director.h"

using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;


int main(int argc, char** argv)
{
	//resource::Res<data::Entity>::Instance().SetResRoot(filesystem::Instance().get_absolute_path("../tools/editor/resource/"));
	//luastate_mt& entity_ls = resource::Res<data::Entity>::Instance().GetLuaStates();
	//entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/ElementType.lua"));
	//entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/RenderQueue.lua"));

	if (!resource::Res<data::Shader>::Instance().Initialize(filesystem::Instance().get_absolute_path("../resource/Shader/Config.lua")))
		return -1;

	if (!resource::Res<data::Material>::Instance().Initialize(filesystem::Instance().get_absolute_path("../resource/Material/Config.lua")))
		return -1;

	if (!resource::Res<data::Mesh>::Instance().Initialize(filesystem::Instance().get_absolute_path("../resource/Mesh/Config.lua")))
		return -1;

	if (!resource::Res<data::Entity>::Instance().Initialize(filesystem::Instance().get_absolute_path("../resource/Entity/Config.lua")))
		return -1;

	if(!Director::Instance().Ready(Director::Argument("scene_0.lua", vec2<uint32>(640, 480))))
		return 1;
	Director::Instance().Action();
	::system("pause");
    return 0;
}

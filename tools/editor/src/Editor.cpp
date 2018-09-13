#include <Director.h>
using namespace gb::render;
using namespace gb::physics;
using namespace gb::utils;

using namespace gb;

#include "data/Element.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

int main(int argc, char** argv)
{
	//resource::Res<data::Entity>::Instance().SetResRoot(filesystem::Instance().get_absolute_path("../tools/editor/resource/"));
	//luastate_mt& entity_ls = resource::Res<data::Entity>::Instance().GetLuaStates();
	//entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/ElementType.lua"));
	//entity_ls.dofile(filesystem::Instance().get_absolute_path("../resource/RenderQueue.lua"));

	editor::data::ElementRecipe::Instance().Reg();

	if (Director::Instance().Ready(Director::Argument("scene_0.lua", vec2<std::int32_t>(640, 480),
	{
		{ GB_RENDER_RESOURCE_CFG_SHADER, filesystem::Instance().get_absolute_path("../resource/Shader/Config.lua") },
		{ GB_RENDER_RESOURCE_CFG_MATERIAL, filesystem::Instance().get_absolute_path("../resource/Material/Config.lua") },
		{ GB_RENDER_RESOURCE_CFG_MESH, filesystem::Instance().get_absolute_path("../resource/Mesh/Config.lua") },
		{ GB_RENDER_RESOURCE_CFG_ENTITY, filesystem::Instance().get_absolute_path("../resource/Entity/Config.lua") }//,
		//{ GB_RENDER_RESOURCE_CFG_FONT, filesystem::Instance().get_absolute_path("../resource/Font/Config.lua")}
	}
	)))
		Director::Instance().Action();
	else
		return -1;
	::system("pause");
    return 0;
}

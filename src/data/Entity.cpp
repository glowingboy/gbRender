#include "Entity.h"
#include <algorithm>
#include <gbUtils/logger.h>
#include "..\Entity.h"

#include "Render.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <gbUtils/filesystem.h>

using namespace gb::render::data;
using namespace gb::utils;

Entity::Entity(Entity && o) :
	_Name(std::move(o._Name)),
	_Transform(o._Transform),
	_Children(std::move(o._Children)),
	_Elements(std::move(o._Elements))
{

}
Entity::~Entity()
{
	std::for_each(_Children.begin(), _Children.end(), [](std::pair<const string, Entity*> e)
	{
		GB_SAFE_DELETE(e.second);
	});
}


void Entity::from_lua(const luatable_mapper& mapper)
{
	if (mapper.has_key("Entity")) //TODO
	{
		//entity
		//TODO : 1. render supporting multi-meshes, one render - multi-meshes or one owner - multi-renders 2. mesh mgr sln
		using namespace gb::utils;
		using namespace gb::render;
		const string fileName = mapper.get_string_by_key("Entity");
		string extension = fileName.file_extension();
		if (extension == "lua")
			return;// _Res_base::_load_res(data);
		else
		{
			//assimp
			Assimp::Importer imp;
			const aiScene * scene = imp.ReadFile(gb::utils::filesystem::Instance().get_absolute_path("../resource/Entity/" + fileName), aiProcess_Triangulate);
			const aiNode * root = scene->mRootNode;

			aiMesh ** const meshes = scene->mMeshes;

			//create entity
			typedef data::Entity*(*createEntity_t)(const aiNode* curNode, const aiMesh ** meshes);
			std::function < void(data::Entity* e, const aiNode * curNode) > createEntity;
			createEntity = [&createEntity, meshes](data::Entity* e, const aiNode * curNode)
			{
				GB_ASSERT(curNode != nullptr);

				//data::Entity* e = new data::Entity; //TODO release
				e->GetName() = curNode->mName.C_Str();
				aiVector3t<float> position, scale, rotation;
				curNode->mTransformation.Decompose(scale, rotation, position);
				data::Transform & transform = e->GetTransform();
				transform.position = { position.x, position.y, position.z };
				transform.scale = { scale.x, scale.y, scale.z };
				transform.rotation = { rotation.x, rotation.y, rotation.z };

				

				const unsigned int meshCount = curNode->mNumMeshes;
				if (meshCount > 0)
				{
					data::Render * r = new data::Render;
					e->GetElements().insert(std::make_pair(render::Element::Type::Render, r));
					r->GetMaterial() = "Tint.lua";

					//fetch mesh
					data::Mesh * mesh = new data::Mesh;//TODO, mesh* mgr
					int idxBase = 0;
					for (unsigned int j = 0; j < meshCount; j++)
					{
						const aiMesh * curMesh = meshes[j];

						mesh->GetPosVar()->append(curMesh->mVertices, curMesh->mNumVertices);

						const unsigned int faceCount = curMesh->mNumFaces;
						std::vector<unsigned int> indices;
						const aiFace * faces = curMesh->mFaces;
						for (unsigned int k = 0; k < faceCount; k++)
						{
							const aiFace & face = faces[k];
							const unsigned int idxCount = face.mNumIndices;
							const unsigned int * idx = face.mIndices;
							for (unsigned int l = 0; l < idxCount; l++)
							{
								indices.push_back(idx[l] + idxBase);
							}
						}

						mesh->GetIdxVar().append(indices.data(), (GLsizei)(indices.size()));
						mesh->UpdateSphereBB();
						idxBase += mesh->GetPosVar()->count();
					}

					r->tmpMesh = mesh;
				}
				
				const unsigned int childrenCount = curNode->mNumChildren;
				aiNode ** const children = curNode->mChildren;
				for (unsigned int i = 0; i < childrenCount; i++)
				{
					const aiNode * curChild = children[i];
					data::Entity * child = new data::Entity;
					createEntity(child, curChild);
					e->GetChildren().insert(std::make_pair(curChild->mName.C_Str(), child));
				}
			};
			createEntity(this, root);
		}
		return;
	}

	//name
	if (mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_NAME))
		_Name = mapper.get_string_by_key(GB_RENDER_DATA_ENTITY_KEY_NAME);
	else
	{
		logger::Instance().error(string("Entity::from_lua broken entity mapper@ ") + mapper.GetData());
		return;
	}

	//transform
	mapper.checkout_table_by_key(GB_RENDER_DATA_ENTITY_KEY_TRANSFORM, _Transform);

	//elements
	if (mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_ELEMENTS))
		mapper.for_each_in([this, &mapper](const int idx)
	{
		Element* ele = mapper.get_table_by_idx<ElementAdapter>(idx).GetElement();
		if (ele != nullptr)
			_Elements.insert(std::make_pair(ele->GetType(), ele));
	}, GB_RENDER_DATA_ENTITY_KEY_ELEMENTS);

	//children
	if (mapper.has_key(GB_RENDER_DATA_ENTITY_KEY_CHILDREN))
		mapper.for_each_in([this, &mapper](const int idx)
	{
		Entity* e = new Entity(mapper.get_table_by_idx<Entity>(idx));
		_Children.insert(std::pair<string, Entity*>(e->GetName(), e));
	}, GB_RENDER_DATA_ENTITY_KEY_CHILDREN);


}

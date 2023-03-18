#include "Precompiled.h"
#include "RenderObject.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;

//void RenderObject::Terminate()
//{
//	diffuseMap.Terminate();
//	specularMap.Terminate();
//	displacementMap.Terminate();
//	normalMap.Terminate();
//
//	meshBuffer.Terminate();
//}

RenderGroup sqEngine::Graphics::CreateRenderGroup(const Model& model,const Animator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](const auto& textureName) ->TextureId
	{
		if (textureName.empty())
		{
			return 0;
		}
		auto tm = TextureManager::Get();
		
		return tm->Get()->LoadTexture(textureName);
	};



	for (auto& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		auto& materialData = model.materialData[meshData.materialIndex];//meshData.materialIndex
		renderObject.material = materialData.material;
		renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
		renderObject.specularMapId = TryLoadTexture(materialData.specularMapName);
		renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
		renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);

		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
		//renderObject.animationclip = &model.animationSet[0];

		renderObject.meshBuffer.Initialize(meshData.mesh);
	}

	return renderGroup;
}

void sqEngine::Graphics::CleanUpRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
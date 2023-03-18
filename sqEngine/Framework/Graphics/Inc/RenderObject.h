#pragma once
#include"Material.h"
#include"MeshBuffer.h"
#include"TextureManager.h"
#include"Transform.h"
#include"Skeleton.h"
#include"Model.h"


namespace sqEngine::Graphics
{
	class Animator;

	class RenderObject
	{
	public:
		void Terminate()
		{
			diffuseMapId = 0;
			specularMapId = 0;
			bumpMapId = 0;
			normalMapId = 0;
			meshBuffer.Terminate();
		}
		Transform transform;

		Material material;

		TextureId diffuseMapId = 0;
		TextureId specularMapId = 0;
		TextureId bumpMapId = 0;
		TextureId normalMapId = 0;

		MeshBuffer meshBuffer;
		const Skeleton* skeleton = nullptr;

		//
		const Animator* animator = nullptr;
		const AnimationClip* animationClip = nullptr;
		float animationTime = 0.0f;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& mode,const Animator* animator = nullptr);
	void CleanUpRenderGroup(RenderGroup& renderGroup);

	/*template<class Effect>
	void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
	{
		for (auto& RenderObject::renderGroup)
		{
			effect.Render(renderObject);
		}
	}*/
}
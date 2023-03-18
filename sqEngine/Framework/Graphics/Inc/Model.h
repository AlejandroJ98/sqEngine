#pragma once
#include "AnimationClip.h"
#include "Material.h"
#include "MeshTypes.h"
#include "RenderObject.h"
#include "Skeleton.h"

namespace sqEngine::Graphics
{
	struct Model
	{
		struct MeshData
		{
			Mesh mesh;
			uint32_t materialIndex = 0;
		};

		struct MaterialDate
		{
			Material material;
			std::string diffuseMapName;
			std::string specularMapName;
			std::string bumpMapName;
			std::string normalMapName;
			std::string displacementMapName;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialDate> materialData;
		std::unique_ptr<Skeleton> skeleton;
		std::vector<AnimationClip> animationSet;
	};
}
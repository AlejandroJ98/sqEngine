#include "Precompiled.h"
#include"AnimationUtil.h"
#include"SimpleDraw.h"
using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Graphics::AnimationUtil;

namespace
{
	void ComputeBoneTransform(Bone* bone, BoneTransforms& boneTransforms, int depth)
	{
		
			Bone* b = bone;
			int count = depth;
			while (b != nullptr && count > 0)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * b->toParentTransform;
				b = b->parent;
				--count;
			}

			//test
			if (bone->index == 8)
				boneTransforms[bone->index] = Math::Matrix4::RotationX(1.0f) * boneTransforms[bone->index];

			for (auto child : bone->children)
			{
				ComputeBoneTransform(child, boneTransforms,  depth);
			}
	
	}
}

void AnimationUtil::ComputeBindPose(const Skeleton& skeleton, BoneTransforms& boneTransforms, int depth)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);

	ComputeBoneTransform(skeleton.root, boneTransforms,  depth);
}

void AnimationUtil::ApplyBoneOffset(const Skeleton& skeleton, BoneTransforms& boneTransforms)
{
	for (auto& bone : skeleton.bones)
	{
		boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
	}
}

void AnimationUtil::DrawSkeleton(const Skeleton& skeleton, const BoneTransforms& boneTransforms)
{
	const size_t numBones = skeleton.bones.size();
	for (size_t i = 0; i < numBones; ++i)
	{
		const auto posA = sqEngine::Math::GetTranslation(boneTransforms[i]);
		if (skeleton.bones[i]->parent != nullptr)
		{
			const auto posB = sqEngine::Math::GetTranslation(boneTransforms[skeleton.bones[i]->parentIndex]);
			SimpleDraw::AddLine(posA, posB, Colors::AliceBlue);
		}
	}
}


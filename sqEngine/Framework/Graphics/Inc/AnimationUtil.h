#pragma once
#include"Skeleton.h"

namespace sqEngine::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<Math::Matrix4>;
	void ComputeBindPose(const Skeleton& skeleton, BoneTransforms& boneTransforms, int depth);
	void ApplyBoneOffset(const Skeleton& skeleton, BoneTransforms& boneTransforms);
	void DrawSkeleton(const Skeleton& skeleton, const BoneTransforms& boneTransforms);
}

#pragma once
#include "Common.h"

namespace sqEngine::Graphics
{
	struct Transform
	{
		Math::Vector3 position = Math::Vector3::Zero;
		//Math::Vector3 rotation = Math::Vector3::Zero;
		Math::Quaternion rotationQ = Math::Quaternion::Identity;
		Math::Vector3 scale = Math::Vector3::One;

		Math::Matrix4 GetMatrix4() const
		{
			Math::Matrix4 matTrans = Math::Matrix4::Translation(position);
			Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(rotationQ);
			Math::Matrix4 matScale = Math::Matrix4::Scaling(scale);

			return matTrans * matRot * matScale;		
		}
	};
}
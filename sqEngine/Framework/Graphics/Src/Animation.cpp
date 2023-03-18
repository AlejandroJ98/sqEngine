#include"Precompiled.h"
#include"Animation.h"

#include "Transform.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;

namespace
{
	Vector3 Interpolate(const Vector3& a, const Vector3& b,float t)
	{
		return Lerp(a,b,t);
	}

	Quaternion Interpolate(const Quaternion& a, const Quaternion& b, float t)
	{
		return Slerp(a, b, t);
	}

	template<class T>
	T GetValue(const Keyframes<T>& keyframes, float time, T defaulValue)
	{
		if (keyframes.empty())
			return defaulValue;

		const Keyframe<T>& firstKey = keyframes.front();
		const Keyframe<T>& lastKey = keyframes.back();

		if (time <= firstKey.time)
		{
			return firstKey.key;
		}
		else if (time >= lastKey.time)
		{
			return lastKey.key;
		}

		//const size_t index = GetKeyFrameIndex(keyframes,time);

		size_t index = 0;
		for (int i = 0; i < keyframes.size(); ++i)
		{
			if (time <= keyframes[i].time)
			{
				index = i - 1;
				break;
			}
		}

		const Keyframe<T>& fromkey = keyframes[index];
		const Keyframe<T>& tokey = keyframes[index + 1];
		const float t = ((time - fromkey.time) / (tokey.time - fromkey.time));
		return Interpolate(fromkey.key, tokey.key, t);
	}
}



Math::Vector3 Animation::GetPosition(float time) const
{
	/*for (int i = 0; i < mPositionKeys.size(); ++i)
	{
		if(time < mPositionKeys[i].time)
			if (i > 0)
			{
				float lerpTime = (time - mPositionKeys[i - 1].time) / (mPositionKeys[i].time - mPositionKeys[i - 1].time);
				Math::Vector3 pos = Math::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, lerpTime);
				return pos;
			}
	}
	return Math::Vector3(0.0f);*/
	return GetValue<Math::Vector3>(mPositionKeys, time, Math::Vector3::Zero);
}

Math::Quaternion Animation::GetRotation(float time) const
{

	return GetValue<Math::Quaternion>(mRotationKeys, time, Math::Quaternion::Identity);
}

Math::Vector3 Animation::GetScale(float time) const
{

	return GetValue<Math::Vector3>(mScaleKeys, time, Math::Vector3::Zero);
}

Math::Matrix4 Animation::GetTransform(float time) const
{
	const auto& position = GetPosition(time);
	const auto& rotation = GetRotation(time);
	const auto& scale = GetScale(time);

	return Transform{position, rotation, scale}.GetMatrix4();
}

float Animation::GetDuration() const
{
	return Math::Max(mPositionKeys.back().time, Math::Max(mRotationKeys.back().time, mScaleKeys.back().time));
}
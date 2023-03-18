#include"Precompiled.h"
#include"sqMath.h"

using namespace sqEngine::Math;
const Vector3 Vector3::Zero(0.0f);
const Vector3 Vector3::One(1.0f);
const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);

//struct Vector2 { float x, y; };
//const Vector2 Vector2::Zero{0.0f};

const Matrix4 Matrix4::Identity
{
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f,
};

const Quaternion Quaternion::Identity{1.0f,0.0f,0.0f,0.0f};

struct Box
{
	Vector2 min, max;
	bool Intersect(Box box1,Box box2)
	{
		if (box1.max.x > box2.min.x &&
			box1.min.x < box2.max.x &&
			box1.max.y > box2.min.y &&
			box1.min.y < box2.max.y)
		{
			return true;
		}
		return false;
	}
};

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(c, a.x * s, a.y * s, a.z * s);
}

Quaternion Quaternion::RotationEuler(const Vector3& eulerAngles)
{
	const float cr = cos(eulerAngles.x * 0.5f);
	const float sr = sin(eulerAngles.x * 0.5f);
	const float cy = cos(eulerAngles.z * 0.5f);
	const float sy = sin(eulerAngles.z * 0.5f);
	const float cp = cos(eulerAngles.y * 0.5f);
	const float sp = sin(eulerAngles.y * 0.5f);

	return {
			cr * cp * cy + sr * sp * sy,
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy
	};
}

Quaternion Quaternion::RotationMatrix(const Matrix4& m)
{
	float trace = m._11 + m._22 + m._33;

	if (trace > 0.0f)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		return
		{
			0.25f / s,
			(m._23 - m._32) * s,
			(m._31 - m._13) * s,
			(m._12 - m._21) * s,
		};
	}

	else if (m._11 > m._22 && m._11 > m._33)
	{
		float s = 2.0f * sqrtf(1.0f + m._11 - m._22 - m._33);
		return
		{
			(m._23 - m._32) / s,
			0.25f * s,
			(m._21 + m._12) / s,
			(m._31 + m._13) / s
		};
	}

	else if (m._22 > m._33)
	{
		float s = 2.0f * sqrtf(1.0f + m._22 - m._11 - m._33);
		return
		{
			(m._31 - m._13) / s,
			(m._21 + m._12) / s,
			0.25f * s,
			(m._32 + m._23) / s
		};
	}

	else
	{
		float s = 2.0f * sqrtf(1.0f + m._33 - m._11 - m._22);
		return
		{
			(m._12 - m._21) / s,
			(m._31 + m._13) / s,
			(m._32 + m._23) / s,
			0.25f * s
		};
	}
}

Quaternion Quaternion::RotationLook(const Vector3& direction, const Vector3& up)
{
	Math::Vector3 l = Math::Normalize(direction);
	Math::Vector3 u = Math::Normalize(up);
	Math::Vector3 r = Math::Normalize(Math::Cross(u, l));
	u = Math::Normalize(Math::Cross(l, r));
	return RotationMatrix(
		{
			r.x, r.y, r.z, 0.f,
			u.x, u.y, u.z, 0.f,
			l.x, l.y, l.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		});
}

Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to)
{
	Math::Vector3 f = Math::Normalize(from);
	Math::Vector3 t = Math::Normalize(to);
	Math::Vector3 axis = Math::Normalize(Math::Cross(f, t));
	float angle = acosf(Dot(f, t));
	return RotationAxis(axis, angle);
}


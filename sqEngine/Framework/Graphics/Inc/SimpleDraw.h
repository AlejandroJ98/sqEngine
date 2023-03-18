#pragma once

#include "Colors.h"

namespace sqEngine::Graphics
{
	class Camera;

	namespace SimpleDraw
	{
		void StaticInitialize(uint32_t maxVertexCount);
		void StaticTerminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color);

		void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color);
		void AddAABB(const Math::AABB& aabb,Color color);
		void AddAABB(float minX, float maxX, float minY, float maxY, float maxZ, float minZ, Color color);

		void AddFilledAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color);
		void AddFilledAABB(const Math::AABB& aabb, Color color);
		void AddFilledAABB(float minX, float maxX, float minY, float maxY, float maxZ, float minZ, Color color);

		void AddSphere(const Math::Vector3& center, float radius, Color color);
		void AddSphere(const Math::Sphere& sphere, Color color);

		//void AddPlane(...);
		void AddTransform(const Math::Matrix4& transform);

		void Render(const Camera& camera);
	}
}
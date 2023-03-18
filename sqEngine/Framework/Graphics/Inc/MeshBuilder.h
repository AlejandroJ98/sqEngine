#pragma once
#include"MeshTypes.h"

namespace sqEngine::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPC CreateCube();
		static MeshPC CreatePlanePc(int row, int column, float spacing);
		static MeshPC Createcylinder(int segment, int sides, float radius);
		static MeshPC Createball(int segment, int sides, float radius);
		static MeshPC CreateSpherePC(float radius, int rings, int slices);

		static MeshPX CreateSkyBoxPX(float size);
		static MeshPX CreateSkyBox();
		static MeshPX CreateSpherePX(float radius, int rings, int slices);
		static MeshPX CreateSkyDome(float radius, int rings, int slices);
		static MeshPX CreateScreenQuad();

		static Mesh CreatePlane(int rows, int columns, float spacing);
		static Mesh CreateSphere(int slices, int rings, float radius);
	};
}
#include "Precompiled.h"
#include "SimpleDraw.h"
#include "BlendState.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color);
		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, Color color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		BlendState mAlphaBlendState;

		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFaceVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};


	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/SimpleDraw.fx");
		mPixelShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);
		mAlphaBlendState.Initialize(BlendState::Mode::AlphaBlend);

		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mFaceVertexCount = 0;
		mLineVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mAlphaBlendState.Terminate();
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, Color color)
	{
		if (mFaceVertexCount + 3 <= mMaxVertexCount)
		{
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mAlphaBlendState.Set();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		BlendState::ClearState();

		mLineVertexCount = 0;
		mFaceVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color)
{
	sInstance->AddLine(v0, v1, color);
}


void SimpleDraw::AddAABB(const Math::AABB& aabb, Color color)
{
	AddAABB(aabb.center, aabb.extend, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color)
{
	AddAABB(
		center.x - extend.x,
		center.x + extend.x,
		center.y - extend.y,
		center.y + extend.y,
		center.z - extend.z,
		center.z + extend.z,
		color);
}
void SimpleDraw::AddAABB(float minX, float maxX, float minY, float maxY, float maxZ, float minZ, Color color)
{
	const Math::Vector3 NNN = {minX,minY,minZ};
	const Math::Vector3 NNP = {minX,minY,maxZ};
	const Math::Vector3 NPN = {minX,maxY,minZ};
	const Math::Vector3 NPP = {minX,maxY,maxZ};
	const Math::Vector3 PNN = {maxX,minY,minZ};
	const Math::Vector3 PNP = {maxX,minY,maxZ};
	const Math::Vector3 PPN = {maxX,maxY,minZ};
	const Math::Vector3 PPP = {maxX,maxY,maxZ};

	sInstance->AddLine(NNN, NNP, color);
	sInstance->AddLine(NNP, PNP, color);
	sInstance->AddLine(PNP, PNN, color);
	sInstance->AddLine(PNN, NNN, color);

	sInstance->AddLine(NNN, NPN, color);
	sInstance->AddLine(NNP, NPP, color);
	sInstance->AddLine(PNP, PPP, color);
	sInstance->AddLine(PNN, PPN, color);

	sInstance->AddLine(NPN, NPP, color);
	sInstance->AddLine(NPP, PPP, color);
	sInstance->AddLine(PPP, PPN, color);
	sInstance->AddLine(PPN, NPN, color);
}

void SimpleDraw::AddFilledAABB(const Math::AABB& aabb, Color color)
{
	AddFilledAABB(aabb.center, aabb.extend, color);
}

void SimpleDraw::AddFilledAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color)
{
	AddFilledAABB(
		center.x - extend.x,
		center.x + extend.x,
		center.y - extend.y,
		center.y + extend.y,
		center.z - extend.z,
		center.z + extend.z,
		color);
}
void SimpleDraw::AddFilledAABB(float minX, float maxX, float minY, float maxY, float maxZ, float minZ, Color color)
{
	const Math::Vector3 NNN = { minX,minY,minZ };
	const Math::Vector3 NNP = { minX,minY,maxZ };
	const Math::Vector3 NPN = { minX,maxY,minZ };
	const Math::Vector3 NPP = { minX,maxY,maxZ };
	const Math::Vector3 PNN = { maxX,minY,minZ };
	const Math::Vector3 PNP = { maxX,minY,maxZ };
	const Math::Vector3 PPN = { maxX,maxY,minZ };
	const Math::Vector3 PPP = { maxX,maxY,maxZ };

	sInstance->AddFace(NNN,	NPN, PPN, color);
	sInstance->AddFace(NNN, PPN, PNN, color);

	sInstance->AddFace(PNN, PPN, PPP, color);
	sInstance->AddFace(PNN, PPP, PNP, color);

	sInstance->AddFace(PNP, PPP, NPP, color);
	sInstance->AddFace(PNP, NPP, NNP, color);

	sInstance->AddFace(NNP, NPP, NPN, color);
	sInstance->AddFace(NNP, NPN, NNN, color);

	sInstance->AddFace(NPN, NPP, PPP, color);
	sInstance->AddFace(NPN, PPP, PPN, color);

	sInstance->AddFace(NNP, NNN, PNN, color);
	sInstance->AddFace(NNP, PNN, PNP, color);
}


void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, Color color)
{
	// todo
}

void SimpleDraw::AddSphere(const Math::Sphere& sphere, Color color)
{
	AddSphere(sphere.center, sphere.radius, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}

void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	auto r = Math::GetRight(transform);
	auto u = Math::GetUp(transform);
	auto l = Math::GetForward(transform);
	auto p = Math::GetTranslation(transform);
	sInstance->AddLine(p,p+r,Colors::Red);
	sInstance->AddLine(p,p+u,Colors::Green);
	sInstance->AddLine(p,p+l,Colors::Blue);
}

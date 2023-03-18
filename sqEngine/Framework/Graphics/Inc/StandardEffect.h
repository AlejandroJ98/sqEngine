#pragma once
#include "Effect.h"
#include"ConstantBuffer.h"
#include"LightTypes.h"
#include"Material.h"
#include"PixelShader.h"
#include"VertexShader.h"
#include"Sampler.h"
namespace sqEngine::Graphics
{
	class Camera;
	//class RenderObject;
	class Texture;

	class StandardEffect : public Effect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetBumpWeight(float weight) { mBumpWeight = weight; }
		void SetShadowMap(const Texture* shadowMap);
		void SetDepthBias(float bias) { mDepthBias = bias; }

		void UseDiffuseMap(bool use) { mSettingsData.useDiffuseMap = use ? 1 : 0; }
		void UseSpecularMap(bool use) { mSettingsData.useSpecularMap = use ? 1 : 0; }
		void UseBumpMap(bool use) { mSettingsData.useBumpMap = use ? 1 : 0; }
		void UseNormalMap(bool use) { mSettingsData.useNormalMap = use ? 1 : 0; }
		void UseShadowMap(bool use){ mSettingsData.useNormalMap = use ? 1 : 0; }

		void DebugUI();

		
	private:
		struct TransformData
		{
			sqEngine::Math::Matrix4 world;
			sqEngine::Math::Matrix4 wvp[2];
			sqEngine::Math::Vector3 viewPosition;
			float bumpWeight = 0.0f;
		};

		struct BoneTransformData
		{
			static constexpr size_t MaxBonCount = 128;
			Math::Matrix4 boneTransforms[128];
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useSpecularMap = 1;
			int	useBumpMap = 1;
			int	useNormalMap = 1;

			int useShadowMap = 1;
			int useSkinning = 0;
			float depthBias = 0.0f;
			float padding[1] = {};
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = TypedConstantBuffer<BoneTransformData>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		Sampler mSampler;

		SettingsData mSettingsData;
		float mBumpWeight = 0.0f;
		float mDepthBias = 0.0f;
	};
}
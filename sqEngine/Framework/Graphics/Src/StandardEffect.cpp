#include"Precompiled.h"
#include"StandardEffect.h"
#include"Camera.h"
#include"VertexTypes.h"
#include"RenderObject.h"
#include"AnimationUtil.h"
using namespace sqEngine;
using namespace sqEngine::Graphics;

void StandardEffect::Initialize()
{
	mVertexShader.Initialize<Vertex>(L"../../Assets/Shaders/Standard.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mSampler.Terminate();

	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect - no camera set!");
	ASSERT(mDirectionalLight != nullptr,"StandardEffect - no light set!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mBoneTransformBuffer.BindVS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindPS(3);

	mSettingsBuffer.BindVS(4);
	mSettingsBuffer.BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	if (mShadowMap != nullptr)
		Texture::UnbindPS(4);
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();


	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp[0]  = Transpose(matWorld * matView * matProj);

	if (mSettingsData.useShadowMap && mShadowMap != nullptr)
	{
		const auto& matLightView = mLightCamera->GetViewMatrix();
		const auto& matLightProj = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Transpose(matWorld * matLightView * matLightProj);
	}

	transformData.viewPosition = mCamera->GetPosition();
	transformData.bumpWeight = mBumpWeight;


	

	SettingsData settingsData;

	if (mSettingsData.useDiffuseMap == 0 || renderObject.diffuseMapId == 0)
		settingsData.useDiffuseMap = 0;
	if (mSettingsData.useSpecularMap == 0 || renderObject.specularMapId == 0)
		settingsData.useSpecularMap = 0;
	if (mSettingsData.useBumpMap == 0 || renderObject.bumpMapId == 0)
		settingsData.useBumpMap = 0;
	if (mSettingsData.useNormalMap == 0 || renderObject.normalMapId == 0)
		settingsData.useNormalMap = 0;
	if (mSettingsData.useShadowMap == 1 && mShadowMap != nullptr)
	{
		mShadowMap->BindPS(4);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;
	}
	else
	{
		settingsData.useShadowMap = 0;
	}

	if (renderObject.animationClip)
	{
		BoneTransformData boneTransformData;
		std::vector<Math::Matrix4> boneTransforms;
		
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBonCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);

		}
		mBoneTransformBuffer.Update(boneTransformData);
		settingsData.useSkinning = 1;

	}
	else if (renderObject.skeleton)
	{
		BoneTransformData boneTransformData;

		std::vector<Math::Matrix4> boneTransforms;

		AnimationUtil::ComputeBindPose(*renderObject.skeleton, boneTransforms, 37);
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBonCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);
		}

		mBoneTransformBuffer.Update(boneTransformData);
		mSettingsData.useSkinning = 1;
	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(mSettingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specularMapId, 1);
	tm->BindVS(renderObject.bumpMapId, 2);
	tm->BindPS(renderObject.normalMapId, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}


void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Bump Weight", &mBumpWeight,0.01f,0.0f,1.0f);
		ImGui::DragFloat("Bump Weight", &mBumpWeight, 0.01f, 0.0f, 1.0f);
		bool useDiffuseMap = (mSettingsData.useDiffuseMap == 1);
		ImGui::Checkbox("Use Diffuse Map", &useDiffuseMap);
		mSettingsData.useDiffuseMap = useDiffuseMap ? 1 : 0;

		bool useSpecularMap = (mSettingsData.useSpecularMap == 1);
		ImGui::Checkbox("use Specular Map", &useSpecularMap);
		mSettingsData.useSpecularMap = useSpecularMap ? 1 : 0;

		bool useBumpMap = (mSettingsData.useBumpMap == 1);
		ImGui::Checkbox("use Bump Map", &useBumpMap);
		mSettingsData.useBumpMap = useBumpMap ? 1 : 0;

		bool useNormalMap = (mSettingsData.useNormalMap == 1);
		ImGui::Checkbox("use Normal Map", &useNormalMap);
		mSettingsData.useNormalMap = useNormalMap ? 1 : 0;

		bool useShadowMap = (mSettingsData.useShadowMap == 1);
		ImGui::Checkbox("use Shadow Map", &useShadowMap);
		mSettingsData.useShadowMap = useShadowMap ? 1 : 0;

		ImGui::DragFloat("Depth bias", &mDepthBias, 0.01f, 0.0f, 1.0f,"%.5f");
	}
}
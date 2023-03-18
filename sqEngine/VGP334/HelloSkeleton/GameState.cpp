#include "GameState.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine::Input;
using namespace sqEngine::Graphics::AnimationUtil;

void GameState::Initialize()
{
	const float turnTime = 1.99f;
	
	mCamera.SetPosition({0.0f,8.0f,-10.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f});
	
	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.7f,0.7f, 0.7f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f,0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f,0.7f, 0.7f, 1.0f };

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	auto tm = TextureManager::Get();

	
	ModelIO::LoadModel("../../Assets/Models/Ortiz/ortiz.model", mCharacterModel);
	ModelIO::LoadMaterial("../../Assets/Models/Ortiz/ortiz.model", mCharacterModel);
	ModelIO::LoadSkeleton("../../Assets/Models/Ortiz/ortiz.model", mCharacterModel);
	ModelIO::LoadAnimation("../../Assets/Models/Ortiz/ortiz.model", mCharacterModel);

	//mBallRenderObject.material = mCharacterModel.materialData[0].material;

	//mBallRenderObject.material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };
	mCharacterAnimator.Initialize(&mCharacterModel);
	mCharacterAnimator.PlayAnimation(0);
	mGroup = CreateRenderGroup(mCharacterModel,&mCharacterAnimator);
	mGroup[0].material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };






	//mBallRenderObject.material.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	//mBallRenderObject.material.specular = { 0.5f,0.5f, 0.5f, 1.0f };
	//mBallRenderObject.material.power = 10.0f;

	//mBallRenderObject.diffuseMapId = tm->LoadTexture(mCharacterModel.materialData[0].diffuseMapName);
	//mBallRenderObject.normalMapId = tm->LoadTexture(mCharacterModel.materialData[0].normalMapName);
	//mBallRenderObject.specularMapId = tm->LoadTexture(mCharacterModel.materialData[0].specularMapName);
	//

	//mBallRenderObject.meshBuffer.Initialize(mCharacterModel.meshData[0].mesh);
	//mBallRenderObject.skeleton = mCharacterModel.skeleton.get();


	mGroundRenderObject.material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };
	mGroundRenderObject.material.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	mGroundRenderObject.material.specular = { 0.5f,0.5f, 0.5f, 1.0f };
	mGroundRenderObject.material.power = 10.0f;
	mGroundRenderObject.diffuseMapId = tm->LoadTexture("concrete.jpeg");
	mGroundRenderObject.meshBuffer.Initialize(MeshBuilder::CreatePlane(25, 25, 1.0f));
}

void GameState::Terminate()
{
	mGroundRenderObject.Terminate();
	//mBallRenderObject.Terminate(); 
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
	CleanUpRenderGroup(mGroup);
}

void GameState::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
	auto inputSystem = InputSystem::Get();
	const float moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	mFPS = 1.0f / deltaTime;
	mCharacterAnimator.Update(deltaTime);
}

void GameState::Render()
{
	mShadowEffect.Begin();
	mShadowEffect.RenderGroup(mGroup);
	//mShadowEffect.Render(mBallRenderObject);
	mShadowEffect.End();

	mStandardEffect.Begin();

	if (mDrawSkeleton)
	{
		//BoneTransforms boneTransforms;
		//auto& skeleton = mCharacterModel.skeleton;
		std::vector<Matrix4> boneTransforms;
		AnimationUtil::ComputeBindPose(*mCharacterModel.skeleton, boneTransforms, mDepth);
		AnimationUtil::DrawSkeleton(*mCharacterModel.skeleton, boneTransforms);
	}
	else
	{

		//mStandardEffect.Render(mBallRenderObject);
		mStandardEffect.RenderGroup(mGroup);
	}
	

	//mStandardEffect.Render(mBallRenderObject);
	//mStandardEffect.Render(mGroundRenderObject);
	mStandardEffect.End();

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	/*if (ImGui::CollapsingHeader("Ball", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Earth", &mBallRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Earth", &mBallRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Earth", &mBallRenderObject.material.specular.r);2
		ImGui::DragFloat3("Power##Earth", &mBallRenderObject.material.power, 1.0f, 1.0f, 100.0f);
	}*/

	if (ImGui::CollapsingHeader("group", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Earth", &mGroup[0].material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Earth", &mGroup[0].material.diffuse.r);
		ImGui::ColorEdit4("Specular##Earth", &mGroup[0].material.specular.r);
		ImGui::DragFloat3("Power##Earth", &mGroup[0].material.power, 1.0f, 1.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("positionZ", &mBallRenderObject.transform.position.x);
		ImGui::DragFloat("positionY", &mBallRenderObject.transform.position.y);
		ImGui::DragFloat("positionZ", &mBallRenderObject.transform.position.z);
		//ImGui::DragFloat("rx", &mBallRenderObject.transform.rotation.x);
		//ImGui::DragFloat("ry", &mBallRenderObject.transform.rotation.y);
		//ImGui::DragFloat("rz", &mBallRenderObject.transform.rotation.z, 0.01f, -10.0f, 10.0f,"%.5f");
		ImGui::DragFloat("rotationQ.w", &mBallRenderObject.transform.rotationQ.w, 0.01f, -10.0f, 10.0f, "%.5f");
		ImGui::DragFloat("rotationQ.x", &mBallRenderObject.transform.rotationQ.x, 0.01f, -10.0f, 10.0f, "%.5f");
		ImGui::DragFloat("rotationQ.y", &mBallRenderObject.transform.rotationQ.y, 0.01f, -10.0f, 10.0f, "%.5f");
		ImGui::DragFloat("rotationQ.z", &mBallRenderObject.transform.rotationQ.z, 0.01f, -10.0f, 10.0f, "%.5f");
	}

	if (ImGui::Checkbox("show skeleton", &mDrawSkeleton))
	{

	}

	ImGui::Separator();
	ImGui::Text("Depth: %d", mDepth);
	if (ImGui::Button("Add"))
		mDepth++;
	if (ImGui::Button("Subtract") && mDepth > 0)
		mDepth--;

	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();

	ImGui::End();
}



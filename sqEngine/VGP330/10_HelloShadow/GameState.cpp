#include "GameState.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine::Input;

void GameState::Initialize()
{
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

	Model duckModel;
	ModelIO::LoadModel("../../Assets/Models/Duck/duck.model", duckModel);


	mBallRenderObject.material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };
	mBallRenderObject.material.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	mBallRenderObject.material.specular = { 0.5f,0.5f, 0.5f, 1.0f };
	mBallRenderObject.material.power = 10.0f;
	//mBallRenderObject.diffuseMapId = tm->LoadTexture("basketball.jpg");
	//mBallRenderObject.meshBuffer.Initialize(MeshBuilder::CreateSphere(512,512,1.0f));
	mBallRenderObject.diffuseMapId = tm->LoadTexture("../Models/Duck/duck.png");
	mBallRenderObject.meshBuffer.Initialize(duckModel.meshData[0].mesh);

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
	mBallRenderObject.Terminate(); 
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
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

	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
		mPause = !mPause;

	mFPS = 1.0f / deltaTime;

	const float dt = mPause ? 0.0f : deltaTime;
	mRunningTime += dt;

	float height = mBallheight * abs(cos(mRunningTime)) + 1.0f;
	mBallRenderObject.transform.position.y = height;
	//
}

void GameState::Render()
{
	mShadowEffect.Begin();
	mShadowEffect.Render(mBallRenderObject);
	mShadowEffect.End();

	mStandardEffect.Begin();
	mStandardEffect.Render(mBallRenderObject);
	mStandardEffect.Render(mGroundRenderObject);
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

	if (ImGui::CollapsingHeader("Ball", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Earth", &mBallRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Earth", &mBallRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Earth", &mBallRenderObject.material.specular.r);
		ImGui::DragFloat3("Power##Earth", &mBallRenderObject.material.power, 1.0f, 1.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("x", &mBallRenderObject.transform.position.x);
		ImGui::DragFloat("y", &mBallRenderObject.transform.position.y);
		ImGui::DragFloat("z", &mBallRenderObject.transform.position.z);
		//ImGui::DragFloat("rx", &mBallRenderObject.transform.rotation.x);
		//ImGui::DragFloat("ry", &mBallRenderObject.transform.rotation.y);
		//ImGui::DragFloat("rz", &mBallRenderObject.transform.rotation.z, 0.01f, -10.0f, 10.0f,"%.5f");
	}

	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();

	ImGui::End();
}



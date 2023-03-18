#include "GameState.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({0.0f,1.0f,-3.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f});
	
	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.0f,0.0f, 0.0f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f,0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f,0.7f, 0.7f, 1.0f };

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetBumpWeight(0.2f);

	mMesh = MeshBuilder::CreateSphere(512,512,1.0f);

	auto tm = TextureManager::Get();
	mEarthRenderObject.material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };
	mEarthRenderObject.material.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	mEarthRenderObject.material.specular = { 0.5f,0.5f, 0.5f, 1.0f };
	mEarthRenderObject.material.power = 10.0f;
	mEarthRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mEarthRenderObject.specularMapId = tm->LoadTexture("earth_spec.jpg");
	mEarthRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	mEarthRenderObject.normalMapId = tm->LoadTexture("earth_normal.jpg");

	mEarthRenderObject.meshBuffer.Initialize(mMesh);

}

void GameState::Terminate()
{
	mEarthRenderObject.Terminate();
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

	mFPS = 1.0f / deltaTime;
}

void GameState::Render()
{
	mStandardEffect.Begin();

	mStandardEffect.Render(mEarthRenderObject);

	mStandardEffect.End();

	for (int i = 0; i <= 20; ++i)
	{
		SimpleDraw::AddLine({i-10.0f, -0.001f,-10.0f}, {i-10.0f, -0.001, 10.0f},Colors::White);
		SimpleDraw::AddLine({-10.0f,-0.001,i-10.0f}, {10.0f,-0.001,i-10.0f}, Colors::White);
	}
	SimpleDraw::AddLine(Vector3::Zero,Vector3::XAxis, Colors::Red);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);

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

	if (ImGui::CollapsingHeader("Earth", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mEarthRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mEarthRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mEarthRenderObject.material.specular.r);
		ImGui::DragFloat3("Power##Material", &mEarthRenderObject.material.power, 1.0f, 1.0f, 100.0f);
		ImGui::DragFloat3("Position", &mEarthRenderObject.transform.position.x, 0.01f);
		//ImGui::DragFloat3("Rotation", &mEarthRenderObject.transform.rotation.x, 0.01f);
	}

	mStandardEffect.DebugUI();

	ImGui::Checkbox("Show Normal", &mShowNormals);
	ImGui::End();
}



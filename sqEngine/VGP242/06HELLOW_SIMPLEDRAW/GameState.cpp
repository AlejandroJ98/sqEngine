#include "GameState.h"

using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine::Input;
using namespace sqEngine;

void GameState::Initialize()
{
	mCamera.SetPosition({0.0f,3.0f,-3.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f});
	
}

void GameState::Terminate()
{

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
}

void GameState::Render()
{
	for (int i = 0; i <= 20; ++i)
	{
		SimpleDraw::AddLine({i-10.0f, -0.001f,-10.0f}, {i-10.0f, -0.001, 10.0f},Colors::White);
		SimpleDraw::AddLine({-10.0f,-0.001,i-10.0f}, {10.0f,-0.001,i-10.0f}, Colors::White);
	}
	SimpleDraw::AddLine(Vector3::Zero,Vector3::XAxis, Colors::Red);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);
	SimpleDraw::AddFilledAABB(mPosition, { 0.5f,0.2f,0.3f }, mColor);
	SimpleDraw::AddAABB(mPosition, {0.5f,0.2f,0.3f},Colors::Magenta);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragFloat3("Position", &mPosition.x, 0.01f);
	ImGui::ColorEdit4("Color", &mColor.r);
	ImGui::End();
}



#include "GameState.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({0.0f,1.0f,-15.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f});
	
	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.0f,0.0f, 0.0f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f,0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f,0.7f, 0.7f, 1.0f };

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetBumpWeight(0.2f);

	mGaussianBlurEffect.Initialize();
	mGaussianBlurEffect.SetSourceTexture(mBlurRenderTarget);
	mGaussianBlurEffect.BlurIterations() = 10;
	mGaussianBlurEffect.BlurSaturation() = 1.0f;

	//mPostProcessingEffect.Initialize("NightVisionPs");
	//mPostProcessingEffect.Initialize("InvertPS");
	mPostProcessingEffect.Initialize();
	mPostProcessingEffect.SetMode(PostProcessingEffect::Mode::None);
	mPostProcessingEffect.SetTexture(&mBaseRenderTarget,0);
	mPostProcessingEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(),1);

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	mBaseRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget:: Format::RGBA_U8);
	mBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	auto tm = TextureManager::Get();

	mSunRenderObject.material.emissive = Colors::White;
	mSunRenderObject.material.power = 1.0f;
	mSunRenderObject.diffuseMapId = tm->LoadTexture("sun.jpg");
	mSunRenderObject.meshBuffer.Initialize(MeshBuilder::CreateSphere(512,512,5.0f));

	mEarthRenderObject.material.ambient = { 0.5f,0.5f, 0.5f, 1.0f };
	mEarthRenderObject.material.diffuse = { 0.8f,0.8f, 0.8f, 1.0f };
	mEarthRenderObject.material.specular = { 0.5f,0.5f, 0.5f, 1.0f };
	mEarthRenderObject.material.power = 10.0f;
	mEarthRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mEarthRenderObject.specularMapId = tm->LoadTexture("earth_spec.jpg");
	mEarthRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	mEarthRenderObject.normalMapId = tm->LoadTexture("earth_normal.jpg");
	mEarthRenderObject.meshBuffer.Initialize(MeshBuilder::CreateSphere(512,512,1.0f));

	mScreenQuad.meshBuffer.Initialize(MeshBuilder::CreateScreenQuad());
}

void GameState::Terminate()
{
	mScreenQuad.Terminate();
	mEarthRenderObject.Terminate();
	mSunRenderObject.Terminate();

	mBlurRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();

	mPostProcessingEffect.Terminate();
	mGaussianBlurEffect.Terminate();
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

	Vector3 earthPosition = { 0.0f,0.0f,-mEarthOffset };
	mEarthRenderObject.transform.position = TransformCoord(earthPosition,Matrix4::RotationY(mEarthRevolution));
		//mEarthRenderObject.transform.rotation.y = mEarthRotation;
}

void GameState::Render()
{

	mBaseRenderTarget.BeginRender();
	{
		mStandardEffect.Begin();
		mStandardEffect.Render(mSunRenderObject);
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
	mBaseRenderTarget.EndRender();

	mBlurRenderTarget.BeginRender();
	{
		 Material dummyMaterial;
		dummyMaterial.power = 1.0f;
		std::swap(mEarthRenderObject.material, dummyMaterial);

		mStandardEffect.Begin();
		mStandardEffect.Render(mSunRenderObject);
		mStandardEffect.Render(mEarthRenderObject);
		mStandardEffect.End();

		std::swap(mEarthRenderObject.material, dummyMaterial);
	}

	mBlurRenderTarget.EndRender();

	mGaussianBlurEffect.Begin();
	mGaussianBlurEffect.Render(mScreenQuad);
	mGaussianBlurEffect.End();

	mPostProcessingEffect.SetTexture(&mBaseRenderTarget,0);
	mPostProcessingEffect.SetTexture(&mBlurRenderTarget,1);
	mPostProcessingEffect.Begin();
	mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();
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

		ImGui::DragFloat3("Offset##Earth", &mEarthOffset,0.01f);
		ImGui::DragFloat3("Rotation##Earth", &mEarthRotation, 0.01f);
		ImGui::DragFloat3("Revolution##Earth", &mEarthRevolution, 0.01f);
	}

	mStandardEffect.DebugUI();
	mPostProcessingEffect.DebugUI();

	if (ImGui::CollapsingHeader("Blur Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("Blur Iterations", &mGaussianBlurEffect.BlurIterations(),1,1,100);
		ImGui::DragFloat("Blur Saturation", &mGaussianBlurEffect.BlurSaturation(),0.001f,1.0f,100.0f);
	}

	ImGui::End();

	ImGui::Begin("render Targets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Base");
	ImGui::Image(mBaseRenderTarget.GetRawData(), {256,144});

	ImGui::Text("Bloom");
	ImGui::Image(mBaseRenderTarget.GetRawData(), { 256,144 });

	ImGui::Text("Horizontal Blur");
	ImGui::Image(mGaussianBlurEffect.GetHorizontalBlurTexture().GetRawData(), { 256,144 });
	ImGui::Text("Vertical Blur");
	ImGui::Image(mGaussianBlurEffect.GetVerticalBlurTexture().GetRawData(), { 256,144 });
	ImGui::End();
}



#include "GameState.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Input;
using namespace sqEngine::Math;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	// NDC - Normalized Device Coordinate
	//
	//          +-----------------+
	//         /                 /|
	//   1.0  +-----------------+ |
	//	      |        ^        | |
	//	      |        |        | |
	//	      | <------+------> | |
	//	      |        |        | | 1.0
	//	      |        v        |/
	//	-1.0  +-----------------+ 0.0
	//      -1.0               1.0
	//
	// (0,0)
	//+---------------+
	//|				  |
	//|			      |
	//|			      |
	//|			      |
	//|               |
	//+---------------+(1,1)
	//
	//			   /+ 2 
	// 2	  3   / |
	// +-----+ 	 /	|
	// |	/	/   |
	// |   /   / 	|
	// |  /	  /		|
	// | /	 /		|
	// |/	/+-----+|		(draw face)
	// 1	1		3

	//			   /+ 3 
	// 3	  2   / |		(drwa back)
	// +-----+ 	 /	|
	// |	/	/   |
	// |   /   / 	|
	// |  /	  /		|
	// | /	 /		|
	// |/	/+-----+|		
	// 1	1		2
	Color color;

	/////////////////////////////////////////
	sun.distanceFromCenter = 0.0f;
	sun.ratationSpeed = 0.1f;
	sun.revolutionSpeed = 0.0f;
	sun.scale = 1.0f;
	sun.mMesh = MeshBuilder::CreateSpherePX(sun.scale, 50, 50);
	sun.mMeshBuffer.Initialize(sun.mMesh);
	sun.mTexture.Initialize("../../Assets/Images/sun.jpg");

	earth.distanceFromCenter = 3.0f;
	earth.ratationSpeed = 0.1f;
	earth.revolutionSpeed = 0.3f;
	earth.scale = 0.1f;
	earth.mMesh = MeshBuilder::CreateSpherePX(earth.scale, 50, 50);
	earth.mMeshBuffer.Initialize(earth.mMesh);
	earth.mTexture.Initialize("../../Assets/Images/earth.jpg");

	mercury.distanceFromCenter = 1.8f;
	mercury.ratationSpeed = 0.1f;
	mercury.revolutionSpeed = 2.0f;
	mercury.scale = 0.05f;
	mercury.mMesh = MeshBuilder::CreateSpherePX(mercury.scale, 50, 50);
	mercury.mMeshBuffer.Initialize(mercury.mMesh);
	mercury.mTexture.Initialize("../../Assets/Images/Mercury.jpg");

	venus.distanceFromCenter = 2.5f;
	venus.ratationSpeed = 0.1f;
	venus.revolutionSpeed = 0.5f;
	venus.scale = 0.1f;
	venus.mMesh = MeshBuilder::CreateSpherePX(venus.scale, 50, 50);
	venus.mMeshBuffer.Initialize(venus.mMesh);
	venus.mTexture.Initialize("../../Assets/Images/venus.jpg");

	mars.distanceFromCenter = 3.5f;
	mars.ratationSpeed = 0.1f;
	mars.revolutionSpeed = 0.5f;
	mars.scale = 0.1f;
	mars.mMesh = MeshBuilder::CreateSpherePX(mars.scale, 50, 50);
	mars.mMeshBuffer.Initialize(mars.mMesh);
	mars.mTexture.Initialize("../../Assets/Images/mars.jpg");

	jupiter.distanceFromCenter = 4.3f;
	jupiter.ratationSpeed = 0.1f;
	jupiter.revolutionSpeed = 0.6f;
	jupiter.scale = 0.9f;
	jupiter.mMesh = MeshBuilder::CreateSpherePX(jupiter.scale, 50, 50);
	jupiter.mMeshBuffer.Initialize(jupiter.mMesh);
	jupiter.mTexture.Initialize("../../Assets/Images/jupiter.jpg");

	saturn.distanceFromCenter = 5.1f;
	saturn.ratationSpeed = 0.1f;
	saturn.revolutionSpeed = 0.7f;
	saturn.scale = 0.45f;
	saturn.mMesh = MeshBuilder::CreateSpherePX(saturn.scale, 50, 50);
	saturn.mMeshBuffer.Initialize(saturn.mMesh);
	saturn.mTexture.Initialize("../../Assets/Images/saturn.jpg");

	neptune.distanceFromCenter = 6.3f;
	neptune.ratationSpeed = 0.1f;
	neptune.revolutionSpeed = 1.0f;
	neptune.scale = 0.09f;
	neptune.mMesh = MeshBuilder::CreateSpherePX(neptune.scale, 50, 50);
	neptune.mMeshBuffer.Initialize(neptune.mMesh);
	neptune.mTexture.Initialize("../../Assets/Images/neptune.jpg");

	uranus.distanceFromCenter = 7.5f;
	uranus.ratationSpeed = 0.1f;
	uranus.revolutionSpeed = 0.8f;
	uranus.scale = 0.07f;
	uranus.mMesh = MeshBuilder::CreateSpherePX(uranus.scale, 50, 50);
	uranus.mMeshBuffer.Initialize(uranus.mMesh);
	uranus.mTexture.Initialize("../../Assets/Images/uranus.jpg");


	mCubeMesh = MeshBuilder::CreateSkyBoxPX(500);
	mCubeMeshBuffer.Initialize(mCubeMesh);

	mVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/DoTexturing.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
	mConstantBuffer.Initialize(sizeof(Matrix4));

	mMengTexture.Initialize("../../Assets/Images/space.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSampler.Terminate();
	sun.mTexture.Terminate();
	sun.mMeshBuffer.Terminate();
	earth.mTexture.Terminate();
	earth.mMeshBuffer.Terminate();
	mercury.mTexture.Terminate();
	mercury.mMeshBuffer.Terminate();
	venus.mTexture.Terminate();
	venus.mMeshBuffer.Terminate();
	mars.mTexture.Terminate();
	mars.mMeshBuffer.Terminate();
	jupiter.mTexture.Terminate();
	jupiter.mMeshBuffer.Terminate();
	saturn.mTexture.Terminate();
	saturn.mMeshBuffer.Terminate();
	neptune.mTexture.Terminate();
	neptune.mMeshBuffer.Terminate();
	uranus.mTexture.Terminate();
	uranus.mMeshBuffer.Terminate();
	mMengTexture.Terminate();
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

	earth.Update(deltaTime);
	mercury.Update(deltaTime);
	venus.Update(deltaTime);
	mars.Update(deltaTime);
	jupiter.Update(deltaTime);
	saturn.Update(deltaTime);
	neptune.Update(deltaTime);
	uranus.Update(deltaTime);
}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	Matrix4 view = mCamera.GetViewMatrix();
	Matrix4 proj = mCamera.GetProjectionMatrix();

	static float rotationSkyBox{};
	static float rotationSun{};
	static float rotationearth{};
	static float rotationmercury{};
	static float rotationvenus{};
	static float rmars{};
	static float rjupiter{};
	static float rsaturn{};
	static float rneptune{};
	static float ruranus{};
	

	rotationSkyBox += 0.0001f;
	rotationSun += sun.ratationSpeed;
	rotationearth += earth.ratationSpeed;
	rotationmercury += mercury.ratationSpeed;
	rotationvenus += venus.ratationSpeed;
	rmars += mars.ratationSpeed;
	rjupiter += jupiter.ratationSpeed;
	rsaturn += saturn.ratationSpeed;
	ruranus += uranus.ratationSpeed;
	rneptune += neptune.ratationSpeed;

	Matrix4 rotationSkyBoxM = Matrix4::RotationY(rotationSkyBox);
	Matrix4 skyBoxMatrix = Transpose(rotationSkyBoxM * view * proj);

	Matrix4 sunM = Matrix4::RotationY(rotationSun) * Matrix4::Translation({ 0.0f, 0.0f, 0.0f });

	Matrix4 earthM = Matrix4::RotationY(rotationearth) * Matrix4::Translation({ sin(earth.angel) * earth.distanceFromCenter, 0.0f, cos(earth.angel) * earth.distanceFromCenter });

	Matrix4 mercuryM = Matrix4::RotationY(rotationmercury) * Matrix4::Translation({ sin(mercury.angel) * mercury.distanceFromCenter, 0.0f, cos(mercury.angel) * mercury.distanceFromCenter });

	Matrix4 venusM = Matrix4::RotationY(rotationvenus) * Matrix4::Translation({ sin(venus.angel) * venus.distanceFromCenter, 0.0f, cos(venus.angel) * venus.distanceFromCenter });

	Matrix4 marM = Matrix4::RotationY(rmars) * Matrix4::Translation({ sin(mars.angel) * mars.distanceFromCenter, 0.0f, cos(mars.angel) * mars.distanceFromCenter });
	Matrix4 jupiterM = Matrix4::RotationY(rjupiter) * Matrix4::Translation({ sin(jupiter.angel) * jupiter.distanceFromCenter, 0.0f, cos(jupiter.angel) * jupiter.distanceFromCenter });
	Matrix4 saturnM = Matrix4::RotationY(rsaturn) * Matrix4::Translation({ sin(saturn.angel) * saturn.distanceFromCenter, 0.0f, cos(saturn.angel) * saturn.distanceFromCenter });
	Matrix4 neptuneM = Matrix4::RotationY(ruranus) * Matrix4::Translation({ sin(neptune.angel) * neptune.distanceFromCenter, 0.0f, cos(neptune.angel) * neptune.distanceFromCenter });
	Matrix4 uranusM = Matrix4::RotationY(rneptune) * Matrix4::Translation({ sin(uranus.angel) * uranus.distanceFromCenter, 0.0f, cos(uranus.angel) * uranus.distanceFromCenter });

	Matrix4 wvpSun = Transpose(sunM * view * proj);
	Matrix4 wvpearth = Transpose(earthM * view * proj);
	Matrix4 wvpmercury = Transpose(mercuryM * view * proj);
	Matrix4 wvpvenus = Transpose(venusM * view * proj);
	Matrix4 wvpjupiter = Transpose(jupiterM * view * proj);
	Matrix4 wvpsaturn = Transpose(saturnM * view * proj);
	Matrix4 wvpneptune = Transpose(neptuneM * view * proj);
	Matrix4 wvpuranus = Transpose(uranusM * view * proj);
	Matrix4 wvpvmar = Transpose(marM * view * proj);

	mConstantBuffer.BindVS(0);
	//
	mConstantBuffer.Update(&wvpSun);
	sun.mTexture.BindPS(0);
	sun.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpearth);
	earth.mTexture.BindPS(0);
	earth.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpmercury);
	mercury.mTexture.BindPS(0);
	mercury.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpvenus);
	venus.mTexture.BindPS(0);
	venus.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpvmar);
	mars.mTexture.BindPS(0);
	mars.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpjupiter);
	jupiter.mTexture.BindPS(0);
	jupiter.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpsaturn);
	saturn.mTexture.BindPS(0);
	saturn.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpneptune);
	neptune.mTexture.BindPS(0);
	neptune.mMeshBuffer.Render();

	mConstantBuffer.Update(&wvpuranus);
	uranus.mTexture.BindPS(0);
	uranus.mMeshBuffer.Render();

	mConstantBuffer.Update(&skyBoxMatrix);
	mMengTexture.BindPS(0);
	mCubeMeshBuffer.Render();
}

void GameState::DebugUI()
{

}
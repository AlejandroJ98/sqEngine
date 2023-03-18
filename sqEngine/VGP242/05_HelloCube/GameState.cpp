#include "GameState.h"

using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine;

void GameState::Initialize()
{
	mCamera.SetPosition({0.0f,1.0f,-3.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f});
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


	//auto mCubeMesh = MeshBuilder::CreateCube();
	//auto mCubeMesh = MeshBuilder::CreatePlanePC(5,5,0.3);
	//auto mCubeMesh = MeshBuilder::Createcylinder(10, 30, 0.5);
	 //auto mCubeMesh = MeshBuilder::Createball(30, 100, 0.8);
	auto mCubeMesh = MeshBuilder::CreateSpherePC(1.1, 50, 50);
	mMeshBuffer.Initialize(mCubeMesh);

	//>??????   L"../../Assets/Shaders/DoTransform.fx"
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/DoTransform.fx"); 
	// -------------------------------------------


	// -------------------------------------------
	// Compile and create pixel shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
		
	HRESULT hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoTransform.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");
	auto device = GraphicsSystem::Get()->GetDevice();
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
	// -------------------------------------------
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	SafeRelease(mPixelShader);
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	mVertexShader.Bind();
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader,nullptr,0);

	/// /////////
	static float rotationX = -0.1f;
	static float rotationY = 0.0f;

	//rotationX += 0.01f;
	//rotationY += 0.01f;
	Matrix4 world = Matrix4::RotationY(rotationY);
	Matrix4 world1 = Matrix4::RotationX(rotationX);
	world = world * world1;

	Matrix4 view = mCamera.GetViewMatrix();
	Matrix4 proj = mCamera.GetProjectionMatrix();
	Matrix4 wvp = Transpose(world * view * proj);
	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);
	mMeshBuffer.Render();
}

void GameState::DebugUI()
{

}



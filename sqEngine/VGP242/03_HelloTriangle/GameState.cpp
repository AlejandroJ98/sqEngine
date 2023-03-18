#include "GameState.h"

using namespace sqEngine::Graphics;
using namespace sqEngine::Math;
using namespace sqEngine;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
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
	mVertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f }, Colors::AliceBlue });
	mVertices.push_back({ Vector3{  0.3f,  0.1f, 0.0f }, Colors::MintCream });
	mVertices.push_back({ Vector3{ -0.3f, 0.1f, 0.0f }, Colors::AliceBlue });
	
	mVertices.push_back({ Vector3{  0.0f,  -0.8f, 0.0f }, Colors::SandyBrown });
	mVertices.push_back({ Vector3{  -0.3f,  0.1f, 0.0f }, Colors::SandyBrown });
	mVertices.push_back({ Vector3{ 0.3f, 0.1f, 0.0f }, Colors::RosyBrown });

	
	

	mMeshBuffer.Initialize(mVertices);
	//mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/DoTransform.fx");
	// -------------------------------------------


	// -------------------------------------------
	// Compile and create pixel shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoSomething.fx",
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
	context->PSSetShader(mPixelShader, nullptr, 0);

	/// /////////

	Matrix4 world = Matrix4::RotationY(0);
	Matrix4 view = mCamera.GetViewMatrix();
	Matrix4 proj = mCamera.GetProjectionMatrix();
	Matrix4 wvp = Transpose(world * view * proj);
	mConstantBuffer.Update(&world);
	mConstantBuffer.BindVS(0);
	mMeshBuffer.Render();
}

void GameState::DebugUI()
{

}
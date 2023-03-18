#include <sqEngine/Inc/sqEngine.h>

class GameState : public sqEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct TransformData
	{
		sqEngine::Math::Matrix4 wvp;
	};

	sqEngine::Graphics::Camera mCamera;

	sqEngine::Graphics::VertexShader mVertexShader;
	sqEngine::Graphics::PixelShader mPixelShader;

	using TransformBuffer = sqEngine::Graphics::TypedConstantBuffer<TransformData>;
	TransformBuffer mTransformBuffer;

	sqEngine::Graphics::ConstantBuffer mConstantBuffer;

	sqEngine::Graphics::Texture mDiffuseMap;
	sqEngine::Graphics::Sampler mSampler;

	sqEngine::Graphics::Mesh mMesh;
	sqEngine::Graphics::MeshBuffer mMeshBuffer;

	sqEngine::Math::Vector3 mPosition = sqEngine::Math::Vector3::Zero;
	sqEngine::Math::Vector3 mRotation = sqEngine::Math::Vector3::Zero;

	float mFPS = 0.0f;
};
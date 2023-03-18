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
	
	std::vector<sqEngine::Graphics::VertexPC> mVertices;
	sqEngine::Graphics::Camera mCamera;
	sqEngine::Graphics::VertexShader mVertexShader;
	ID3D11PixelShader* mPixelShader = nullptr;
	sqEngine::Graphics::ConstantBuffer mConstantBuffer;
	sqEngine::Graphics::MeshBuffer mMeshBuffer;
};
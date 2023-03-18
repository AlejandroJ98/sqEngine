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
	
	sqEngine::Graphics::Camera mCamera;

	sqEngine::Graphics::DirectionalLight mDirectionalLight;

	sqEngine::Graphics::StandardEffect mStandardEffect;

	sqEngine::Graphics::Mesh mMesh;

	sqEngine::Graphics::RenderObject mEarthRenderObject;

	float mFPS = 0.0f;
	bool mShowNormals = false;
};
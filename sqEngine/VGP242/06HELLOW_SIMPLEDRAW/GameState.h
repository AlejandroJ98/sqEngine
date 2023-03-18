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
	

	sqEngine::Math::Vector3 mPosition = sqEngine::Math::Vector3::Zero;
	sqEngine::Graphics::Color mColor = sqEngine::Graphics::Colors::White;
};
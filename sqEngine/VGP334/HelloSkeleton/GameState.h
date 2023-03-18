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
	sqEngine::Graphics::ShadowEffect mShadowEffect;

	sqEngine::Graphics::Model model;

	sqEngine::Graphics::RenderObject mBallRenderObject;
	sqEngine::Graphics::RenderObject mGroundRenderObject;
	sqEngine::Graphics::Model mCharacterModel;

	sqEngine::Graphics::RenderGroup mGroup;

	sqEngine::Graphics::Animation mAnimation;
	sqEngine::Graphics::Animator mCharacterAnimator;

	float mFPS = 0.0f;
	float mAnimationTime = 0.0f;

	float mBallheight = 5.0f;
	float mRunningTime = 0.0f;
	bool mPause = false;
	bool mDrawSkeleton = false;
	int mDepth = 0;
};
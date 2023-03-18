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
	struct PlanetInfo
	{
		void Update(float deltaTime)
		{
			angel += revolutionSpeed * deltaTime;
		}

		float angel;
		float scale;
		float ratationSpeed;
		float distanceFromCenter;
		float revolutionSpeed;
		sqEngine::Graphics::Texture mTexture;
		sqEngine::Graphics::MeshBuffer mMeshBuffer;
		sqEngine::Graphics::MeshPX mMesh;
	};

	PlanetInfo sun;
	PlanetInfo mercury;
	PlanetInfo venus;
	PlanetInfo earth;
	PlanetInfo mars;
	PlanetInfo jupiter;
	PlanetInfo saturn;
	PlanetInfo neptune;
	PlanetInfo uranus;

	sqEngine::Graphics::MeshPX mCubeMesh;
	sqEngine::Graphics::MeshPX mEarthMesh;
	sqEngine::Graphics::Camera mCamera;
	
	sqEngine::Graphics::VertexShader mVertexShader;
	sqEngine::Graphics::PixelShader mPixelShader;
	
	sqEngine::Graphics::ConstantBuffer mConstantBuffer;
	sqEngine::Graphics::MeshBuffer mCubeMeshBuffer;
	sqEngine::Graphics::Texture mMengTexture;

	sqEngine::Graphics::Sampler mSampler;
};
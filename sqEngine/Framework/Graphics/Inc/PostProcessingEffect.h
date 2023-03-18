#pragma once
#include "Effect.h"
#include"ConstantBuffer.h"
#include"PixelShader.h"
#include"Sampler.h"
#include"VertexShader.h"

namespace sqEngine::Graphics
{
	class Camera;
	//class RenderObject;
	class Texture;

	class Texture;

	class PostProcessingEffect : public Effect
	{
	public:
		enum class Mode
		{
			None,
			Monochrome,
			Invert,
			Mirror,
			Blur,
			Combine2
		};

		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetTexture(const Texture* texture, uint32_t slot = 0);
		void SetMode(Mode mode);
		void SetMirrorScale(float mirrorScale);

		void DebugUI();


	private:
		struct PostProcessData
		{
			Mode mode = Mode::None;
			float param0 = 0.0f;
			float param1 = 0.0f;
			float param2 = 0.0f;
		};

		using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		PostProcessBuffer mPostProcessBuffer;

		Sampler mSampler;

		std::array<const Texture*, 4> mTexture;

		Mode mMode = Mode::None;

		float mMirrorScale = -1.0f;
	};
}
#include"Precompiled.h"
#include"PostProcessingEffect.h"
#include"Camera.h"
#include"GraphicsSystem.h"
#include"VertexTypes.h"
#include"RenderObject.h"
using namespace sqEngine;
using namespace sqEngine::Graphics;

void PostProcessingEffect::Initialize()
{
	mVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/PostProcess.fx");
	mPixelShader.Initialize(L"../../Assets/Shaders/PostProcess.fx");

	mPostProcessBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
	mSampler.Terminate();

	mPostProcessBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
	//ASSERT(mTexture != nullptr, "PostProcessingEffect -- no input texture!");

	mVertexShader.Bind();
	mPixelShader.Bind();

	PostProcessData data;
	data.mode = mMode;

	switch (mMode)
	{
		case Mode::None:
		{
			break;
		}
		case Mode::Monochrome:
		{
			break;
		}
		case Mode::Invert:
		{
			break;
		}
		case Mode::Mirror:
		{
			data.param0 = mMirrorScale;
			break;
		}
		case Mode::Blur:
		{
			auto gs = GraphicsSystem::Get();
			const auto screenWidth = gs->GetBackBufferWidth();
			const auto screenHeight = gs->GetBackBufferHeight();
			data.param0 = 4.0f / screenWidth;
			data.param1 = 4.0f / screenHeight;
			break;
		}
	}
	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);

	mSampler.BindPS(0);
	
	for (int i = 0; i < mTexture.size(); ++i)
	{
		if (mTexture[i])
			mTexture[i]->BindPS(i);
	}
}

void PostProcessingEffect::End()
{
	for(int i=0; i< mTexture.size(); ++i)
	Texture::UnbindPS(i);
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	//ASSERT(slot < mTexture.size(),"PostProcessingEffect -- Invalid slot index");
	mTexture[slot] = texture;
}

void PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void PostProcessingEffect::SetMirrorScale(float mirrorScale)
{
	mMirrorScale = mirrorScale;
}


void PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		const char* modeNames[] =
		{
			"None",
			"MonoChrome",
			"Invert",
			"Mirror",
			"Blur",
			"Combine2",
		};
		if (ImGui::Combo("Mode", &currentMode, modeNames, static_cast<int>(std::size(modeNames))))
		{
			mMode = static_cast<Mode>(currentMode);
		}
		if (mMode == Mode::Mirror)
		{
			ImGui::DragFloat("Scale", &mMirrorScale, 0.01f, -5.0f, 5.0f);
		}
	}
}
#include "Precompiled.h"
#include "TextureManager.h"

using namespace sqEngine;
using namespace sqEngine::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sInstance;
}

void sqEngine::Graphics::TextureManager::StaticInitialize(std::filesystem::path root)
{
	ASSERT(sInstance == nullptr, "TextureManager -- System already error");
	sInstance = std::make_unique<TextureManager>();
	sInstance->SetRootDirectory(std::move(root));
}

void sqEngine::Graphics::TextureManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance.reset();
	}
}

TextureManager* sqEngine::Graphics::TextureManager::Get()
{
	ASSERT(sInstance != nullptr, "TextureManager -- No system register ");
	return sInstance.get();
}

sqEngine::Graphics::TextureManager::~TextureManager()
{
	for (auto& [id, texture] : mInventory)
		texture->Terminate();
	mInventory.clear();
}

TextureId sqEngine::Graphics::TextureManager::LoadTexture(std::filesystem::path fileName)
{
	auto textureId = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({ textureId, nullptr });
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize(mRootDirectory/ fileName);
	}
	return textureId;
}

void sqEngine::Graphics::TextureManager::BindVS(TextureId id, uint32_t slot) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

void sqEngine::Graphics::TextureManager::BindPS(TextureId id, uint32_t slot) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}

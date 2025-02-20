// (C) king.com Ltd 2021
#include "TextureManager.h"
#include "SdlSurface.h"
#include "SdlWindow.h"

#include <SDL.h>

#include <algorithm>
#include <vector>

namespace King {

static int sTextureId = 0;

class TextureManager::TextureManagerImpl {
public:
	TextureManagerImpl(SDL_Renderer& renderer) : mRenderer(renderer) {}

	int LoadTexture(const std::string& path) {
		if (auto it = std::find_if(mTextures.begin(), mTextures.end(), [&path](const auto& desc) { return desc.path == path; }); it != mTextures.end()) {
			return it->id;
		}

		mTextures.push_back(TextureManagerImpl::TexDesc{ sTextureId++, path, SdlSurface(path.c_str(), mRenderer) });
		return mTextures.back().id;
	}
	SdlSurface* GetTexture(int id) {
		if (auto it = std::find_if(mTextures.begin(), mTextures.end(), [&id](const auto& desc) { return desc.id == id; }); it != mTextures.end()) {
			return &it->surface;
		}
		return nullptr;
	}
	
private:
	SDL_Renderer& mRenderer;
	struct TexDesc {
		int id;
		std::string path;
		SdlSurface surface;
	};
	std::vector<TexDesc> mTextures;
};

TextureManager::TextureManager(SdlWindow& window)
	: mImpl(new TextureManagerImpl(window.Renderer())) {}

TextureManager::~TextureManager() = default;

int TextureManager::LoadTexture(const std::string& path) {
	return mImpl->LoadTexture(path);
}

SdlSurface* TextureManager::GetTexture(int id) {
	return mImpl->GetTexture(id);
}

}

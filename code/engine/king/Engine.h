// (C) king.com Ltd 2021

#pragma once
#include <glm/fwd.hpp>
#include <memory>
#include <string>

namespace King {
	class Updater;
	class CollisionWorld;
	class Engine {
	public:
		Engine(const char* assetsDirectory);
		~Engine();

		CollisionWorld& GetCollisionWorld();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		bool GetMouseButtonPressed() const;
		bool GetKeyDown(int keyCode) const;

		void SetCursorVisible(bool visible);
		
		void Start(Updater& updater);
		void Quit();

		int LoadTexture(const char* name);
		int GetTextureHeight(int textureId) const;
		int GetTextureWidth(int textureId) const;
		void Render(int textureId, float x, float y, float rotation = 0.0f, float scale = 1.0f);


		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, float x, float y, float scale = 2.0f);
		void WriteCentered(const char* text, float x, float y, float scale = 2.0f);

		int GetWidth() const;
		int GetHeight() const;

	private:
		std::string mAssetsDirectory;
		struct EngineImplementation;
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}

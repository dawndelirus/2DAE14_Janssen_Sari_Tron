#pragma once
#include "Scene.h"

struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		void CreatePlayer(Scene& scene, float healtPosX, float peterPosX, int playerId) const;
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_pWindow{};
	};
}
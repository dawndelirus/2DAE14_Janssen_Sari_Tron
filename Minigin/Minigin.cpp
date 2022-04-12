#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Time.h"

#include "ComponentIncludes.h"

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	Scene& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto soBg = std::make_shared<GameObject>();
	auto textureBg = std::make_shared<Texture2DComponent>(soBg, "background.jpg");
	soBg->AddComponent(textureBg);
	scene.Add(soBg);

	auto soLogo = std::make_shared<GameObject>(216.f, 180.f, 0.f);
	auto textureLogo = std::make_shared<Texture2DComponent>(soLogo, "logo.png");
	soLogo->AddComponent(textureLogo);
	scene.Add(soLogo);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto soAssignment = std::make_shared<GameObject>(80, 20, 0);
	auto textAssignment = std::make_shared<dae::TextComponent>(soAssignment, "Programming 4 Assignment", font, glm::vec3(255.f, 255.f, 255.f));
	soAssignment->AddComponent(textAssignment);
	scene.Add(soAssignment);


	auto soFPS = std::make_shared<GameObject>();
	auto textFPS = std::make_shared<dae::TextComponent>(soFPS, "0", font, glm::vec3(255.f, 255.f, 255.f));
	soFPS->AddComponent(textFPS);
	auto FPS = std::make_shared<dae::FPSComponent>(soFPS, textFPS);
	soFPS->AddComponent(FPS);
	scene.Add(soFPS);

	//auto peterPepper = std::make_shared<GameObject>();
	//peterPepper->AddComponent(std::make_shared<PeterPepperComponent>(peterPepper));
	//peterPepper->AddComponent(std::make_shared<HealthComponent>(peterPepper, 5));
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = Time::GetInstance();

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		while (doContinue)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			time.SetDeltaTime(deltaTime);

			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();

			lastTime = currentTime;

			const auto sleepTime = currentTime + std::chrono::milliseconds(MsPerFrame) - std::chrono::high_resolution_clock::now();
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}

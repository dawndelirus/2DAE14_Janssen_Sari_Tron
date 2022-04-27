#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
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

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_pWindow);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	Scene& scene = SceneManager::GetInstance().CreateScene("Demo");

	//auto soBg = std::make_shared<GameObject>();
	//auto textureBg = std::make_shared<Texture2DComponent>(soBg, "background.jpg");
	//soBg->AddComponent(textureBg);
	//scene.Add(soBg);

	//auto soLogo = std::make_shared<GameObject>(216.f, 180.f, 0.f);
	//auto textureLogo = std::make_shared<Texture2DComponent>(soLogo, "logo.png");
	//soLogo->AddComponent(textureLogo);
	//scene.Add(soLogo);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto soAssignment = std::make_shared<GameObject>(80, 20, 0);
	auto textAssignment = std::make_shared<dae::TextComponent>(soAssignment, "Programming 4 Assignment", font, glm::vec3(255.f, 255.f, 255.f));
	soAssignment->AddComponent(textAssignment);
	scene.Add(soAssignment);

	//auto soFPS = std::make_shared<GameObject>();
	//auto textFPS = std::make_shared<dae::TextComponent>(soFPS, "0", font, glm::vec3(255.f, 255.f, 255.f));
	//soFPS->AddComponent(textFPS);
	//auto FPS = std::make_shared<dae::FPSComponent>(soFPS, textFPS);
	//soFPS->AddComponent(FPS);
	//scene.Add(soFPS);
	
	CreatePlayer(scene, glm::vec3(0.f, 70.f, 0.f), glm::vec3(640.f / 3.f, 480.f / 2.f, 0.f), 0);
	CreatePlayer(scene, glm::vec3(450.f, 70.f, 0.f), glm::vec3(640.f / 3.f * 2.f, 480.f / 2.f, 0.f), 1);
}

void dae::Minigin::CreatePlayer(Scene& scene, const glm::vec3& healtPos, const glm::vec3& peterPos, int playerId) const
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	// Peter
	auto peterObject = std::make_shared<GameObject>(peterPos);
	peterObject->AddComponent(std::make_shared<PeterPepperComponent>(peterObject));
	auto peterHealthComp = peterObject->AddComponent(std::make_shared<HealthComponent>(peterObject, 3));
	peterObject->AddComponent(std::make_shared<dae::Texture2DComponent>(peterObject, "PP_Idle.png"));

	// Input
	auto& input = InputManager::GetInstance();
	auto damageCommand = std::make_shared<TakeDamageCommand>(peterObject);
	input.AddInput(InputAction(playerId, ButtonState::downThisFrame, damageCommand, dae::ControllerButton::ButtonA));

	// Health
	auto healthObject = std::make_shared<GameObject>(healtPos);
	auto healthTextComp = std::make_shared<dae::TextComponent>(healthObject, "0", font, glm::vec3(255.f, 255.f, 255.f));
	healthObject->AddComponent(healthTextComp);
	auto healthDisplayComp = std::make_shared<HealthDisplayComponent>(healthObject, healthTextComp, peterHealthComp);
	healthObject->AddComponent(healthDisplayComp);

	peterHealthComp->AddObserver(healthDisplayComp);
	
	scene.Add(healthObject);
	scene.Add(peterObject);
}


void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
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

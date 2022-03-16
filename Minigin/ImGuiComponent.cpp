#include "MiniginPCH.h"
#include "ImGuiComponent.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui_plot.h"
#include <algorithm>

using namespace dae;

ImGuiComponent::ImGuiComponent(std::shared_ptr<GameObject> gameObject, std::shared_ptr<PlotIntComponent> intComp, std::shared_ptr<PlotGameObjectComponent> gameComp, SDL_Window* window)
	: BaseComponent(gameObject)
	, m_IntComponent{intComp}
	, m_GameObjectComponent{gameComp}
	, m_Window{ window }
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

ImGuiComponent::~ImGuiComponent()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiComponent::Update()
{
}

void ImGuiComponent::ImGuiWindow() const
{
	ImGui::Begin("Exercises Programming 4");

	ImGui::PlotConfig config{};
	config.scale.min = 0;
	config.tooltip.show = true;
	config.frame_size = ImVec2{ 200, 200 };
	config.grid_x.show = true;
	config.grid_x.size = 11;
	config.line_thickness = 2.f;

	if (ImGui::CollapsingHeader("Exercise 1"))
	{
		ImGui::Text("Graphing with Integers: ");
		ImGui::Separator();
		static char buf[32] = "1000000";
		ImGui::InputText("# Int samples", buf, IM_ARRAYSIZE(buf));
		
		static bool hasClicked = false;
		if (ImGui::Button("Trash the cashe"))
		{
			hasClicked = true;
			ImGui::SameLine();
			ImGui::Text("Processing...");
			m_IntComponent.lock()->Plot(std::atoi(buf));
		}

		if (hasClicked)
		{
			auto results = m_IntComponent.lock()->GetResults();

			config.scale.max = results[0];
			config.values.color = ImGui::GetColorU32(ImVec4{ 0.f, 255.f, 0.f, 1.f });
			config.values.ys = &results[0];
			config.values.count = static_cast<int>(results.size());
			config.values.xs = &m_IntComponent.lock()->GetSteps()[0];

			ImGui::Plot("Integers", config);
		}
	}

	if (ImGui::CollapsingHeader("Exercise 2"))
	{
		ImGui::Text("Graphing with GameObject3D structs: ");
		ImGui::Separator();
		static char buf[32] = "1000000";
		ImGui::InputText("# GameObject3D samples", buf, IM_ARRAYSIZE(buf));
		//ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
		
		static bool hasClicked3D = false;
		if (ImGui::Button("Trash the cashe with GameObject3D"))
		{
			hasClicked3D = true;
			ImGui::SameLine();
			ImGui::Text("Processing...");
			m_GameObjectComponent.lock()->Plot(std::atoi(buf));
		}

		if (hasClicked3D)
		{
			auto results = m_GameObjectComponent.lock()->GetResults();
			
			config.scale.max = results[0];
			config.values.color = ImGui::GetColorU32(ImVec4{ 255.f, 0.f, 0.f, 1.f });
			config.values.ys = &results[0];
			config.values.count = static_cast<int>(results.size());
			config.values.xs = &m_GameObjectComponent.lock()->GetSteps()[0];

			ImGui::Plot("Integers", config);
		}
		
		static bool hasClicked3DAlt = false;
		if (ImGui::Button("Trash the cashe with GameObject3DAlt"))
		{
			hasClicked3DAlt = true;
			ImGui::SameLine();
			ImGui::Text("Processing...");
			m_GameObjectComponent.lock()->PlotAlt(std::atoi(buf));
		}

		if (hasClicked3DAlt)
		{
			auto results = m_GameObjectComponent.lock()->GetResultsAlt();
			
			config.scale.max = results[0];
			config.values.color = ImGui::GetColorU32(ImVec4{ 100.f, 100.f, 0.f, 1.f });
			config.values.ys = &results[0];
			config.values.count = static_cast<int>(results.size());
			config.values.xs = &m_GameObjectComponent.lock()->GetSteps()[0];
			ImGui::Plot("Integers", config);
		}
	}

	ImGui::End();
}


void ImGuiComponent::Render() const
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	ImGuiWindow();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

